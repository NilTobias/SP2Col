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
	void Task3Complete();

	void ShootTo(std::string Target,Vector3 Destination, Vector3 Source, float MaxDistance, float BulletSpeed);//Source will Always be the Totems.
	void ShootTo2(std::string Target,Vector3 Destination, Vector3 Source, float MaxDistance, float BulletSpeed);
	void ShootTo3(std::string Target,Vector3 Destination, Vector3 Source, float MaxDistance, float BulletSpeed);
	void ShootTo4(std::string Target,Vector3 Destination, Vector3 Source, float MaxDistance, float BulletSpeed);

	float LSPEED;
	float MovementSpeed;
	std::string MyTimer;
	float Floor3Timer;
	float Limiter;
	float speed;
	bool Teleport;
	

	
};

#endif