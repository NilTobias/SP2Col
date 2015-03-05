#include "Floor2.h"
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

ISoundEngine* engine = createIrrKlangDevice(ESOD_AUTO_DETECT, ESEO_MULTI_THREADED | ESEO_LOAD_PLUGINS | ESEO_USE_3D_BUFFERS );
vec3df Pos(0,0,0);

int  Floor2::sound()
{ 
	engine->setDefault3DSoundMaxDistance(100.0f);
	engine->setDefault3DSoundMinDistance(0.0f);
	engine->setSoundVolume(1.0f);
	if(!engine)
	{ 
		return 0;
	}
	vec3df position(0,0,0);

		/*if(sound)
		{
			sound->setPosition(pos3d);
		}*/

		//int playPos = sound ? sound->getPlayPosition() : 0;



		ISound* music = engine->play3D("../Sounds/test.mp3", Pos,true,false,true);
				
		ISound* sound = engine->play3D("../Sounds/wind.ogg", Pos,true,false,true);
		/*if (sound)
		{
			sound->setPosition(position);
		}*/
	if(sound)
	{
		sound->setMinDistance(0.f);
		sound->setMaxDistance(1000.f);
		music->setMinDistance(0.f);
		music->setMaxDistance(1000.f);
	}
		//sound->setIsPaused(false);
		//test = false;
	

}

Floor2::Floor2()
{
}

Floor2 ::~Floor2()
{
}

void Floor2::Init()
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

	SpeedUp = 0;
	Limiter = 1900;
	Floor2Timer = 2000;
	FreezeTimer = 0;
	isFixed = false;
	LSPEED = 20.f;
	MovementSpeed = 10;
	test = false;
	TaskList[0] = false;
	TaskList[1] = false;
	TaskList[2] = false;


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

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);


	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);

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

	meshList[FLOOR2] = MeshBuilder::GenerateOBJ("SecondFloor", "OBJ//storey2.obj");
	meshList[FLOOR2]->textureID = LoadTGA("Image//storey2.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//ExportedFont.tga");


	/*
	Object Floor2;
	Floor2.Name = "SecondLevel";//IMPORTANT
	Floor2.ReverseCollision = true;
	Floor2.OBJcV = new AABB(80.f, 75.f, 100.f, Vector3(0, 0, 0)); //IMPORTANT
	Floor2.OBJcV->setEffect(1); //IMPORTANT
	Floor2.OBJcV->setVelocity(Vector3(0, 5, 0));
	SP.Add(Floor2);
	*/
	/**********************************************************************************/
	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Packet;
			Packet.Name = "Packet1" + AddToName;
			Packet.CollisionTrigger = false;
			Packet.OBJcV = new collisionSphere(0.1f , Vector3(13.2, 4.2, -2.7+i*2));
			Packet.OBJcV->setEffect(0);
			Packet.OBJmesh = MeshBuilder::GenerateOBJ("Packet1", "OBJ//Packet.obj");
			Packet.OBJmesh->textureID = LoadTGA("Image//Packet.tga");
			SP.Add(Packet);


	}

	for(int j = 0; j < 5; j++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << j;
			AddToName = ss.str();
			Object Packet;
			Packet.Name = "Packet2" + AddToName;
			Packet.CollisionTrigger = false;
			Packet.OBJcV = new collisionSphere(0.1f , Vector3(14.2, 4.2, -2.7+j*2));
			Packet.OBJcV->setEffect(0);
			Packet.OBJmesh = MeshBuilder::GenerateOBJ("Packet2", "OBJ//Packet.obj");
			Packet.OBJmesh->textureID = LoadTGA("Image//Packet.tga");
			SP.Add(Packet);


	}

	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Packet;
			Packet.Name = "Packet4" + AddToName;
			Packet.CollisionTrigger = false;
			Packet.OBJcV = new collisionSphere(0.1f , Vector3(13.2, 2.6, -2.7+i*2));
			Packet.OBJcV->setEffect(0);
			Packet.OBJmesh = MeshBuilder::GenerateOBJ("Packet4", "OBJ//Packet.obj");
			Packet.OBJmesh->textureID = LoadTGA("Image//Packet.tga");
			SP.Add(Packet);


	}

	for(int k = 0; k < 5; k++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << k;
			AddToName = ss.str();
			Object Packet;
			Packet.Name = "Packet3" + AddToName;
			Packet.CollisionTrigger = false;
			Packet.OBJcV = new collisionSphere(0.1f , Vector3(14.2, 2.6, -2.7+k*2));
			Packet.OBJcV->setEffect(0);
			Packet.OBJmesh = MeshBuilder::GenerateOBJ("Packet3", "OBJ//Packet.obj");
			Packet.OBJmesh->textureID = LoadTGA("Image//Packet.tga");
			SP.Add(Packet);


	}
	/**********************************************************************************/

	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Sugar;
			Sugar.Name = "Sugar1" + AddToName;
			Sugar.CollisionTrigger = false;
			Sugar.OBJcV = new collisionSphere(0.1f , Vector3(13.2, 4.2, -19+(i*2)));
			Sugar.OBJcV->setEffect(0);
			Sugar.OBJmesh = MeshBuilder::GenerateOBJ("Sugar1", "OBJ//Sugar.obj");
			Sugar.OBJmesh->textureID = LoadTGA("Image//Sugar.tga");
			SP.Add(Sugar);
	}

	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Sugar;
			Sugar.Name = "Sugar2" + AddToName;
			Sugar.CollisionTrigger = false;
			Sugar.OBJcV = new collisionSphere(0.1f , Vector3(13.2, 2, -19+(i*2)));
			Sugar.OBJcV->setEffect(0);
			Sugar.OBJmesh = MeshBuilder::GenerateOBJ("Sugar2", "OBJ//Sugar.obj");
			Sugar.OBJmesh->textureID = LoadTGA("Image//Sugar.tga");
			SP.Add(Sugar);
	}
	/**********************************************************************************/

	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box;
			Box.Name = "Box1" + AddToName;
			Box.CollisionTrigger = false;
			Box.OBJcV = new collisionSphere(0.1f , Vector3(21, 3.95, -19.8+i*2));
			Box.OBJcV->setEffect(0);
			Box.OBJmesh = MeshBuilder::GenerateOBJ("Box1", "OBJ//Box.obj");
			Box.OBJmesh->textureID = LoadTGA("Image//Box.tga");
			SP.Add(Box);
	}

	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box;
			Box.Name = "Box2" + AddToName;
			Box.CollisionTrigger = false;
			Box.OBJcV = new collisionSphere(0.1f , Vector3(22, 3.95, -19.8+i*2));
			Box.OBJcV->setEffect(0);
			Box.OBJmesh = MeshBuilder::GenerateOBJ("Box2", "OBJ//Box.obj");
			Box.OBJmesh->textureID = LoadTGA("Image//Box.tga");
			SP.Add(Box);
	}

	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box;
			Box.Name = "Box3" + AddToName;
			Box.CollisionTrigger = false;
			Box.OBJcV = new collisionSphere(0.1f , Vector3(21, 2.27, -19.8+i*2));
			Box.OBJcV->setEffect(0);
			Box.OBJmesh = MeshBuilder::GenerateOBJ("Box3", "OBJ//Box.obj");
			Box.OBJmesh->textureID = LoadTGA("Image//Box.tga");
			SP.Add(Box);
	}

	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box;
			Box.Name = "Box4" + AddToName;
			Box.CollisionTrigger = false;
			Box.OBJcV = new collisionSphere(0.1f , Vector3(22, 2.27, -19.8+i*2));
			Box.OBJcV->setEffect(0);
			Box.OBJmesh = MeshBuilder::GenerateOBJ("Box4", "OBJ//Box.obj");
			Box.OBJmesh->textureID = LoadTGA("Image//Box.tga");
			SP.Add(Box);
	}
	/**********************************************************************************/
	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Spice;
			Spice.Name = "Spice1" + AddToName;
			Spice.CollisionTrigger = false;
			Spice.OBJcV = new collisionSphere(0.1f , Vector3(22, 3.95, -2.3+i * 2));
			Spice.OBJcV->setEffect(0);
			Spice.OBJmesh = MeshBuilder::GenerateOBJ("Spice1", "OBJ//cat.obj");
			Spice.OBJmesh->textureID = LoadTGA("Image//cat.tga");
			SP.Add(Spice);
	}
	/**********************************************************************************/
	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box2;
			Box2.Name = "Box21" + AddToName;
			Box2.CollisionTrigger = false;
			Box2.OBJcV = new collisionSphere(0.1f , Vector3(21, 3.95, 12.7+i*2));
			Box2.OBJcV->setEffect(0);
			Box2.OBJmesh = MeshBuilder::GenerateOBJ("Box21", "OBJ//Box2.obj");
			Box2.OBJmesh->textureID = LoadTGA("Image//Box2.tga");
			SP.Add(Box2);
	}

	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box2;
			Box2.Name = "Box22" + AddToName;
			Box2.CollisionTrigger = false;
			Box2.OBJcV = new collisionSphere(0.1f , Vector3(21.5, 3.95, 12.7+i*2));
			Box2.OBJcV->setEffect(0);
			Box2.OBJmesh = MeshBuilder::GenerateOBJ("Box22", "OBJ//Box2.obj");
			Box2.OBJmesh->textureID = LoadTGA("Image//Box2.tga");
			SP.Add(Box2);
	}

	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box2;
			Box2.Name = "Box23" + AddToName;
			Box2.CollisionTrigger = false;
			Box2.OBJcV = new collisionSphere(0.1f , Vector3(22, 3.95, 12.7+i*2));
			Box2.OBJcV->setEffect(0);
			Box2.OBJmesh = MeshBuilder::GenerateOBJ("Box23", "OBJ//Box2.obj");
			Box2.OBJmesh->textureID = LoadTGA("Image//Box2.tga");
			SP.Add(Box2);
	}

	for(int i = 0; i < 2; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box2;
			Box2.Name = "Box24" + AddToName;
			Box2.CollisionTrigger = false;
			Box2.OBJcV = new collisionSphere(0.1f , Vector3(22.5, 3.95, 12.7+i*2));
			Box2.OBJcV->setEffect(0);
			Box2.OBJmesh = MeshBuilder::GenerateOBJ("Box24", "OBJ//Box2.obj");
			Box2.OBJmesh->textureID = LoadTGA("Image//Box2.tga");
			SP.Add(Box2);
	}

	for(int i = 0; i < 2; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box2;
			Box2.Name = "Box25" + AddToName;
			Box2.CollisionTrigger = false;
			Box2.OBJcV = new collisionSphere(0.1f , Vector3(22.5, 3.95, 18.7+i*2));
			Box2.OBJcV->setEffect(0);
			Box2.OBJmesh = MeshBuilder::GenerateOBJ("Box25", "OBJ//Box2.obj");
			Box2.OBJmesh->textureID = LoadTGA("Image//Box2.tga");
			SP.Add(Box2);
	}

			Object Box2;
			Box2.Name = "Spice2";
			Box2.CollisionTrigger = false;
			Box2.OBJcV = new collisionSphere(0.1f , Vector3(22.5, 3.95, 16.7));
			Box2.OBJcV->setEffect(0);
			Box2.OBJmesh = MeshBuilder::GenerateOBJ("Spice2", "OBJ//Spice.obj");
			Box2.OBJmesh->textureID = LoadTGA("Image//Spice.tga");
			SP.Add(Box2);

	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box2;
			Box2.Name = "Box26" + AddToName;
			Box2.CollisionTrigger = false;
			Box2.OBJcV = new collisionSphere(0.1f , Vector3(21, 2.3, 12.7+i*2));
			Box2.OBJcV->setEffect(0);
			Box2.OBJmesh = MeshBuilder::GenerateOBJ("Box26", "OBJ//Box2.obj");
			Box2.OBJmesh->textureID = LoadTGA("Image//Box2.tga");
			SP.Add(Box2);
	}

	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box2;
			Box2.Name = "Box27" + AddToName;
			Box2.CollisionTrigger = false;
			Box2.OBJcV = new collisionSphere(0.1f , Vector3(21.5, 2.3, 12.7+i*2));
			Box2.OBJcV->setEffect(0);
			Box2.OBJmesh = MeshBuilder::GenerateOBJ("Box27", "OBJ//Box2.obj");
			Box2.OBJmesh->textureID = LoadTGA("Image//Box2.tga");
			SP.Add(Box2);
	}

	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box2;
			Box2.Name = "Box28" + AddToName;
			Box2.CollisionTrigger = false;
			Box2.OBJcV = new collisionSphere(0.1f , Vector3(22, 2.3, 12.7+i*2));
			Box2.OBJcV->setEffect(0);
			Box2.OBJmesh = MeshBuilder::GenerateOBJ("Box28", "OBJ//Box2.obj");
			Box2.OBJmesh->textureID = LoadTGA("Image//Box2.tga");
			SP.Add(Box2);
	}

	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box2;
			Box2.Name = "Box29" + AddToName;
			Box2.CollisionTrigger = false;
			Box2.OBJcV = new collisionSphere(0.1f , Vector3(22.5, 2.3, 12.7+i*2));
			Box2.OBJcV->setEffect(0);
			Box2.OBJmesh = MeshBuilder::GenerateOBJ("Box29", "OBJ//Box2.obj");
			Box2.OBJmesh->textureID = LoadTGA("Image//Box2.tga");
			SP.Add(Box2);
	}
	/**********************************************************************************/
			Object Nice;
			Nice.Name = "Nice";
			Nice.CollisionTrigger = false;
			Nice.OBJcV = new collisionSphere(2.f, Vector3(3, 5, 25));
			Nice.OBJcV->setEffect(0,2);
			Nice.OBJmesh = MeshBuilder::GenerateOBJ("Nice", "OBJ//NICE.obj");
			Nice.OBJmesh->textureID = LoadTGA("Image//NICE.tga");
			SP.Add(Nice);
	/**********************************************************************************/
	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box3;
			Box3.Name = "Box31" + AddToName;
			Box3.CollisionTrigger = false;
			Box3.OBJcV = new collisionSphere(0.1f , Vector3(13.2, 3.95, 12.8+i*2));
			Box3.OBJcV->setEffect(0);
			Box3.OBJmesh = MeshBuilder::GenerateOBJ("Box31", "OBJ//Box3.obj");
			Box3.OBJmesh->textureID = LoadTGA("Image//Box3.tga");
			SP.Add(Box3);
	}

	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box3;
			Box3.Name = "Box32" + AddToName;
			Box3.CollisionTrigger = false;
			Box3.OBJcV = new collisionSphere(0.1f , Vector3(14.2, 3.95, 12.8+i*2));
			Box3.OBJcV->setEffect(0);
			Box3.OBJmesh = MeshBuilder::GenerateOBJ("Box32", "OBJ//Box3.obj");
			Box3.OBJmesh->textureID = LoadTGA("Image//Box3.tga");
			SP.Add(Box3);
	}

	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box3;
			Box3.Name = "Box33" + AddToName;
			Box3.CollisionTrigger = false;
			Box3.OBJcV = new collisionSphere(0.1f , Vector3(13.2, 2.3, 12.8+i*2));
			Box3.OBJcV->setEffect(0);
			Box3.OBJmesh = MeshBuilder::GenerateOBJ("Box33", "OBJ//Box3.obj");
			Box3.OBJmesh->textureID = LoadTGA("Image//Box3.tga");
			SP.Add(Box3);
	}

	for(int i = 0; i < 5; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box3;
			Box3.Name = "Box34" + AddToName;
			Box3.CollisionTrigger = false;
			Box3.OBJcV = new collisionSphere(0.1f , Vector3(14.2, 2.3, 12.8+i*2));
			Box3.OBJcV->setEffect(0);
			Box3.OBJmesh = MeshBuilder::GenerateOBJ("Box34", "OBJ//Box3.obj");
			Box3.OBJmesh->textureID = LoadTGA("Image//Box3.tga");
			SP.Add(Box3);
	}
	/**********************************************************************************/
	for(int i = 0; i < 12; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box4;
			Box4.Name = "Box41" + AddToName;
			Box4.CollisionTrigger = false;
			Box4.OBJcV = new collisionSphere(0.1f , Vector3(-28+i*2, 4, 18));
			Box4.OBJcV->setEffect(0);
			Box4.OBJmesh = MeshBuilder::GenerateOBJ("Box41", "OBJ//Box4.obj");
			Box4.OBJmesh->textureID = LoadTGA("Image//Box4.tga");
			SP.Add(Box4);
	}

	for(int i = 0; i < 12; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box4;
			Box4.Name = "Box42" + AddToName;
			Box4.CollisionTrigger = false;
			Box4.OBJcV = new collisionSphere(0.1f , Vector3(-28+i*2, 4, 19));
			Box4.OBJcV->setEffect(0);
			Box4.OBJmesh = MeshBuilder::GenerateOBJ("Box42", "OBJ//Box4.obj");
			Box4.OBJmesh->textureID = LoadTGA("Image//Box4.tga");
			SP.Add(Box4);
	}
	/**********************************************************************************/
	for(int i = 0; i < 12; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box5;
			Box5.Name = "Box51" + AddToName;
			Box5.CollisionTrigger = false;
			Box5.OBJcV = new collisionSphere(0.1f , Vector3(-28+i*2, 4, -18));
			Box5.OBJcV->setEffect(0);
			Box5.OBJmesh = MeshBuilder::GenerateOBJ("Box51", "OBJ//Box5.obj");
			Box5.OBJmesh->textureID = LoadTGA("Image//Box5.tga");
			SP.Add(Box5);
	}

	for(int i = 0; i < 12; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box5;
			Box5.Name = "Box52" + AddToName;
			Box5.CollisionTrigger = false;
			Box5.OBJcV = new collisionSphere(0.1f , Vector3(-28+i*2, 4, -19));
			Box5.OBJcV->setEffect(0);
			Box5.OBJmesh = MeshBuilder::GenerateOBJ("Box52", "OBJ//Box5.obj");
			Box5.OBJmesh->textureID = LoadTGA("Image//Box5.tga");
			SP.Add(Box5);
	}

	for(int i = 0; i < 12; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box5;
			Box5.Name = "Box53" + AddToName;
			Box5.CollisionTrigger = false;
			Box5.OBJcV = new collisionSphere(0.1f , Vector3(-28+i*2, 2.3, -18));
			Box5.OBJcV->setEffect(0);
			Box5.OBJmesh = MeshBuilder::GenerateOBJ("Box53", "OBJ//Box5.obj");
			Box5.OBJmesh->textureID = LoadTGA("Image//Box5.tga");
			SP.Add(Box5);
	}

	for(int i = 0; i < 12; i++)
	{
			string AddToName;
			std::ostringstream ss;
			ss << i;
			AddToName = ss.str();
			Object Box5;
			Box5.Name = "Box54" + AddToName;
			Box5.CollisionTrigger = false;
			Box5.OBJcV = new collisionSphere(0.1f , Vector3(-28+i*2, 2.3, -19));
			Box5.OBJcV->setEffect(0);
			Box5.OBJmesh = MeshBuilder::GenerateOBJ("Box54", "OBJ//Box5.obj");
			Box5.OBJmesh->textureID = LoadTGA("Image//Box5.tga");
			SP.Add(Box5);

	}
	/**********************************************************************************/
			Object UFO;
			UFO.Name = "UFO";
			UFO.CollisionTrigger = true;
			UFO.OBJcV = new collisionSphere(0.1f , Vector3(10, 0, 0));
			UFO.OBJcV->setEffect(2);
			UFO.OBJmesh = MeshBuilder::GenerateOBJ("UFO", "OBJ//ufo.obj");
			UFO.OBJmesh->textureID = LoadTGA("Image//UFO.tga");
			UFO.OBJmesh->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
			UFO.OBJmesh->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
			UFO.OBJmesh->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
			UFO.OBJmesh->material.kShininess = 5.f;
			SP.Add(UFO);

			Object FreezeBuff;
			FreezeBuff.Name = "FreezeBuff";
			FreezeBuff.CollisionTrigger = true;
			FreezeBuff.Gravity = false;
			FreezeBuff.OBJcV = new collisionSphere(0.2f, Vector3(0, 100, 0));
			FreezeBuff.OBJcV->setEffect(2);
			FreezeBuff.OBJmesh = MeshBuilder::GenerateOBJ("FreezeBuff", "OBJ//dart.obj");
			FreezeBuff.OBJmesh->textureID = LoadTGA("Image//dart.tga");
			SP.Add(FreezeBuff);
			

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

double Floor2::DtCopy = 0;

void Floor2::Update(double dt)
{
	int randomX = rand() %  42 + -30;
	int randomZ = rand() %  34 + -17;
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

	if (Application::IsKeyPressed('E'))
	{
		Here.AllowPickUp = true;
		Here.AllowForce = true;
		Here.AllowActivate = true;
	}

	if (SP.Call("Nice").OBJcV->getActivate() == true)
	{
		TaskList[2] = true;
	}

	SP.CheckCollision();
	SP.Gravity();
	UpdateCrosshair();


	if (SP.Call("FreezeBuff").OBJcV->getActivate() == false)
		SP.Call("UFO").OBJcV->Chase(SP.Call("Player").OBJcV, 0.1f + SpeedUp, true);

	if (Floor2Timer <= Limiter)
	{
		Limiter -= 100;
		SpeedUp += 0.01f;
		SP.Call("FreezeBuff").OBJcV->setCOORD(randomX,0,randomZ);
	}

	if (Floor2Timer > 1 && SP.Call("FreezeBuff").OBJcV->getActivate() == false)
		Floor2Timer -= 1;
	else
		FreezeTimer += 0.5f;

	if (FreezeTimer >= 50)
	{
		SP.Call("FreezeBuff").OBJcV->setCOORD(0,100,0);
		SP.Call("FreezeBuff").OBJcV->setActivate(false);
		FreezeTimer = 0;
	}

	std::ostringstream ss;
	ss << Floor2Timer;
	MyTimer = ss.str();

	//if(SP.Call("UFO").OBJcV->getActivate() == true)
		//gameover
	

	if (SP.Call("Player").OBJcV->getFace() > 180)
		SP.Call("Player").OBJcV->setFace(SP.Call("Player").OBJcV->getFace() - 360);
	if (SP.Call("Player").OBJcV->getFace() < -180)
		SP.Call("Player").OBJcV->setFace(SP.Call("Player").OBJcV->getFace() + 360);

	float multiplier = 0.1f;
	Vector3 view = 0.f;
	view = (camera.target + camera.position);
	//engine->setListenerPosition(vec3df(multiplier * camera.position.x,multiplier * camera.position.y,multiplier * camera.position.z), vec3df(0,0,0), vec3df(0,0,0), Pos);
	engine->setListenerPosition(vec3df(multiplier * view.x,multiplier * view.y,multiplier * view.z), Pos);
	engine->update();
	camera.Update(dt);
}

void Floor2::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x,
		camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);

	modelStack.LoadIdentity();

	Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;// TEST TEST TEST
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);


	Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);

	RenderMesh(meshList[GEO_AXES], false);
	// TEST TEST TEST
	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Crosshair").OBJcV->getCOORD(0),
		SP.Call("Crosshair").OBJcV->getCOORD(1),
		SP.Call("Crosshair").OBJcV->getCOORD(2));
	RenderMesh(SP.Call("Crosshair").OBJmesh, false);
	modelStack.PopMatrix();

	RenderFloor2();
	RenderTaskList();

	if(test == true)
	{
	sound();
	test = false;
	}
}

void Floor2::Exit()
{
	engine->drop();
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

}

void Floor2::RenderTaskList()
{
	modelStack.PushMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], MyTimer, Color(1,1,1),10,6.5f,5.5f);
	modelStack.PopMatrix();

	if (SP.Call("FreezeBuff").OBJcV->getActivate() == true)
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Time Has Frozen!", Color(1, 1, 1), 10, 1, 5);
		modelStack.PopMatrix();
	}

	if (TaskList[0] == false)
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Find Sugar", Color(1, 1, 1), 7, 1, 0.8);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "SUGAR FOUND", Color(1, 1, 1), 7, 1, 0.8);
		modelStack.PopMatrix();
	}
	if (TaskList[1] == false)
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Find Spice", Color(1, 1, 1), 7, 1, 0.5);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "FOUND SPICE", Color(1, 1, 1), 7, 1, 0.5);
		modelStack.PopMatrix();
	}
	if (TaskList[2] == false)
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Find Everything Nice", Color(1, 1, 1), 7, 1, 0.2);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "FOUND EVERYTHING NICE", Color(1, 1, 1), 7, 1, 0.2);
		modelStack.PopMatrix();
	}
}

void Floor2::RenderFloor2()
{
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Player").OBJcV->getCOORD(0),
		SP.Call("Player").OBJcV->getCOORD(1),
		SP.Call("Player").OBJcV->getCOORD(2));
	modelStack.Rotate(SP.Call("Player").OBJcV->getFace() + 90, 0, 1, 0);
	RenderMesh(SP.Call("Player").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[FLOOR2], false);
	modelStack.PopMatrix();

	/*******************************************************************************/
	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Packet1" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Packet1" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Packet1" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Packet1" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}
	for (int j = 0; j < 5; j++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << j;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Packet2" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Packet2" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Packet2" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Packet2" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}
	for (int k = 0; k < 5; k++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << k;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Packet3" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Packet3" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Packet3" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Packet3" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}
	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Packet4" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Packet4" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Packet4" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Packet4" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}
	/*******************************************************************************/

	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Sugar1" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Sugar1" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Sugar1" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Sugar1" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}
	/*******************************************************************************/
	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box1" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box1" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box1" + AddToName).OBJcV->getCOORD(2));
		RenderMesh(SP.Call("Box1" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box2" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box2" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box2" + AddToName).OBJcV->getCOORD(2));
		RenderMesh(SP.Call("Box2" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box3" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box3" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box3" + AddToName).OBJcV->getCOORD(2));
		RenderMesh(SP.Call("Box3" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box4" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box4" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box4" + AddToName).OBJcV->getCOORD(2));
		RenderMesh(SP.Call("Box4" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	/*******************************************************************************/
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Spice2").OBJcV->getCOORD(0),
		SP.Call("Spice2").OBJcV->getCOORD(1),
		SP.Call("Spice2").OBJcV->getCOORD(2));
	RenderMesh(SP.Call("Spice2").OBJmesh, false);
	modelStack.PopMatrix();
	/*******************************************************************************/
	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Spice1" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Spice1" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Spice1" + AddToName).OBJcV->getCOORD(2));
		RenderMesh(SP.Call("Spice1" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}
	/********************************************************************************/
	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box21" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box21" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box21" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Box21" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box22" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box22" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box22" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Box22" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box23" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box23" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box23" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Box23" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 2; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box24" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box24" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box24" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Box24" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 2; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box25" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box25" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box25" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Box25" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box26" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box26" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box26" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Box26" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box27" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box27" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box27" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Box27" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box28" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box28" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box28" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Box28" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box29" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box29" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box29" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Box29" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}
	/*******************************************************************************/
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Nice").OBJcV->getCOORD(0),
		SP.Call("Nice").OBJcV->getCOORD(1),
		SP.Call("Nice").OBJcV->getCOORD(2));
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(SP.Call("Nice").OBJmesh, false);
	modelStack.PopMatrix();
	/*******************************************************************************/
	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box31" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box31" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box31" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Box31" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box32" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box32" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box32" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Box32" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box33" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box33" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box33" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Box33" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 5; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box34" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box34" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box34" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(SP.Call("Box34" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}
	/*******************************************************************************/
	for (int i = 0; i < 12; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box41" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box41" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box41" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-180, 0, 1, 0);
		RenderMesh(SP.Call("Box41" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 12; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box42" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box42" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box42" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(-180, 0, 1, 0);
		RenderMesh(SP.Call("Box42" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}
	/*******************************************************************************/
	for (int i = 0; i < 12; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box51" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box51" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box51" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(SP.Call("Box51" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 12; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box52" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box52" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box52" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(SP.Call("Box52" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 12; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box53" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box53" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box53" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(SP.Call("Box53" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 12; i++)
	{
		string AddToName;
		std::ostringstream ss;
		ss << i;
		AddToName = ss.str();
		modelStack.PushMatrix();
		modelStack.Translate(SP.Call("Box54" + AddToName).OBJcV->getCOORD(0),
			SP.Call("Box54" + AddToName).OBJcV->getCOORD(1),
			SP.Call("Box54" + AddToName).OBJcV->getCOORD(2));
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(SP.Call("Box54" + AddToName).OBJmesh, false);
		modelStack.PopMatrix();
	}
	/*******************************************************************************/
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("UFO").OBJcV->getCOORD(0),
		SP.Call("UFO").OBJcV->getCOORD(1) + 6,
		SP.Call("UFO").OBJcV->getCOORD(2));
	modelStack.Translate(0, 10, 0);
	RenderMesh(SP.Call("UFO").OBJmesh, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("FreezeBuff").OBJcV->getCOORD(0),
		SP.Call("FreezeBuff").OBJcV->getCOORD(1),
		SP.Call("FreezeBuff").OBJcV->getCOORD(2));
	RenderMesh(SP.Call("FreezeBuff").OBJmesh, false);
	modelStack.PopMatrix();
}

