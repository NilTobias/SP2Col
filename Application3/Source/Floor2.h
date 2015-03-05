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
	void RenderTaskList();
	void RenderFloor2();
	void Task2Complete();
	

	float SpeedUp;
	float Floor2Timer;
	float FreezeTimer;
	float Limiter;
	float LSPEED;
	float MovementSpeed;
	std::string warningTest;
	bool JetPackActivated;
	std::string MyTimer;
	
	
	bool Teleport;
	bool test;
	bool TaskList[3];

	
};

#endif