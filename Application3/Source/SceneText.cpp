#include "SceneText.h"
#include "GL\glew.h"
#include "Time.h"
#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "stdint.h"
#include "Time.h"
#include <iostream>
#include <sstream>
#include "collisionSphere.h"
#include <sstream>
#include "Object.h"


SceneText::SceneText()
{
}

SceneText ::~SceneText()
{
}

void SceneText::Init()
{
	// Init VBO here

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	rotateCharacter = 0;
	rotateTele = 0;
	Teleport = false;
	isFixed = false;
	engineHeat = 0;
	LSPEED = 20.f;
	lightOn = true;
	JetPackActivated = true;
	MovementSpeed = 10;

	//Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");


	glUseProgram(m_programID);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	lights[0].type = Light::LIGHT_SPOT;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);


	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("Lightball", Color(1, 1, 1), 10, 10, 1);// TEST TEST TEST

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");

	meshList[GEO_NOOB] = MeshBuilder::GenerateQuad("noob", Color(1, 1, 1), 1.f);
	meshList[GEO_NOOB]->textureID = LoadTGA("Image//noob.tga");

	meshList[GEO_MODEL1] = MeshBuilder::GenerateOBJ("model1", "OBJ//chair.obj");
	meshList[GEO_MODEL1]->textureID = LoadTGA("Image//chair.tga");

	meshList[GEO_MODEL4] = MeshBuilder::GenerateOBJ("model1", "OBJ//doorman.obj");
	meshList[GEO_MODEL4]->textureID = LoadTGA("Image//doorman.tga");
	cV[GEO_MODEL4] = new collisionSphere(2.f);
	Player = ((collisionSphere*)(cV[GEO_MODEL4]));
	Player->setCOORD(0, 0, 0);
	Player->setCOORD(1, 1, 1);

	meshList[THIRDFLOOR] = MeshBuilder::GenerateOBJ("ThirdFloor", "OBJ//ThirdFloor.obj");
	meshList[THIRDFLOOR]->textureID = LoadTGA("Image//ThirdFloor.tga");
	cV[THIRDFLOOR] = new collisionSphere(25.f);
	((collisionSphere*)(cV[THIRDFLOOR]))->setCOORD(0, 0, 0);

	meshList[TELEPORTER] = MeshBuilder::GenerateOBJ("elevator", "OBJ//Elevator.obj");
	meshList[TELEPORTER]->textureID = LoadTGA("Image//Elevator.tga");
	cV[TELEPORTER] = new collisionSphere(0.5f);// TEST TEST TEST
	Teleporter = ((collisionSphere*)cV[TELEPORTER]);
	Teleporter->setCOORD(10, 0, 0);

	meshList[GEO_MODEL6] = MeshBuilder::GenerateOBJ("model1", "OBJ//winebottle.obj");
	meshList[GEO_MODEL6]->textureID = LoadTGA("Image//winebottle.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//ExportedFont.tga");


	Object Player;
	Player.Name = "Player";
	Player.CollisionTrigger = true;
	Player.OBJcV = new collisionSphere(2.f, Vector3(-10, 0, 0));
	Player.OBJmesh = MeshBuilder::GenerateOBJ("Player", "OBJ//doorman.obj");
	Player.OBJmesh->textureID = LoadTGA("Image//doorman.tga");
	SP.Add(Player);

	Object Can;
	Can.Name = "Bowser";
	Can.CollisionTrigger = false;
	Can.OBJcV = new collisionSphere(2.f, Vector3(5, 0, 0));
	Can.OBJcV->setEffect(0);
	Can.OBJcV->setVelocity(Vector3(0, 5, 0));
	Can.OBJmesh = MeshBuilder::GenerateOBJ("Can", "OBJ//Bowser.obj");
	Can.OBJmesh->textureID = LoadTGA("Image//CanTexture.tga");
	SP.Add(Can);

	Object Wine;
	Wine.Name = "Wine";
	Wine.CollisionTrigger = false;
	Wine.OBJcV = new collisionSphere(2.f, Vector3(15, 0, 0));
	Wine.OBJcV->setEffect(1);
	Wine.OBJcV->setVelocity(Vector3(0, 5, 0));
	Wine.OBJmesh = MeshBuilder::GenerateOBJ("Wine", "OBJ//winebottle.obj");
	Wine.OBJmesh->textureID = LoadTGA("Image//winebottle.tga");
	SP.Add(Wine);


	//Initialize camera settings
	camera.Init(Vector3(0, 25, 20), Vector3(0, 0, 0), Vector3(0, 1, 0));

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 5000.f);
	projectionStack.LoadMatrix(projection);

	//remove all glGenBuffers, glBindBuffer, glBufferData code
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

double SceneText::DtCopy = 0;

void SceneText::Update(double dt)
{
	DtCopy = dt;
	collisionVolume Here;
	Here.AllowPickUp = false;
	Here.AllowForce = false;
	Here.AllowActivate = false;
	if (Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	if (Application::IsKeyPressed('I'))// TEST TEST TEST
		lights[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(LSPEED * dt);

	if (Application::IsKeyPressed('T'))
		lightOn = true;

	if (Application::IsKeyPressed('Y'))
		lightOn = false;

	if (Application::IsKeyPressed('Z'))   //POINT LIGHT
	{
		lights[0].type = Light::LIGHT_POINT;
	}
	else if (Application::IsKeyPressed('X')) //DIRECTIONAL LIGHT
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
	}
	else if (Application::IsKeyPressed('C')) //SPOT LIGHT
	{
		lights[0].type = Light::LIGHT_SPOT;
	}
	if (Application::IsKeyPressed('W'))
	{
		Mtx44 charRotate;

		SP.Call("Player").OBJcV->setVelocity(Vector3(MovementSpeed*dt, 0, 0));
		charRotate.SetToRotation(SP.Call("Player").OBJcV->getFace(), 0, 1, 0);
		SP.Call("Player").OBJcV->setVelocity(charRotate * SP.Call("Player").OBJcV->getVelocity());
		SP.Call("Player").OBJcV->setCentre(SP.Call("Player").OBJcV->getCentre() + SP.Call("Player").OBJcV->getVelocity());
		Here.MainFace = 0;
	}
	if (Application::IsKeyPressed('S'))
	{
		Mtx44 charRotate;

		SP.Call("Player").OBJcV->setVelocity(Vector3(MovementSpeed*dt, 0, 0));
		charRotate.SetToRotation(SP.Call("Player").OBJcV->getFace(), 0, 1, 0);
		SP.Call("Player").OBJcV->setVelocity(charRotate * SP.Call("Player").OBJcV->getVelocity());
		SP.Call("Player").OBJcV->setCentre(SP.Call("Player").OBJcV->getCentre() - SP.Call("Player").OBJcV->getVelocity());
		Here.MainFace = 1;
	}
	if (Application::IsKeyPressed('A'))
	{
		Mtx44 charRotate;

		SP.Call("Player").OBJcV->setVelocity(Vector3(0, 0, MovementSpeed*dt));
		charRotate.SetToRotation(SP.Call("Player").OBJcV->getFace(), 0, 1, 0);
		SP.Call("Player").OBJcV->setVelocity(charRotate * SP.Call("Player").OBJcV->getVelocity());
		SP.Call("Player").OBJcV->setCentre(SP.Call("Player").OBJcV->getCentre() - SP.Call("Player").OBJcV->getVelocity());
		Here.MainFace = 2;
	}
	if (Application::IsKeyPressed('D'))
	{
		Mtx44 charRotate;

		SP.Call("Player").OBJcV->setVelocity(Vector3(0, 0, MovementSpeed*dt));
		charRotate.SetToRotation(SP.Call("Player").OBJcV->getFace(), 0, 1, 0);
		SP.Call("Player").OBJcV->setVelocity(charRotate * SP.Call("Player").OBJcV->getVelocity());
		SP.Call("Player").OBJcV->setCentre(SP.Call("Player").OBJcV->getCentre() + SP.Call("Player").OBJcV->getVelocity());
		Here.MainFace = 3;
	}

	if (Application::IsKeyPressed(VK_RIGHT))
		SP.Call("Player").OBJcV->setFace(SP.Call("Player").OBJcV->getFace() - 5.f);// TEST TEST TEST
	else if (Application::IsKeyPressed(VK_LEFT))
		SP.Call("Player").OBJcV->setFace(SP.Call("Player").OBJcV->getFace() + 5.f);

	/*
	if (Application::IsKeyPressed('E') && Player->checkCollision(Teleporter))
	{
	Teleport = true;
	isFixed = true;
	}
	if (Teleport)
	rotateTele += 15;
	if (rotateTele > 1080)
	{
	Teleport = false;
	isFixed = false;
	rotateTele = 0;
	Player->setCOORD(0,0,0);
	}

	//JetPack
	if (Application::IsKeyPressed('F') && JetPackActivated)
	{

	Player->setCOORD(Player->getCOORD(0), Player->getCOORD(1) + 0.4, Player->getCOORD(2));
	engineHeat += 40;
	if (engineHeat >= 2000)
	JetPackActivated = false;
	}
	if (Player->getCOORD(1) > 0)
	{
	Player->setCOORD(Player->getCOORD(0), Player->getCOORD(1) - 0.1, Player->getCOORD(2));
	if (engineHeat < 50 && !JetPackActivated)
	JetPackActivated = true;
	else
	engineHeat -= 10;
	}

	/* COLLISION EFFECTS
	while (SP.CheckCollision().Name != "Finished")
	//Pick Ups

	//Stationary Bounds

	//Activations

	//Vector Addition

	float test = engineHeat;
	std::ostringstream ss;
	ss<< test;
	warningTest = ss.str();
	*/
	if (Application::IsKeyPressed('E'))
	{
		Here.AllowPickUp = true;
		Here.AllowForce = true;
		Here.AllowActivate = true;
	}

	SP.CheckCollision();

	if (Application::IsKeyPressed('G'))
	{
		SP.Call("Player").OBJcV->setCentre(Vector3(0.5, 0, 0) + SP.Call("Player").OBJcV->getCentre());
	}

	if (SP.Call("Player").OBJcV->getFace() > 180)
		SP.Call("Player").OBJcV->setFace(SP.Call("Player").OBJcV->getFace() - 360);
	if (SP.Call("Player").OBJcV->getFace() < -180)
		SP.Call("Player").OBJcV->setFace(SP.Call("Player").OBJcV->getFace() + 360);
	camera.Update(dt);
}

void SceneText::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);// TEST TEST TEST
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once 
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneText::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)// TEST TEST TEST
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneText::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;// TEST TEST TEST
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.5f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void SceneText::RenderImageOnScreen(Mesh* mesh, float size, float x, float y)
{

}

void SceneText::RenderSkybox()
{
	modelStack.PushMatrix();
	//scale, translate, rotate// TEST TEST TEST
	modelStack.Translate(0, 0, -0.498);
	modelStack.Rotate(-90, 0, 0, 1);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0, 0, 0.498);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Rotate(180, 1, 0, 0);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(-0.498, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0.498, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();// TEST TEST TEST
	//scale, translate, rotate
	modelStack.Translate(0, -0.498, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0, 0.498, 0);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	/*
	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0,0.2,0);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	// TEST TEST TEST


	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0.8,0,0);
	modelStack.Rotate(-90,0,0,1);
	modelStack.Rotate(90,1,0,0);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0,0,-0.2);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
	*/
}

void SceneText::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x,
		camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);

	modelStack.LoadIdentity();

	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;// TEST TEST TEST
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}


	Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);

	RenderMesh(meshList[GEO_AXES], false);
	// TEST TEST TEST
	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], lightOn);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);
	RenderSkybox();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Player").OBJcV->getCOORD(0),
		SP.Call("Player").OBJcV->getCOORD(1),
		SP.Call("Player").OBJcV->getCOORD(2));
	modelStack.Rotate(SP.Call("Player").OBJcV->getFace() + 90, 0, 1, 0);
	RenderMesh(SP.Call("Player").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Bowser").OBJcV->getCOORD(0),
		SP.Call("Bowser").OBJcV->getCOORD(1),
		SP.Call("Bowser").OBJcV->getCOORD(2));
	RenderMesh(SP.Call("Bowser").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Wine").OBJcV->getCOORD(0),
		SP.Call("Wine").OBJcV->getCOORD(1),
		SP.Call("Wine").OBJcV->getCOORD(2));
	RenderMesh(SP.Call("Wine").OBJmesh, false);
	modelStack.PopMatrix();
}

void SceneText::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

}

