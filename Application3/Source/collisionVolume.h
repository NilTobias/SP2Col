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

	int getEffect(bool mainEffect = true)const;
	void setEffect(int Eff, int secEff = 5);

	bool getActivate(void)const;
	void setActivate(bool Act);

	float getTimer(void)const;
	void setTimer(float time);

	float getRise(void)const;
	void setRise(float RISE);

	float getAlt(void)const;
	void setAlt(float Alt);

	bool getLiving(void)const;
	void setLiving(bool life);

	void Chase(collisionVolume* Target, float Speed, float flying);
	void Jump(float JumpStrength, float MaxHeight = 5);

	void CollisionEffect(collisionVolume* Target);

	static bool AllowPickUp;
	static bool AllowForce;
	static bool AllowActivate;
	static bool MainFace[4];//W = 0, S = 1, A = 2, D = 3
	static int MoveKeysPressed;
private:
	Vector3 collisionCentre;
	Vector3 Velocity;
	Mtx44 charRotate;
	float Face;
	float timer;
	float RiseObject; //Elevates the Object which sometimes sinks in when you try to set its Y Value to another Object's Y value
	float Altitude; //The Level of the Platform at which it is standing
	int type; //0 == Sphere, 1 == AABB
	bool Activate; //Object's Boolean that will be True when Collide
	bool gravity;
	bool maxVelo;
	bool Living;


	int Effect, SecondaryEffect;
	/*
	  0 == Items That You Pick Up, 
	  1 == Items That Act as Stationary Bounds, 
	  2 == Items That Activate Things(Change Boolean Value)
	  3 == Items That Adds a Vector to your Player
	*/
};

#endif COLLISION_VOLUME_H