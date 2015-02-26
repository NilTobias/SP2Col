#include "AABB.h"

AABB::AABB()
{
	length = 1;
	width = 1;
	height = 1;
	maximum.Set(getCentre().x + width / 2, getCentre().y + height / 2, getCentre().z + length / 2);
	minimum.Set(getCentre().x - width / 2, getCentre().y - height / 2, getCentre().z - length / 2);
	setCollisionType(1);
	
}

AABB::AABB(float l, float w, Vector3 cC)
{
	length = l;
	width = w;
	maximum.Set(getCentre().x + width / 2, getCentre().y + height / 2, getCentre().z + length / 2);
	minimum.Set(getCentre().x - width / 2, getCentre().y - height / 2, getCentre().z - length / 2);
	setCollisionType(1);
	setCentre(cC);
}

AABB::AABB(Vector3 min, Vector3 max, Vector3 cC)
{
	maximum = max;
	minimum = min;
	length = maximum.z - minimum.z;
	width = maximum.x - minimum.x;
	height = maximum.y - minimum.y;
	setCentre(cC);
}

void AABB::setBox(float l, float w, float h)
{
	length = l;
	width = w;
	height = h;
	maximum.Set(getCentre().x + width / 2, getCentre().y + height / 2, getCentre().z + length / 2);
	minimum.Set(getCentre().x - width / 2, getCentre().y - height / 2, getCentre().z - length / 2);
}

float AABB::getLength(void)const
{
	return length;
}
void AABB::setLength(float l)
{
	length = l;
}

float AABB::getWidth(void)const
{
	return width;
}
void AABB::setWidth(float w)
{
	width = w;
}

float AABB::getHeight(void)const
{
	return height;
}
void AABB::setHeight(float h)
{
	height = h;
}

Vector3 AABB::getMinimum(void)const
{
	return minimum;
}

Vector3 AABB::getMaximum(void)const
{
	return maximum;
}

/*
bool AABB::checkCollision(AABB *target)
{
	Vector3 d1 = getMaximum() + target.getMaximum();
	Vector3 d2 = getMinimum() - target.getMinimum();

	return d2.greaterThan(d1);
}
*/
