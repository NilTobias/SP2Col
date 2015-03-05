#ifndef OBJECT_H
#define OBJECT_H

#include "Mesh.h"
#include "collisionVolume.h"

struct Object{
	Mesh* OBJmesh;
	collisionVolume* OBJcV;
	std::string Name;
	bool CollisionTrigger; //If CollisionTrigger == 1, transverse this Object through the SceneAsset
	bool ReverseCollision; //Checks if Not Colliding, then Uses Collision effect
	bool Gravity;
	Object()
	{
		CollisionTrigger = false;
		ReverseCollision = false;
		Gravity = false;
	}
};

#endif
