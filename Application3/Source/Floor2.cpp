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

	rotateCharacter = 0;
	rotateTele = 0;
	Teleport = false;
	isFixed = false;
	engineHeat = 0;
	LSPEED = 20.f;
	lightOn = true;
	JetPackActivated = true;
	MovementSpeed = 10;
	test = false;
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

	meshList[FLOOR2] = MeshBuilder::GenerateOBJ("SecondFloor", "OBJ//storey2.obj");
	meshList[FLOOR2]->textureID = LoadTGA("Image//storey2.tga");

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
	Player.OBJcV = new collisionSphere(3.f, Vector3(-10, 0, 0));
	Player.OBJmesh = MeshBuilder::GenerateOBJ("Player", "OBJ//doorman.obj");
	Player.OBJmesh->textureID = LoadTGA("Image//doorman.tga");
	SP.Add(Player);

	Object Can;
	Can.Name = "Bowser";
	Can.CollisionTrigger = false;
	Can.OBJcV = new collisionSphere(0.01f, Vector3(5, 0, 0));
	Can.OBJcV->setEffect(0);
	Can.OBJcV->setVelocity(Vector3(0, 5, 0));
	Can.OBJmesh = MeshBuilder::GenerateOBJ("Can", "OBJ//Bowser.obj");
	Can.OBJmesh->textureID = LoadTGA("Image//CanTexture.tga");
	SP.Add(Can);

	Object Wine;
	Wine.Name = "Wine";
	Wine.CollisionTrigger = false;
	Wine.OBJcV = new collisionSphere(2.f, Vector3(10, 0, 0));
	Wine.OBJcV->setEffect(1);
	Wine.OBJcV->setVelocity(Vector3(0, 5, 0));
	Wine.OBJmesh = MeshBuilder::GenerateOBJ("Wine", "OBJ//winebottle.obj");
	Wine.OBJmesh->textureID = LoadTGA("Image//winebottle.tga");
	SP.Add(Wine);

	Object Random;
	Random.Name = "Random";
	Random.CollisionTrigger = false;
	Random.OBJcV = new AABB(2.f, 2.f, Vector3(5, 0, 0));
	Random.OBJcV->setEffect(0);
	Random.OBJcV->setVelocity(Vector3(0, 1, 0));
	Random.OBJmesh = MeshBuilder::GenerateOBJ("Random", "OBJ//winebottle.obj");
	Random.OBJmesh->textureID = LoadTGA("Image//winebottle.tga");
	SP.Add(Random);
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
			Nice.OBJcV->setEffect(0);
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
			SP.Add(UFO);
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

	SP.CheckCollision();

	if (Application::IsKeyPressed('G'))
	{
		//SP.Call("Bowser").OBJcV->Chase(SP.Call("Player").OBJcV, 1, true);
		//SP.Call("Player").OBJcV->Jump(0.1, 20);
	}

	SP.Call("UFO").OBJcV->Chase(SP.Call("Player").OBJcV, 0.1, true);



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
	modelStack.Translate(SP.Call("Player").OBJcV->getCOORD(0),
		SP.Call("Player").OBJcV->getCOORD(1),
		SP.Call("Player").OBJcV->getCOORD(2));
	modelStack.Rotate(SP.Call("Player").OBJcV->getFace() + 90, 0, 1, 0);
	RenderMesh(SP.Call("Player").OBJmesh, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[FLOOR2], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Bowser").OBJcV->getCOORD(0),
		SP.Call("Bowser").OBJcV->getCOORD(1),
		SP.Call("Bowser").OBJcV->getCOORD(2));
	RenderMesh(SP.Call("Bowser").OBJmesh, false);
	modelStack.PopMatrix();

	/*modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Wine").OBJcV->getCOORD(0),
		SP.Call("Wine").OBJcV->getCOORD(1),
		SP.Call("Wine").OBJcV->getCOORD(2));
	RenderMesh(SP.Call("Wine").OBJmesh, false);
	modelStack.PopMatrix();*/

	/*modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Random").OBJcV->getCOORD(0),
		SP.Call("Random").OBJcV->getCOORD(1),
		SP.Call("Random").OBJcV->getCOORD(2));
	RenderMesh(SP.Call("Random").OBJmesh, false);
	modelStack.PopMatrix();
	*/
	/*******************************************************************************/
	for(int i = 0; i < 5; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Packet1" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Packet1" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Packet1" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Packet1" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}
	for(int j = 0; j < 5; j++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << j;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Packet2" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Packet2" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Packet2" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Packet2" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}
	for(int k = 0; k < 5; k++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << k;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Packet3" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Packet3" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Packet3" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Packet3" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}
	for(int i = 0; i < 5; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Packet4" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Packet4" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Packet4" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Packet4" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}
	/*******************************************************************************/

	for(int i = 0; i < 5; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Sugar1" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Sugar1" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Sugar1" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Sugar1" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}
	/*******************************************************************************/
	for(int i = 0; i < 5; i++)
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

	for(int i = 0; i < 5; i++)
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

	for(int i = 0; i < 5; i++)
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

	for(int i = 0; i < 5; i++)
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
	for(int i = 0; i < 5; i++)
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
	for(int i = 0; i < 5; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box21" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box21" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box21" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Box21" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}

	for(int i = 0; i < 5; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box22" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box22" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box22" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Box22" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}

	for(int i = 0; i < 5; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box23" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box23" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box23" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Box23" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}

	for(int i = 0; i < 2; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box24" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box24" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box24" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Box24" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}

	for(int i = 0; i < 2; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box25" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box25" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box25" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Box25" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}

	for(int i = 0; i < 5; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box26" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box26" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box26" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Box26" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}

	for(int i = 0; i < 5; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box27" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box27" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box27" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Box27" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}

	for(int i = 0; i < 5; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box28" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box28" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box28" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Box28" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}

	for(int i = 0; i < 5; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box29" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box29" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box29" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Box29" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}
	/*******************************************************************************/
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Nice").OBJcV->getCOORD(0),
		SP.Call("Nice").OBJcV->getCOORD(1),
		SP.Call("Nice").OBJcV->getCOORD(2));
	modelStack.Rotate(180,0,1,0);
	RenderMesh(SP.Call("Nice").OBJmesh, false);
	modelStack.PopMatrix();
	/*******************************************************************************/
	for(int i = 0; i < 5; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box31" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box31" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box31" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Box31" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}

	for(int i = 0; i < 5; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box32" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box32" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box32" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Box32" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}

	for(int i = 0; i < 5; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box33" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box33" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box33" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Box33" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}

	for(int i = 0; i < 5; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box34" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box34" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box34" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(SP.Call("Box34" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}
	/*******************************************************************************/
	for(int i = 0; i < 12; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box41" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box41" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box41" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-180,0,1,0);
	RenderMesh(SP.Call("Box41" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}

	for(int i = 0; i < 12; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box42" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box42" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box42" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(-180,0,1,0);
	RenderMesh(SP.Call("Box42" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}
	/*******************************************************************************/
	for(int i = 0; i < 12; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box51" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box51" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box51" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(90,0,1,0);
	RenderMesh(SP.Call("Box51" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}

	for(int i = 0; i < 12; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box52" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box52" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box52" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(90,0,1,0);
	RenderMesh(SP.Call("Box52" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}

	for(int i = 0; i < 12; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box53" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box53" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box53" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(90,0,1,0);
	RenderMesh(SP.Call("Box53" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}

	for(int i = 0; i < 12; i++)
	{
	string AddToName;
	std::ostringstream ss;
	ss << i;
	AddToName = ss.str();
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("Box54" + AddToName).OBJcV->getCOORD(0),
		SP.Call("Box54" + AddToName).OBJcV->getCOORD(1),
		SP.Call("Box54" + AddToName).OBJcV->getCOORD(2));
	modelStack.Rotate(90,0,1,0);
	RenderMesh(SP.Call("Box54" + AddToName).OBJmesh, false);
	modelStack.PopMatrix();
	}
	/*******************************************************************************/
	modelStack.PushMatrix();
	modelStack.Translate(SP.Call("UFO").OBJcV->getCOORD(0),
		SP.Call("UFO").OBJcV->getCOORD(1)+ 6,
		SP.Call("UFO").OBJcV->getCOORD(2));
	modelStack.Translate(0,10,0);
	RenderMesh(SP.Call("UFO").OBJmesh, false);
	modelStack.PopMatrix();

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

