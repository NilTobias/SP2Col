#include "collisionVolume.h"
#include <math.h>
#include "Mtx44.h"
#include "Floor3.h"

collisionVolume::collisionVolume(Vector3 cC, int eff)
{
	setCentre(cC);
	Velocity.SetZero();
	Face = 0;
	setEffect(eff);
	setActivate(false);
	timer = 0;
	maxVelo = 0;
}

collisionVolume::collisionVolume()
{
	collisionCentre.SetZero();
	Velocity.SetZero();
	Face = 0;
	setEffect(0);
	setActivate(false);
	timer = 0;
	maxVelo = 0;
}

collisionVolume::~collisionVolume()
{

}

bool collisionVolume::AllowPickUp = false;
bool collisionVolume::AllowForce = false;
bool collisionVolume::AllowActivate = false;
bool collisionVolume::MainFace[4] = { false, false, false, false };
int collisionVolume::MoveKeysPressed = 0;

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

void collisionVolume::Chase(collisionVolume* Target, float Speed, float flying)
{
	Vector3 ToTarget;

	ToTarget = Target->getCentre() - getCentre();

	ToTarget.Normalize();
	ToTarget *= Speed;

	if (flying)
		ToTarget.y = 0;

	setCentre(getCentre() + ToTarget);
}

void collisionVolume::Jump(float JumpStrength, float MaxHeight)
{
	Vector3 thisVector(1, 0, 0);
	Vector3 thisOtherVector(0, 0, 1);

	Vector3 up = thisVector.Cross(thisOtherVector);
	up.Normalize();
	up *= JumpStrength;
	setCentre(getCentre() - up);
	maxVelo -= up.Length();
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

float collisionVolume::getTimer(void)const
{
	return timer;
}
void collisionVolume::setTimer(float time)
{
	timer = time;
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
		if (MainFace[0]) //W
		{
			Target->setVelocity(Vector3(10 * Floor3::DtCopy, 0, 0));
			charRotate.SetToRotation(Target->getFace(), 0, 1, 0);
			Target->setVelocity(charRotate * Target->getVelocity());
			Target->setCentre(Target->getCentre() - Target->getVelocity());
		}
		if (MainFace[1]) //S
		{
			Target->setVelocity(Vector3(10 * Floor3::DtCopy, 0, 0));
			charRotate.SetToRotation(Target->getFace(), 0, 1, 0);
			Target->setVelocity(charRotate * Target->getVelocity());
			Target->setCentre(Target->getCentre() + Target->getVelocity());
		}
		if (MainFace[2])//A
		{
			Target->setVelocity(Vector3(0, 0, 10 * Floor3::DtCopy));
			charRotate.SetToRotation(Target->getFace(), 0, 1, 0);
			Target->setVelocity(charRotate * Target->getVelocity());
			Target->setCentre(Target->getCentre() + Target->getVelocity());
		}
		if (MainFace[3])//D
		{
			Target->setVelocity(Vector3(0, 0, 10 * Floor3::DtCopy));
			charRotate.SetToRotation(Target->getFace(), 0, 1, 0);
			Target->setVelocity(charRotate * Target->getVelocity());
			Target->setCentre(Target->getCentre() - Target->getVelocity());
		}
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

