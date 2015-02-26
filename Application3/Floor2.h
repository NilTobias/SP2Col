#ifndef FLOOR2_H
#define FLOOR2_H

#include "Scene.h"

	
class Floor2  : public Scene
{
	
public:
	Floor2 ();
	~Floor2 ();

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