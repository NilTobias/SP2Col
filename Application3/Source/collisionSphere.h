#ifndef COLLISION_SPHERE_H
#define COLLISION_SPHERE_H

#include "collisionVolume.h"
#include "AABB.h"

class collisionSphere : public collisionVolume
{
public:
	collisionSphere(float r);
	collisionSphere(float r, Vector3 cC);
	~collisionSphere();

	float getRadius(void)const;
	void setRadius(float r);

	bool checkCollision(collisionSphere *target);
	bool checkCollision(AABB *target);

private:
	float radius;
};


#endif