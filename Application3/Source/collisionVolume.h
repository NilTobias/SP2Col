#ifndef COLLISION_VOLUME_H
#define COLLISION_VOLUME_H

#include "Vector3.h"
#include "Mtx44.h"

class collisionVolume
{
public:
	collisionVolume(Vector3 cC, int eff = 0);
	collisionVolume();
	~collisionVolume();

	Vector3 getCentre(void)const;
	void setCentre(Vector3 cC);

	Vector3 getVelocity(void)const;
	void setVelocity(Vector3 v);

	float getCOORD(int Axis)const;
	void setCOORD(float X, float Y, float Z);

	int getCollisionType(void)const;
	void setCollisionType(int t);

	float getFace(void)const;
	void setFace(float f);

	int getEffect(void)const;
	void setEffect(int eff);

	bool getActivate(void)const;
	void setActivate(bool Act);

	void Chase(collisionVolume* Target, float Speed);

	void CollisionEffect(collisionVolume* Target);

	static bool AllowPickUp;
	static bool AllowForce;
	static bool AllowActivate;
	static int MainFace;//W = 0, S = 1, A = 2, D = 3
private:
	Vector3 collisionCentre;
	Vector3 Velocity;
	Mtx44 charRotate;
	float Face;
	float timer;
	int type; //0 == Sphere, 1 == AABB
	bool Activate; //Object's Boolean that will be True when Collide


	int Effect;
	/*
	  0 == Items That You Pick Up, 
	  1 == Items That Act as Stationary Bounds, 
	  2 == Items That Activate Things(Change Boolean Value)
	  3 == Items That Adds a Vector to your Player
	*/
};

#endif COLLISION_VOLUME_H