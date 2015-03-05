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
	maxVelo = false;
	Living = false;
	RiseObject = 0;
	Altitude = 0;
	isFixed = false;
	isCursor = false;
}

collisionVolume::collisionVolume()
{
	collisionCentre.SetZero();
	Velocity.SetZero();
	Face = 0;
	setEffect(0);
	setActivate(false);
	maxVelo = false;
	Living = false;
	RiseObject = 0;
	Altitude = 0;
	isFixed = false;
	isCursor = false;
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

void collisionVolume::Chase(collisionVolume* Target, float Speed, bool flying)
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
	if (getCOORD(1) <= MaxHeight && !maxVelo)
	{
		Vector3 thisVector(1, 0, 0);
		Vector3 thisOtherVector(0, 0, 1);

		Vector3 up = thisVector.Cross(thisOtherVector);
		up.Normalize();
		up *= JumpStrength;
		setCentre(getCentre() - up);
		if (getCOORD(1) >= MaxHeight)
			maxVelo = true;
	}
	else if (getCOORD(1) <= 0 && maxVelo)
	{
		setCOORD(getCOORD(0),0,getCOORD(2));
		maxVelo = false;
	}	
}

bool collisionVolume::getLiving(void)const
{
	return Living;
}
void collisionVolume::setLiving(bool life)
{
	Living = life;
}

int collisionVolume::getEffect(bool mainEffect)const
{
	if (mainEffect)
		return Effect;
	else
		return SecondaryEffect;
}
void collisionVolume::setEffect(int eff, int secEff)
{
	Effect = eff;
	SecondaryEffect = secEff;
}

bool collisionVolume::getActivate(void)const
{
	return Activate;
}
void collisionVolume::setActivate(bool Act)
{
	Activate = Act;
}

bool collisionVolume::getFixed(void)const
{
	return isFixed;
}
void collisionVolume::setFixed(bool f)
{
	isFixed = f;
}

float collisionVolume::getAlt(void)const
{
	return Altitude;
}
void collisionVolume::setAlt(float Alt)
{
	Altitude = Alt;
}

bool collisionVolume::getCursor(void)const
{
	return isCursor;
}
void collisionVolume::setCursor(bool cur)
{
	isCursor = cur;
}

void collisionVolume::CollisionEffect(collisionVolume *Target)
{
	//Pick Up
	if (getEffect() == 0)
	{
		if (AllowPickUp && Target->getCursor())
		{
			setCentre(Target->getCentre());
			Target->setAlt(0);
		}
	}


	//Stationary Bounds(Walls/Shelves)
	if (getEffect() == 1)
	{
		if (Target->getLiving())
		{
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
		}
		else if (Target->getLiving() == false)
		{
			Target->setAlt(getCOORD(1));
		}
	}

	//Activation
	if (getEffect() == 2 || getEffect(false))
	{
		if (AllowActivate)
		{ 
			setActivate(true);
			Target->setActivate(true);
		}
	}
}

