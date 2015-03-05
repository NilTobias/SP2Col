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
	SP3.Call("Teleporter").OBJcV->setActivate(false);
	speed = 0.2;
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

	Object Player;
	Player.Name = "Player";
	Player.OBJcV = new collisionSphere(2.f, Vector3(-10, 0, 0));
	Player.OBJcV->setLiving(true);
	Player.OBJmesh = MeshBuilder::GenerateOBJ("Player", "OBJ//doorman.obj");
	Player.OBJmesh->textureID = LoadTGA("Image//doorman.tga");
	SP3.Add(Player);


	Object Teleporter;
	Teleporter.Name = "Teleporter";
	Teleporter.Gravity = false;
	Teleporter.OBJcV = new collisionSphere(1.5f, Vector3(0, 0, 10));
	Teleporter.OBJcV->setEffect(2);
	Teleporter.OBJmesh = MeshBuilder::GenerateOBJ("Tele", "OBJ//Elevator.obj");
	Teleporter.OBJmesh->textureID = LoadTGA("Image//Elevator.tga");
	SP3.Add(Teleporter);

	Object Crosshair;
	Crosshair.Name = "Crosshair";
	Crosshair.Gravity = false;
	Crosshair.OBJcV = new collisionSphere(0.1f, Vector3(0, 0, 0));
	Crosshair.OBJcV->setCursor(true);
	Crosshair.OBJmesh = MeshBuilder::GenerateQuad("Crosshair", Color(1, 1, 1), 1.f);
	Crosshair.OBJmesh->textureID = LoadTGA("Image//Hand.tga");
	SP3.Add(Crosshair);

	Object Floor;
	Floor.Name = "ThirdLevel";//IMPORTANT
	Floor.ReverseCollision = true;
	Floor.OBJcV = new AABB(80.f, 75.f, 100.f, Vector3(0,0,0)); //IMPORTANT
	Floor.OBJcV->setEffect(1); //IMPORTANT
	Floor.OBJcV->setVelocity(Vector3(0, 5, 0));
	Floor.OBJmesh = MeshBuilder::GenerateOBJ("ThirdLevel", "OBJ//Floor3.obj");
	Floor.OBJmesh->textureID = LoadTGA("Image//Floor3.tga");
	SP3.Add(Floor);

	Object ProfessorX;
	ProfessorX.Name = "ProfessorX";//IMPORTANT
	ProfessorX.OBJcV = new collisionSphere(2.f, Vector3(0,0,15)); //IMPORTANT
	ProfessorX.OBJcV->setEffect(2); //IMPORTANT
	ProfessorX.OBJcV->setVelocity(Vector3(0, 5, 0));
	ProfessorX.OBJmesh = MeshBuilder::GenerateOBJ("ProfessorX", "OBJ//ProfessorX.obj");
	ProfessorX.OBJmesh->textureID = LoadTGA("Image//ProfessorX.tga");
	SP3.Add(ProfessorX);

	Object Totem;
	Totem.Name = "Totem";//IMPORTANT
	Totem.OBJcV = new collisionSphere(1.f, Vector3(-39,0,-35)); //IMPORTANT
	Totem.OBJcV->setEffect(2); //IMPORTANT
	Totem.OBJcV->setVelocity(Vector3(0, 5, 0));
	Totem.OBJmesh = MeshBuilder::GenerateOBJ("Totem", "OBJ//Totem.obj");
	Totem.OBJmesh->textureID = LoadTGA("Image//Totem.tga");
	SP3.Add(Totem);

	Object Totem2;
	Totem2.Name = "Totem2";//IMPORTANT
	Totem2.OBJcV = new collisionSphere(1.f, Vector3(40,0,45)); //IMPORTANT
	Totem2.OBJcV->setEffect(2); //IMPORTANT
	Totem2.OBJcV->setVelocity(Vector3(0, 5, 0));
	Totem2.OBJmesh = MeshBuilder::GenerateOBJ("Totem2", "OBJ//Totem.obj");
	Totem2.OBJmesh->textureID = LoadTGA("Image//Totem.tga");
	SP3.Add(Totem2);

	Object Totem3;
	Totem3.Name = "Totem3";//IMPORTANT
	Totem3.OBJcV = new collisionSphere(1.f, Vector3(40,0,-34)); //IMPORTANT
	Totem3.OBJcV->setEffect(2); //IMPORTANT
	Totem3.OBJcV->setVelocity(Vector3(0, 5, 0));
	Totem3.OBJmesh = MeshBuilder::GenerateOBJ("Totem3", "OBJ//Totem.obj");
	Totem3.OBJmesh->textureID = LoadTGA("Image//Totem.tga");
	SP3.Add(Totem3);

	Object Totem4;
	Totem4.Name = "Totem4";//IMPORTANT
	Totem4.OBJcV = new collisionSphere(1.f, Vector3(-35,0,43)); //IMPORTANT
	Totem4.OBJcV->setEffect(2); //IMPORTANT
	Totem4.OBJcV->setVelocity(Vector3(0, 5, 0));
	Totem4.OBJmesh = MeshBuilder::GenerateOBJ("Totem4", "OBJ//Totem.obj");
	Totem4.OBJmesh->textureID = LoadTGA("Image//Totem.tga");
	SP3.Add(Totem4);

	Object Bullet;
	Bullet.Name = "Bullet";//IMPORTANT
	Bullet.OBJcV = new collisionSphere(1.f, Vector3(-39,5,-35)); //IMPORTANT
	Bullet.OBJcV->setEffect(2); //IMPORTANT
	Bullet.OBJcV->setVelocity(Vector3(0, 5, 0));
	Bullet.OBJmesh = MeshBuilder::GenerateSphere("Bullet",Color(1,0,0),10,10,0.3f);
	//Bullet.OBJmesh->textureID = LoadTGA("Image//dart.tga");
	SP3.Add(Bullet);

	Object Bullet2;
	Bullet2.Name = "Bullet2";//IMPORTANT
	Bullet2.OBJcV = new collisionSphere(1.f, Vector3(40,0,45)); //IMPORTANT
	Bullet2.OBJcV->setEffect(2); //IMPORTANT
	Bullet2.OBJcV->setVelocity(Vector3(0, 5, 0));
	Bullet2.OBJmesh = MeshBuilder::GenerateSphere("Bullet2",Color(1,0,0),10,10,0.3f);
	//Bullet.OBJmesh->textureID = LoadTGA("Image//dart.tga");
	SP3.Add(Bullet2);

	Object Bullet3;
	Bullet3.Name = "Bullet3";//IMPORTANT
	Bullet3.OBJcV = new collisionSphere(1.f, Vector3(40,0,-34)); //IMPORTANT
	Bullet3.OBJcV->setEffect(2); //IMPORTANT
	Bullet3.OBJcV->setVelocity(Vector3(0, 5, 0));
	Bullet3.OBJmesh = MeshBuilder::GenerateSphere("Bullet3",Color(1,0,0),10,10,0.3f);
	//Bullet.OBJmesh->textureID = LoadTGA("Image//dart.tga");
	SP3.Add(Bullet3);

	Object Bullet4;
	Bullet4.Name = "Bullet4";//IMPORTANT
	Bullet4.OBJcV = new collisionSphere(1.f, Vector3(-35,0,43)); //IMPORTANT
	Bullet4.OBJcV->setEffect(2); //IMPORTANT
	Bullet4.OBJcV->setVelocity(Vector3(0, 5, 0));
	Bullet4.OBJmesh = MeshBuilder::GenerateSphere("Bullet4",Color(1,0,0),10,10,0.3f);
	//Bullet.OBJmesh->textureID = LoadTGA("Image//dart.tga");
	SP3.Add(Bullet4);

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
	int randomX = rand() % 57 + -25;
	int randomZ = rand() % 44 + -19;
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

		SP3.Call("Player").OBJcV->setVelocity(Vector3(MovementSpeed*dt, 0, 0));
		charRotate.SetToRotation(SP3.Call("Player").OBJcV->getFace(), 0, 1, 0);
		SP3.Call("Player").OBJcV->setVelocity(charRotate * SP3.Call("Player").OBJcV->getVelocity());
		SP3.Call("Player").OBJcV->setCentre(SP3.Call("Player").OBJcV->getCentre() + SP3.Call("Player").OBJcV->getVelocity());
		Here.MainFace[0] = true;
		Here.MoveKeysPressed++;
	}
	if (Application::IsKeyPressed('S'))
	{
		Mtx44 charRotate;

		SP3.Call("Player").OBJcV->setVelocity(Vector3(MovementSpeed*dt, 0, 0));
		charRotate.SetToRotation(SP3.Call("Player").OBJcV->getFace(), 0, 1, 0);
		SP3.Call("Player").OBJcV->setVelocity(charRotate * SP3.Call("Player").OBJcV->getVelocity());
		SP3.Call("Player").OBJcV->setCentre(SP3.Call("Player").OBJcV->getCentre() - SP3.Call("Player").OBJcV->getVelocity());
		Here.MainFace[1] = true;
		Here.MoveKeysPressed++;
	}
	if (Application::IsKeyPressed('A'))
	{
		Mtx44 charRotate;

		SP3.Call("Player").OBJcV->setVelocity(Vector3(0, 0, MovementSpeed*dt));
		charRotate.SetToRotation(SP3.Call("Player").OBJcV->getFace(), 0, 1, 0);
		SP3.Call("Player").OBJcV->setVelocity(charRotate * SP3.Call("Player").OBJcV->getVelocity());
		SP3.Call("Player").OBJcV->setCentre(SP3.Call("Player").OBJcV->getCentre() - SP3.Call("Player").OBJcV->getVelocity());
		Here.MainFace[2] = true;
		Here.MoveKeysPressed++;
	}
	if (Application::IsKeyPressed('D'))
	{
		Mtx44 charRotate;

		SP3.Call("Player").OBJcV->setVelocity(Vector3(0, 0, MovementSpeed*dt));
		charRotate.SetToRotation(SP3.Call("Player").OBJcV->getFace(), 0, 1, 0);
		SP3.Call("Player").OBJcV->setVelocity(charRotate * SP3.Call("Player").OBJcV->getVelocity());
		SP3.Call("Player").OBJcV->setCentre(SP3.Call("Player").OBJcV->getCentre() + SP3.Call("Player").OBJcV->getVelocity());
		Here.MainFace[3] = true;
		Here.MoveKeysPressed++;
	}

	if (Application::IsKeyPressed(VK_RIGHT))
		SP3.Call("Player").OBJcV->setFace(SP3.Call("Player").OBJcV->getFace() - 5.f);// TEST TEST TEST
	else if (Application::IsKeyPressed(VK_LEFT))
		SP3.Call("Player").OBJcV->setFace(SP3.Call("Player").OBJcV->getFace() + 5.f);

	if (Application::IsKeyPressed('F'))
		SP3.Call("Player").OBJcV->Jump(0.1f);
	
	if (Application::IsKeyPressed('E'))
	{
		Here.AllowPickUp = true;
		Here.AllowForce = true;
		Here.AllowActivate = true;
	}

	Floor3Timer -= 0.5f;
	if (Floor3Timer <= Limiter)
	{
		speed += 0.05;
		Limiter -= 150;
		SP3.Call("ProfessorX").OBJcV->setCOORD(randomX,0,randomZ);
	}

	ShootTo("Bullet",SP3.Call("Player").OBJcV->getCentre(),SP3.Call("Totem").OBJcV->getCentre(),130,speed);
	ShootTo2("Bullet2",SP3.Call("Player").OBJcV->getCentre(),SP3.Call("Totem2").OBJcV->getCentre(),150,speed);
	ShootTo3("Bullet3",SP3.Call("Player").OBJcV->getCentre(),SP3.Call("Totem3").OBJcV->getCentre(),120,speed);
	ShootTo4("Bullet4",SP3.Call("Player").OBJcV->getCentre(),SP3.Call("Totem4").OBJcV->getCentre(),170,speed);

	Task3Complete();



	std::ostringstream ss;
	ss << Floor3Timer;
	MyTimer = ss.str();

	SP3.CheckCollision();
	SP3.Gravity();
	UpdateCrosshair();

	//SP3.Call("Bowser").OBJcV->Chase(SP3.Call("Player").OBJcV, 0.1, true);

	if (SP3.Call("Player").OBJcV->getFace() > 180)
		SP3.Call("Player").OBJcV->setFace(SP3.Call("Player").OBJcV->getFace() - 360);
	if (SP3.Call("Player").OBJcV->getFace() < -180)
		SP3.Call("Player").OBJcV->setFace(SP3.Call("Player").OBJcV->getFace() + 360);
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
	modelStack.Scale(1000, 1000, 1000);
	RenderSkybox();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP3.Call("Player").OBJcV->getCOORD(0),
		SP3.Call("Player").OBJcV->getCOORD(1),
		SP3.Call("Player").OBJcV->getCOORD(2));
	modelStack.Rotate(SP3.Call("Player").OBJcV->getFace() + 90, 0, 1, 0);
	RenderMesh(SP3.Call("Player").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP3.Call("ThirdLevel").OBJcV->getCOORD(0),
		SP3.Call("ThirdLevel").OBJcV->getCOORD(1),
		SP3.Call("ThirdLevel").OBJcV->getCOORD(2));
	RenderMesh(SP3.Call("ThirdLevel").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP3.Call("Crosshair").OBJcV->getCOORD(0),
		SP3.Call("Crosshair").OBJcV->getCOORD(1),
		SP3.Call("Crosshair").OBJcV->getCOORD(2));
	RenderMesh(SP3.Call("Crosshair").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP3.Call("Teleporter").OBJcV->getCOORD(0),
		SP3.Call("Teleporter").OBJcV->getCOORD(1),
		SP3.Call("Teleporter").OBJcV->getCOORD(2));
	RenderMesh(SP3.Call("Teleporter").OBJmesh, false);
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
	if (SP3.Call("ProfessorX").OBJcV->getActivate() == false)
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
	modelStack.Translate(SP3.Call("ProfessorX").OBJcV->getCOORD(0),
		SP3.Call("ProfessorX").OBJcV->getCOORD(1),
		SP3.Call("ProfessorX").OBJcV->getCOORD(2));
	RenderMesh(SP3.Call("ProfessorX").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP3.Call("Totem").OBJcV->getCOORD(0),
		SP3.Call("Totem").OBJcV->getCOORD(1),
		SP3.Call("Totem").OBJcV->getCOORD(2));
	RenderMesh(SP3.Call("Totem").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP3.Call("Bullet").OBJcV->getCOORD(0),
		SP3.Call("Bullet").OBJcV->getCOORD(1),
		SP3.Call("Bullet").OBJcV->getCOORD(2));
	RenderMesh(SP3.Call("Bullet").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP3.Call("Totem2").OBJcV->getCOORD(0),
		SP3.Call("Totem2").OBJcV->getCOORD(1),
		SP3.Call("Totem2").OBJcV->getCOORD(2));
	modelStack.Rotate(-135,0,1,0);
	RenderMesh(SP3.Call("Totem2").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP3.Call("Bullet2").OBJcV->getCOORD(0),
		SP3.Call("Bullet2").OBJcV->getCOORD(1),
		SP3.Call("Bullet2").OBJcV->getCOORD(2));
	RenderMesh(SP3.Call("Bullet2").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP3.Call("Totem3").OBJcV->getCOORD(0),
		SP3.Call("Totem3").OBJcV->getCOORD(1),
		SP3.Call("Totem3").OBJcV->getCOORD(2));
	modelStack.Rotate(-45,0,1,0);
	RenderMesh(SP3.Call("Totem3").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP3.Call("Bullet3").OBJcV->getCOORD(0),
		SP3.Call("Bullet3").OBJcV->getCOORD(1),
		SP3.Call("Bullet3").OBJcV->getCOORD(2));
	RenderMesh(SP3.Call("Bullet3").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP3.Call("Totem4").OBJcV->getCOORD(0),
		SP3.Call("Totem4").OBJcV->getCOORD(1),
		SP3.Call("Totem4").OBJcV->getCOORD(2));
	modelStack.Rotate(135,0,1,0);
	RenderMesh(SP3.Call("Totem4").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP3.Call("Bullet4").OBJcV->getCOORD(0),
		SP3.Call("Bullet4").OBJcV->getCOORD(1),
		SP3.Call("Bullet4").OBJcV->getCOORD(2));
	RenderMesh(SP3.Call("Bullet4").OBJmesh, false);
	modelStack.PopMatrix();
}

void Floor3::ShootTo(std::string Target, Vector3 Destination, Vector3 Source, float MaxDistance, float BulletSpeed)
{
	SP3.Call(Target).OBJcV->setVelocity(Destination - Source);//Vector from Totem to Player
	Vector3 Temp = SP3.Call(Target).OBJcV->getVelocity().Normalize();   //Direction from Totem to Player
	Temp = Temp * BulletSpeed;
	SP3.Call(Target).OBJcV->setCentre(SP3.Call(Target).OBJcV->getCentre() + Temp);
	float Distance = (Source - SP3.Call(Target).OBJcV->getCentre()).Length();
	if( Distance >= MaxDistance)
	{
		SP3.Call(Target).OBJcV->setCentre(Source);
	}
}

void Floor3::ShootTo2(std::string Target, Vector3 Destination, Vector3 Source, float MaxDistance, float BulletSpeed)
{
	SP3.Call(Target).OBJcV->setVelocity(Destination - Source);//Vector from Totem to Player
	Vector3 Temp = SP3.Call(Target).OBJcV->getVelocity().Normalize();   //Direction from Totem to Player
	Temp = Temp * BulletSpeed;
	SP3.Call(Target).OBJcV->setCentre(SP3.Call(Target).OBJcV->getCentre() + Temp);
	float Distance = (Source - SP3.Call(Target).OBJcV->getCentre()).Length();
	if( Distance >= MaxDistance)
	{
		SP3.Call(Target).OBJcV->setCentre(Source);
	}
}

void Floor3::ShootTo3(std::string Target, Vector3 Destination, Vector3 Source, float MaxDistance, float BulletSpeed)
{
	SP3.Call(Target).OBJcV->setVelocity(Destination - Source);//Vector from Totem to Player
	Vector3 Temp = SP3.Call(Target).OBJcV->getVelocity().Normalize();   //Direction from Totem to Player
	Temp = Temp * BulletSpeed;
	SP3.Call(Target).OBJcV->setCentre(SP3.Call(Target).OBJcV->getCentre() + Temp);
	float Distance = (Source - SP3.Call(Target).OBJcV->getCentre()).Length();
	if( Distance >= MaxDistance)
	{
		SP3.Call(Target).OBJcV->setCentre(Source);
	}
}

void Floor3::ShootTo4(std::string Target, Vector3 Destination, Vector3 Source, float MaxDistance, float BulletSpeed)
{
	SP3.Call(Target).OBJcV->setVelocity(Destination - Source);//Vector from Totem to Player
	Vector3 Temp = SP3.Call(Target).OBJcV->getVelocity().Normalize();   //Direction from Totem to Player
	Temp = Temp * BulletSpeed;
	SP3.Call(Target).OBJcV->setCentre(SP3.Call(Target).OBJcV->getCentre() + Temp);
	float Distance = (Source - SP3.Call(Target).OBJcV->getCentre()).Length();
	if( Distance >= MaxDistance)
	{
		SP3.Call(Target).OBJcV->setCentre(Source);
	}
}

void Floor3::Task3Complete()
{
	if (SP3.Call("ProfessorX").OBJcV->getActivate())
	{
		SP3.Call("Teleporter").OBJcV->setCOORD(0,0,0);
	}
}

