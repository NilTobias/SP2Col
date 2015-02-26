#include "collisionVolume.h"
#include <math.h>
#include "Mtx44.h"
#include "SceneText.h"

collisionVolume::collisionVolume(Vector3 cC, int eff)
{
	setCentre(cC);
	Velocity.SetZero();
	Face = 0;
	setEffect(eff);
	setActivate(false);
}

collisionVolume::collisionVolume()
{
	collisionCentre.SetZero();
	Velocity.SetZero();
	Face = 0;
	setEffect(0);
	setActivate(false);
}

collisionVolume::~collisionVolume()
{

}

bool collisionVolume::AllowPickUp = false;
bool collisionVolume::AllowForce = false;
bool collisionVolume::AllowActivate = false;
int collisionVolume::MainFace = 0;

Vector3 collisionVolume::getCentre(void)const
{
	return collisionCentre;
}
void collisionVolume::setCentre(Vector3 cC)
{
	collisionCentre = cC;
}

Vector3 collisionVolume::getVelocity(void)const
{
	return Velocity;
}
void collisionVolume::setVelocity(Vector3 v)
{
	Velocity = v;
}

float collisionVolume::getCOORD(int Axis)const
{
	if (Axis == 0)
		return collisionCentre.x;
	else if (Axis == 1)
		return collisionCentre.y;
	else
		return collisionCentre.z;
}
void collisionVolume::setCOORD(float X, float Y, float Z)
{
	collisionCentre.x = X;
	collisionCentre.y = Y;
	collisionCentre.z = Z;
}

int collisionVolume::getCollisionType(void)const
{
	return type;
}
void collisionVolume::setCollisionType(int t)
{
	type = t;
}

float collisionVolume::getFace(void)const
{
	return Face;
}
void collisionVolume::setFace(float f)
{
	Face = f;
}

void collisionVolume::Chase(collisionVolume* Target, float Speed)
{
	Vector3 DistanceAwayFromPlayer;

	DistanceAwayFromPlayer = Target->getCentre() - getCentre();

	Vector3 ChasingSpeed = DistanceAwayFromPlayer;

	setCentre(getCentre() + ChasingSpeed);
}

int collisionVolume::getEffect(void)const
{
	return Effect;
}
void collisionVolume::setEffect(int eff)
{
	Effect = eff;
}

bool collisionVolume::getActivate(void)const
{
	return Activate;
}
void collisionVolume::setActivate(bool Act)
{
	Activate = Act;
}

void collisionVolume::CollisionEffect(collisionVolume *Target)
{
	switch (getEffect())
	{
		//Pick Up
	case 0: 
		if (AllowPickUp)
		setCentre(Target->getCentre());
		break;
		//Stationary Bounds(Walls/Shelves)
	case 1:
		switch (MainFace)
		{
		case 0: //W
		Target->setVelocity(Vector3(10 * SceneText::DtCopy, 0, 0));
		charRotate.SetToRotation(Target->getFace(), 0, 1, 0);
		Target->setVelocity(charRotate * Target->getVelocity());
		Target->setCentre(Target->getCentre() - Target->getVelocity());
		break;
		case 1://S
		Target->setVelocity(Vector3(10 * SceneText::DtCopy, 0, 0));
		charRotate.SetToRotation(Target->getFace(), 0, 1, 0);
		Target->setVelocity(charRotate * Target->getVelocity());
		Target->setCentre(Target->getCentre() + Target->getVelocity());
		break;
		case 2://A
		Target->setVelocity(Vector3(0, 0, 10 * SceneText::DtCopy));
		charRotate.SetToRotation(Target->getFace(), 0, 1, 0);
		Target->setVelocity(charRotate * Target->getVelocity());
		Target->setCentre(Target->getCentre() + Target->getVelocity());
		break;
		case 3://D
		Target->setVelocity(Vector3(0, 0, 10 * SceneText::DtCopy));
		charRotate.SetToRotation(Target->getFace(), 0, 1, 0);
		Target->setVelocity(charRotate * Target->getVelocity());
		Target->setCentre(Target->getCentre() - Target->getVelocity());
		break;
		}
		break;
		//Activation
	case 2:
		if (AllowActivate)
		setActivate(true);
		break;
		//Add Vector
	case 3:
		Target->setCentre(Target->getCentre() + getVelocity());
		break;
	default:
		break;
	}
}

