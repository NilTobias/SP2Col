#ifndef FLOOR_H
#define FLOOR_H

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
	int sound();
	

	

	float LSPEED;
	float rotateCharacter, rotateTele;
	float engineHeat;
	float MovementSpeed;
	std::string warningTest;
	bool JetPackActivated;
	
	bool Teleport;
	bool test;

	
};

#endif