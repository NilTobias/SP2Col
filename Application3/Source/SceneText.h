#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Camera2.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Utility.h"
#include "Application.h"
#include "collisionSphere.h"
#include "collisionVolume.h"
#include "AABB.h"
#include "SceneAssets.h"

class SceneText  : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	enum GEOMETRY_TYPE
	{
		GEO_AXES = 1,
		GEO_LIGHTBALL,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_NOOB,
		GEO_MODEL1,
		GEO_MODEL4,
		GEO_MODEL6,
		TELEPORTER,
		THIRDFLOOR,
		GEO_TEXT,
		NUM_GEOMETRY,
	};
public:
	SceneText ();
	~SceneText ();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	static double DtCopy;
private:

	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderImageOnScreen(Mesh* mesh, float size, float x, float y);
	void RenderSkybox();
	void Hold();
	
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	collisionVolume* cV[NUM_GEOMETRY];
	collisionSphere *Player, *Teleporter, *PickUp;

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	SceneAssets SP;

	float LSPEED;
	float rotateCharacter, rotateTele;
	float engineHeat;
	float MovementSpeed;
	std::string warningTest;
	bool JetPackActivated;
	bool lightOn;
	bool Teleport;
	bool isFixed;

	Camera2 camera;
	Light lights[1];

	MS modelStack, viewStack, projectionStack;
};

#endif