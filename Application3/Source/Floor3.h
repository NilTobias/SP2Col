#ifndef FLOOR_3_H
#define FLOOR_3_H

#include "Scene.h"


class Floor3  : public Scene
{
	
public:
	Floor3 ();
	~Floor3 ();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	static double DtCopy;
private:
	void RenderSP();
	

	float LSPEED;
	float rotateCharacter, rotateTele;
	float engineHeat;
	float MovementSpeed;
	std::string warningTest;
	bool JetPackActivated;
	
	bool Teleport;
	

	
};

#endif