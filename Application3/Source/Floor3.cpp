#include "Floor3.h"
#include "GL\glew.h"
#include "Time.h"
#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "stdint.h"
#include <iostream>
#include <sstream>
#include "collisionSphere.h"
#include "Object.h"


Floor3::Floor3()
{
}

Floor3 ::~Floor3()
{
}

void Floor3::Init()
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

	Teleport = false;
	isFixed = false;
	LSPEED = 20.f;
	MovementSpeed = 10;
	Floor3Timer = 2100;
	Limiter = 1950;
	SP.Call("Teleporter").OBJcV->setActivate(false);

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
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");


	glUseProgram(m_programID);



	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 5;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	
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

	meshList[GEO_MODEL1] = MeshBuilder::GenerateOBJ("model1", "OBJ//chair.obj");
	meshList[GEO_MODEL1]->textureID = LoadTGA("Image//chair.tga");
	meshList[GEO_MODEL1]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_MODEL1]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_MODEL1]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_MODEL1]->material.kShininess = 5.f;

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//ExportedFont.tga");

	Object Floor;
	Floor.Name = "ThirdLevel";//IMPORTANT
	Floor.ReverseCollision = true;
	Floor.OBJcV = new AABB(80.f, 75.f, 100.f, Vector3(0,0,0)); //IMPORTANT
	Floor.OBJcV->setEffect(1); //IMPORTANT
	Floor.OBJcV->setVelocity(Vector3(0, 5, 0));
	Floor.OBJmesh = MeshBuilder::GenerateOBJ("ThirdLevel", "OBJ//Floor3.obj");
	Floor.OBJmesh->textureID = LoadTGA("Image//Floor3.tga");
	SP.Add(Floor);

	Object ProfessorX;
	ProfessorX.Name = "ProfessorX";//IMPORTANT
	ProfessorX.OBJcV = new collisionSphere(2.f, Vector3(0,0,15)); //IMPORTANT
	ProfessorX.OBJcV->setEffect(2); //IMPORTANT
	ProfessorX.OBJcV->setVelocity(Vector3(0, 5, 0));
	ProfessorX.OBJmesh = MeshBuilder::GenerateOBJ("ProfessorX", "OBJ//ProfessorX.obj");
	ProfessorX.OBJmesh->textureID = LoadTGA("Image//ProfessorX.tga");
	SP.Add(ProfessorX);

	Object Totem;
	Totem.Name = "Totem";//IMPORTANT
	Totem.OBJcV = new collisionSphere(1.f, Vector3(0,0,0)); //IMPORTANT
	Totem.OBJcV->setEffect(2); //IMPORTANT
	Totem.OBJcV->setVelocity(Vector3(0, 5, 0));
	Totem.OBJmesh = MeshBuilder::GenerateOBJ("Totem", "OBJ//Totem.obj");
	Totem.OBJmesh->textureID = LoadTGA("Image//Totem.tga");
	SP.Add(Totem);


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

double Floor3::DtCopy = 0;

void Floor3::Update(double dt)
{
	DtCopy = dt;
	collisionVolume Here;
	Here.AllowPickUp = false;
	Here.AllowForce = false;
	Here.AllowActivate = false;
	Here.MoveKeysPressed = 0;
	Here.MainFace[0] = false;Here.MainFace[1] = false;
	Here.MainFace[2] = false;Here.MainFace[3] = false;
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



	
	if (Application::IsKeyPressed('W'))
	{
		Mtx44 charRotate;

		SP.Call("Player").OBJcV->setVelocity(Vector3(MovementSpeed*dt, 0, 0));
		charRotate.SetToRotation(SP.Call("Player").OBJcV->getFace(), 0, 1, 0);
		SP.Call("Player").OBJcV->setVelocity(charRotate * SP.Call("Player").OBJcV->getVelocity());
		SP.Call("Player").OBJcV->setCentre(SP.Call("Player").OBJcV->getCentre() + SP.Call("Player").OBJcV->getVelocity());
		Here.MainFace[0] = true;
		Here.MoveKeysPressed++;
	}
	if (Application::IsKeyPressed('S'))
	{
		Mtx44 charRotate;

		SP.Call("Player").OBJcV->setVelocity(Vector3(MovementSpeed*dt, 0, 0));
		charRotate.SetToRotation(SP.Call("Player").OBJcV->getFace(), 0, 1, 0);
		SP.Call("Player").OBJcV->setVelocity(charRotate * SP.Call("Player").OBJcV->getVelocity());
		SP.Call("Player").OBJcV->setCentre(SP.Call("Player").OBJcV->getCentre() - SP.Call("Player").OBJcV->getVelocity());
		Here.MainFace[1] = true;
		Here.MoveKeysPressed++;
	}
	if (Application::IsKeyPressed('A'))
	{
		Mtx44 charRotate;

		SP.Call("Player").OBJcV->setVelocity(Vector3(0, 0, MovementSpeed*dt));
		charRotate.SetToRotation(SP.Call("Player").OBJcV->getFace(), 0, 1, 0);
		SP.Call("Player").OBJcV->setVelocity(charRotate * SP.Call("Player").OBJcV->getVelocity());
		SP.Call("Player").OBJcV->setCentre(SP.Call("Player").OBJcV->getCentre() - SP.Call("Player").OBJcV->getVelocity());
		Here.MainFace[2] = true;
		Here.MoveKeysPressed++;
	}
	if (Application::IsKeyPressed('D'))
	{
		Mtx44 charRotate;

		SP.Call("Player").OBJcV->setVelocity(Vector3(0, 0, MovementSpeed*dt));
		charRotate.SetToRotation(SP.Call("Player").OBJcV->getFace(), 0, 1, 0);
		SP.Call("Player").OBJcV->setVelocity(charRotate * SP.Call("Player").OBJcV->getVelocity());
		SP.Call("Player").OBJcV->setCentre(SP.Call("Player").OBJcV->getCentre() + SP.Call("Player").OBJcV->getVelocity());
		Here.MainFace[3] = true;
		Here.MoveKeysPressed++;
	}

	if (Application::IsKeyPressed(VK_RIGHT))
		SP.Call("Player").OBJcV->setFace(SP.Call("Player").OBJcV->getFace() - 5.f);// TEST TEST TEST
	else if (Application::IsKeyPressed(VK_LEFT))
		SP.Call("Player").OBJcV->setFace(SP.Call("Player").OBJcV->getFace() + 5.f);

	if (Application::IsKeyPressed('F'))
		SP.Call("Player").OBJcV->Jump(0.1f);
	
	if (Application::IsKeyPressed('E'))
	{
		Here.AllowPickUp = true;
		Here.AllowForce = true;
		Here.AllowActivate = true;
	}

	Floor3Timer -= 0.5f;
	if (Floor3Timer <= Limiter)
	{
		Limiter -= 150;
		SP.Call("ProfessorX").OBJcV->setCOORD(5,0,0);
	}

	if (SP.Call("ProfessorX").OBJcV->getActivate())
	{
		//TeleporterIsNowHere
	}

	std::ostringstream ss;
	ss << Floor3Timer;
	MyTimer = ss.str();

	SP.CheckCollision();
	SP.Gravity();
	UpdateCrosshair();

	//SP.Call("Bowser").OBJcV->Chase(SP.Call("Player").OBJcV, 0.1, true);

	if (SP.Call("Player").OBJcV->getFace() > 180)
		SP.Call("Player").OBJcV->setFace(SP.Call("Player").OBJcV->getFace() - 360);
	if (SP.Call("Player").OBJcV->getFace() < -180)
		SP.Call("Player").OBJcV->setFace(SP.Call("Player").OBJcV->getFace() + 360);
	camera.Update(dt);
}

void Floor3::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x,
		camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);

	modelStack.LoadIdentity();



	Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);

	RenderMesh(meshList[GEO_AXES], false);
	// TEST TEST TEST
	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0,5,0);
	RenderMesh(meshList[GEO_MODEL1], true);
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
	modelStack.Translate(SP.Call("ThirdLevel").OBJcV->getCOORD(0),
		SP.Call("ThirdLevel").OBJcV->getCOORD(1),
		SP.Call("ThirdLevel").OBJcV->getCOORD(2));
	RenderMesh(SP.Call("ThirdLevel").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Crosshair").OBJcV->getCOORD(0),
		SP.Call("Crosshair").OBJcV->getCOORD(1),
		SP.Call("Crosshair").OBJcV->getCOORD(2));
	RenderMesh(SP.Call("Crosshair").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Teleporter").OBJcV->getCOORD(0),
		SP.Call("Teleporter").OBJcV->getCOORD(1),
		SP.Call("Teleporter").OBJcV->getCOORD(2));
	RenderMesh(SP.Call("Teleporter").OBJmesh, false);
	modelStack.PopMatrix();

	RenderObjective();
	RenderFloor3();
}

void Floor3::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

}

void Floor3::RenderObjective()
{
	modelStack.PushMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], MyTimer, Color(1,1,1),10,6.5f,5.5f);
	modelStack.PopMatrix();

	if (Floor3Timer >= 2000)
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "It's Raining Bullets and Meteors!!!", Color(1, 1, 1), 10, 1, 5);
		modelStack.PopMatrix();
	}
	if (SP.Call("ProfessorX").OBJcV->getActivate() == false)
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Chase ProfessorX and Take Chemical X", Color(1, 1, 1), 7, 1, 0.2);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "YOU HAVE TAKEN CHEMICAL X", Color(1, 1, 1), 7, 1, 0.2);
		modelStack.PopMatrix();
	}
}

void Floor3::RenderFloor3()
{
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("ProfessorX").OBJcV->getCOORD(0),
		SP.Call("ProfessorX").OBJcV->getCOORD(1),
		SP.Call("ProfessorX").OBJcV->getCOORD(2));
	RenderMesh(SP.Call("ProfessorX").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Totem").OBJcV->getCOORD(0),
		SP.Call("Totem").OBJcV->getCOORD(1),
		SP.Call("Totem").OBJcV->getCOORD(2));
	RenderMesh(SP.Call("Totem").OBJmesh, false);
	modelStack.PopMatrix();
}

