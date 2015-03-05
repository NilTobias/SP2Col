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
	void RenderObjective();
	void RenderFloor3();

	float LSPEED;
	float MovementSpeed;
	std::string MyTimer;
	float Floor3Timer;
	float Limiter;
	
	bool Teleport;
	

	
};

#endif