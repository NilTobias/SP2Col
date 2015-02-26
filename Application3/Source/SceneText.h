#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"


class SceneText  : public Scene
{
	
public:
	SceneText ();
	~SceneText ();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	static double DtCopy;
private:

	
	void Hold();
	
	

	SceneAssets SP;

	float LSPEED;
	float rotateCharacter, rotateTele;
	float engineHeat;
	float MovementSpeed;
	std::string warningTest;
	bool JetPackActivated;
	
	bool Teleport;
	

	
};

#endif