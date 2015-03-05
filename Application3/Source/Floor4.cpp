#include "Floor4.h"
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
ISoundEngine* engine1 = createIrrKlangDevice(ESOD_AUTO_DETECT, ESEO_MULTI_THREADED | ESEO_LOAD_PLUGINS | ESEO_USE_3D_BUFFERS );
vec3df Pos1(0,0,0);

int  Floor4::sound()
{ 
	engine1->setDefault3DSoundMaxDistance(100.0f);
	engine1->setDefault3DSoundMinDistance(0.0f);
	engine1->setSoundVolume(1.0f);
	if(!engine1)
	{ 
		return 0;
	}
	vec3df position(0,0,0);

		/*if(sound)
		{
			sound->setPosition(pos3d);
		}*/

		//int playPos = sound ? sound->getPlayPosition() : 0;



		ISound* music = engine1->play3D("../Sounds/ending.mp3", Pos1,false,false,true);
		/*if (sound)
		{
			sound->setPosition(position);
		}*/
	if(music)
	{
		music->setMinDistance(0.f);
		music->setMaxDistance(1000.f);
	}
		//sound->setIsPaused(false);
		//test = false;
	

}

Floor4::Floor4()
{
}

Floor4 ::~Floor4()
{
}

void Floor4::Init()
{
	// Init VBO here

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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

	isFixed = false;

	LSPEED = 20.f;
	JetPackActivated = true;
	MovementSpeed = 10;
	rise = 0.f;
	test = true;
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
	lights[0].power = 3;
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




	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//ExportedFont.tga");



	
	Object Blossom;
	Blossom.Name = "Blossom";
	Blossom.OBJcV = new collisionSphere(2.f, Vector3(10, 0 + rise, 0));
	Blossom.OBJmesh = MeshBuilder::GenerateOBJ("Blossom", "OBJ//PuffBod.obj");
	Blossom.OBJmesh->textureID = LoadTGA("Image//Blossom.tga");
	Blossom.OBJmesh->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	Blossom.OBJmesh->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	Blossom.OBJmesh->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	Blossom.OBJmesh->material.kShininess = 5.f;
	SP4.Add(Blossom);

	Object Bubbles;
	Bubbles.Name = "Bubbles";
	Bubbles.CollisionTrigger = true;
	Bubbles.OBJcV = new collisionSphere(2.f, Vector3(10, 0 + rise, -5));
	Bubbles.OBJmesh = MeshBuilder::GenerateOBJ("Bubbles", "OBJ//PuffBod.obj");
	Bubbles.OBJmesh->textureID = LoadTGA("Image//Bubbles.tga");
	Bubbles.OBJmesh->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	Bubbles.OBJmesh->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	Bubbles.OBJmesh->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	Bubbles.OBJmesh->material.kShininess = 5.f;
	SP4.Add(Bubbles);

	Object Buttercup;
	Buttercup.Name = "Buttercup";
	Buttercup.CollisionTrigger = true;
	Buttercup.OBJcV = new collisionSphere(2.f, Vector3(10, 0 + rise, 5));
	Buttercup.OBJmesh = MeshBuilder::GenerateOBJ("Buttercup", "OBJ//PuffBod.obj");
	Buttercup.OBJmesh->textureID = LoadTGA("Image//Buttercup.tga");
	Buttercup.OBJmesh->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	Buttercup.OBJmesh->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	Buttercup.OBJmesh->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	Buttercup.OBJmesh->material.kShininess = 5.f;
	SP4.Add(Buttercup);

	Object Platform;
	Platform.Name = "Platform";
	Platform.CollisionTrigger = true;
	Platform.OBJcV = new collisionSphere(2.f, Vector3(10, 0 + rise, 0));
	Platform.OBJmesh = MeshBuilder::GenerateOBJ("Platform", "OBJ//Platform.obj");
	Platform.OBJmesh->textureID = LoadTGA("Image//Prison.tga");
	Platform.OBJmesh->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	Platform.OBJmesh->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	Platform.OBJmesh->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	Platform.OBJmesh->material.kShininess = 5.f;
	SP4.Add(Platform);

	Object Prison;
	Prison.Name = "Prison";
	Prison.CollisionTrigger = true;
	Prison.OBJcV = new collisionSphere(2.f, Vector3(10, 0, 0));
	Prison.OBJmesh = MeshBuilder::GenerateOBJ("Prison", "OBJ//Prison.obj");
	Prison.OBJmesh->textureID = LoadTGA("Image//Prison.tga");
	Prison.OBJmesh->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	Prison.OBJmesh->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	Prison.OBJmesh->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	Prison.OBJmesh->material.kShininess = 5.f;
	SP4.Add(Prison);

	Object Sugar;
	Sugar.Name = "Sugar";
	Sugar.CollisionTrigger = true;
	Sugar.OBJcV = new collisionSphere(2.f, Vector3(-200, 5.5, 0));
	Sugar.OBJmesh = MeshBuilder::GenerateOBJ("Sugar", "OBJ//Sugar.obj");
	Sugar.OBJmesh->textureID = LoadTGA("Image//Sugar.tga");
	Sugar.OBJmesh->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	Sugar.OBJmesh->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	Sugar.OBJmesh->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	Sugar.OBJmesh->material.kShininess = 5.f;
	SP4.Add(Sugar);

	Object Spice;
	Spice.Name = "Spice";
	Spice.CollisionTrigger = true;
	Spice.OBJcV = new collisionSphere(2.f, Vector3(-150, 5.5, 0));
	Spice.OBJmesh = MeshBuilder::GenerateOBJ("Spice", "OBJ//Spice.obj");
	Spice.OBJmesh->textureID = LoadTGA("Image//Spice.tga");
	Spice.OBJmesh->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	Spice.OBJmesh->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	Spice.OBJmesh->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	Spice.OBJmesh->material.kShininess = 5.f;
	SP4.Add(Spice);

	Object Nice;
	Nice.Name = "Nice";
	Nice.CollisionTrigger = true;
	Nice.OBJcV = new collisionSphere(2.f, Vector3(-100, 6, 0));
	Nice.OBJmesh = MeshBuilder::GenerateOBJ("Nice", "OBJ//NICE.obj");
	Nice.OBJmesh->textureID = LoadTGA("Image//Nice.tga");
	Nice.OBJmesh->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	Nice.OBJmesh->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	Nice.OBJmesh->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	Nice.OBJmesh->material.kShininess = 5.f;
	SP4.Add(Nice);


	Object ChemicalX;
	ChemicalX.Name = "ChemicalX";
	ChemicalX.Gravity = false;
	ChemicalX.OBJcV = new collisionSphere(1.f, Vector3(-50, 5.5, -2));
	ChemicalX.OBJcV->setEffect(0);
	ChemicalX.OBJmesh = MeshBuilder::GenerateOBJ("ChemicalX", "OBJ//ChemicalX.obj");
	ChemicalX.OBJmesh->textureID = LoadTGA("Image//Flask.tga");
	ChemicalX.OBJmesh->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	ChemicalX.OBJmesh->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	ChemicalX.OBJmesh->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	ChemicalX.OBJmesh->material.kShininess = 5.f;
	SP4.Add(ChemicalX);
	
	
	//Initialize camera settings
	camera.Init(Vector3(-270, 6, 0), Vector3(-269, 6, 0), Vector3(0, 1, 0));

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 5000.f);
	projectionStack.LoadMatrix(projection);

	//remove all glGenBuffers, glBindBuffer, glBufferData code
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

double Floor4::DtCopy = 0;

void Floor4::Update(double dt)
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

	//if (Application::IsKeyPressed('I'))// TEST TEST TEST
	//	lights[0].position.z -= (float)(LSPEED * dt);
	//if (Application::IsKeyPressed('K'))
	//	lights[0].position.z += (float)(LSPEED * dt);
	//if (Application::IsKeyPressed('J'))
	//	lights[0].position.x -= (float)(LSPEED * dt);
	//if (Application::IsKeyPressed('L'))
	//	lights[0].position.x += (float)(LSPEED * dt);
	//if (Application::IsKeyPressed('O'))
	//	lights[0].position.y -= (float)(LSPEED * dt);
	//if (Application::IsKeyPressed('P'))
	//	lights[0].position.y += (float)(LSPEED * dt);


	lights[0].position.x = camera.position.x - 2;
	lights[0].position.y = camera.position.y + 4;
	lights[0].position.z = camera.position.z;
	

	if(camera.position.x <= -10)
	{
		camera.position.x += 15 * dt;
		camera.target.x += 15 * dt;
	}

	if(camera.position.x >= -11)
	{
		if (rise <= 3)
		{
			rise += 1 * dt;
		}
	}


	float multiplier = 0.001f;
	Vector3 view = 0.f;
	view = (camera.target + camera.position);
	//engine->setListenerPosition(vec3df(multiplier * camera.position.x,multiplier * camera.position.y,multiplier * camera.position.z), vec3df(0,0,0), vec3df(0,0,0), Pos);
	engine1->setListenerPosition(vec3df(multiplier * view.x,multiplier * view.y,multiplier * view.z), Pos1);
	engine1->update();
	camera.Update(dt);
}

void Floor4::Render()
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

	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(SP4.Call("Blossom").OBJcV->getCOORD(0),
		SP4.Call("Blossom").OBJcV->getCOORD(1),
		SP4.Call("Blossom").OBJcV->getCOORD(2));
	modelStack.Translate(0,rise,0);
	modelStack.Rotate(SP4.Call("Blossom").OBJcV->getFace() - 90, 0, 1, 0);
	RenderMesh(SP4.Call("Blossom").OBJmesh, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP4.Call("Bubbles").OBJcV->getCOORD(0),
		SP4.Call("Bubbles").OBJcV->getCOORD(1),
		SP4.Call("Bubbles").OBJcV->getCOORD(2));
	modelStack.Translate(0,rise,0);
	modelStack.Rotate(SP4.Call("Bubbles").OBJcV->getFace() - 90, 0, 1, 0);
	RenderMesh(SP4.Call("Bubbles").OBJmesh, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP4.Call("Buttercup").OBJcV->getCOORD(0),
		SP4.Call("Buttercup").OBJcV->getCOORD(1),
		SP4.Call("Buttercup").OBJcV->getCOORD(2));
	modelStack.Translate(0,rise,0);
	modelStack.Rotate(SP4.Call("Buttercup").OBJcV->getFace() - 90, 0, 1, 0);
	RenderMesh(SP4.Call("Buttercup").OBJmesh, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP4.Call("Platform").OBJcV->getCOORD(0),
		SP4.Call("Platform").OBJcV->getCOORD(1),
		SP4.Call("Platform").OBJcV->getCOORD(2));
	modelStack.Translate(0,rise,0);
	RenderMesh(SP4.Call("Platform").OBJmesh, true);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(SP4.Call("Prison").OBJcV->getCOORD(0),
		SP4.Call("Prison").OBJcV->getCOORD(1),
		SP4.Call("Prison").OBJcV->getCOORD(2));
	RenderMesh(SP4.Call("Prison").OBJmesh, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP4.Call("Sugar").OBJcV->getCOORD(0),
		SP4.Call("Sugar").OBJcV->getCOORD(1),
		SP4.Call("Sugar").OBJcV->getCOORD(2));
	modelStack.Rotate(SP4.Call("Sugar").OBJcV->getFace() - 90, 0, 1, 0);
	RenderMesh(SP4.Call("Sugar").OBJmesh, true);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(SP4.Call("Spice").OBJcV->getCOORD(0),
		SP4.Call("Spice").OBJcV->getCOORD(1),
		SP4.Call("Spice").OBJcV->getCOORD(2));
	modelStack.Rotate(SP4.Call("Spice").OBJcV->getFace() - 90, 0, 1, 0);
	RenderMesh(SP4.Call("Spice").OBJmesh, true);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(SP4.Call("Nice").OBJcV->getCOORD(0),
		SP4.Call("Nice").OBJcV->getCOORD(1),
		SP4.Call("Nice").OBJcV->getCOORD(2));
	modelStack.Rotate(SP4.Call("Nice").OBJcV->getFace() - 90, 0, 1, 0);
	RenderMesh(SP4.Call("Nice").OBJmesh, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP4.Call("ChemicalX").OBJcV->getCOORD(0),
		SP4.Call("ChemicalX").OBJcV->getCOORD(1),
		SP4.Call("ChemicalX").OBJcV->getCOORD(2));
	modelStack.Rotate(SP4.Call("ChemicalX").OBJcV->getFace() - 90, 0, 1, 0);
	RenderMesh(SP4.Call("ChemicalX").OBJmesh, true);
	modelStack.PopMatrix();

	if(test == true)
	{
	sound();
	test = false;
	}
}

void Floor4::Exit()
{
	engine1->drop();
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

}
