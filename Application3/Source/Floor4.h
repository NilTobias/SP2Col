#pragma once


#include "Scene.h"

class Floor4 : public Scene
{
	public:
	Floor4();
	~Floor4();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	static double DtCopy;
private:

	
	void Hold();
	int sound();

	float LSPEED;
	float rotateCharacter, rotateTele;

	float rise;

	float MovementSpeed;
	std::string warningTest;
	bool JetPackActivated;
	SceneAssets SP4;

	bool test;
};