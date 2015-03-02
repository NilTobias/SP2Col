#ifndef AABB_H
#define AABB_H

#include "Vector3.h"
#include "collisionVolume.h"

class AABB : public collisionVolume
{
	/*  Two Ways To Create Box: (Matter of Preference)
	1)Provide Length/Width
	2)Provide Its Minimum and Maximum Extents
	*/
public:
	AABB(float l, float w, float h, Vector3 cC);
	AABB(Vector3 min, Vector3 max, Vector3 cC);
	AABB();
	~AABB();

	float getLength(void)const;
	void setLength(float l);

	float getWidth(void)const;
	void setWidth(float w);

	float getHeight(void)const;
	void setHeight(float h);

	void setBox(float l, float w, float h);
	float getBox(bool side_length)const;

	void setMinimum(Vector3 min);
	void setMaximum(Vector3 max);
	Vector3 getMinimum(void)const;
	Vector3 getMaximum(void)const;

private:
	float length;
	float width;
	float height;
	Vector3 minimum;
	Vector3 maximum;
};


#endif