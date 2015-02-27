#include "collisionSphere.h"
#include <math.h>

collisionSphere::collisionSphere(float r)
{
	radius = r;
	setCollisionType(0);
}

collisionSphere::collisionSphere(float r, Vector3 cC)
{
	radius = r;
	setCollisionType(0);
	setCentre(cC);
}

collisionSphere::~collisionSphere()
{

}

float collisionSphere::getRadius(void)const
{
	return radius;
}
void collisionSphere::setRadius(float r)
{
	if (r > 0)
		radius = r;
	setCollisionType(0);
}

bool collisionSphere::checkCollision(collisionSphere *target)
{
	//Distance Between Their Centres
	float d = pow((getCentre().x - target->getCentre().x), 2)
		+ pow((getCentre().y - target->getCentre().y), 2)
		+ pow((getCentre().z - target->getCentre().y), 2);

	//Combined Radius of Two Bounding Spheres
	float tR = pow((getRadius() + target->getRadius()), 2);

	if (d <= tR)
	{
		return true;
		target->setEffect(true);
	}	
	else
		return false;
}

bool collisionSphere::checkCollision(AABB *target)
{
Vector3 d = getCentre() - target->getCentre();
Vector3 minDist;


if (d.x < -target->getWidth() / 2.0)
minDist.x = -target->getWidth() / 2.0;
else if (d.x > target->getWidth() / 2.0)
minDist.x = target->getWidth() / 2.0;
else
minDist.x = d.x;

// ...same for Y axis
if (d.y < -target->getHeight() / 2.0)
minDist.y = -target->getHeight() / 2.0;
else if (d.y > target->getHeight() / 2.0)
minDist.y = target->getHeight() / 2.0;
else
minDist.y = d.y;

// ... same for Z axis
if (d.z < -target->getLength() / 2.0)
minDist.z = -target->getLength() / 2.0;
else if (d.x > target->getLength() / 2.0)
minDist.z = target->getLength() / 2.0;
else
minDist.z = d.z;

// Now we have the closest point on the box, so get the distance from
// that to the sphere center, and see if it's less than the radius

Vector3 dist = d - minDist;

if (pow(dist.x,2) + pow(dist.y,2) + pow(dist.z,2) < pow(getRadius(),2))
{
	target->setEffect(true);
	return true;
}
else
return false;
}


/*Tests

Vec3 SepAxis = Sphere.Center - AABB.Center;
float Dist = Vec3Length( SepAxis );

// then, find the unit vector that points from the box center to the sphere center
Vec3Normalize( SepAxis );

// divide each component of the unit vector by the maximum component, effectively "normalizing" the unit vector
if( SepAxis.x >= SepAxis.y && SepAxis.x >= SepAxis.z )
SepAxis /= SepAxis.x;
else if( SepAxis.y >= SepAxis.x && SepAxis.y >= SepAxis.z )
SepAxis /= SepAxis.y;
else
SepAxis /= SepAxis.z;

// Now, find the effective radius of the box along the "normalized" unit vector pointing to the sphere
SepAxis.x *= target.getWidth() / 2.0f;
SepAxis.y *= target.getHeight() / 2.0f;
SepAxis.z *= target.getLength() / 2.0f;

// Finally, add the sphere radius to the box radius and compare to the distance
if( Dist <= (Sphere.Radius + Vec3Length( SepAxis ) )
Collision true!
else
Collision false

------------------------------------------------------

// Get the center of the sphere relative to the center of the box
Vec3 d = Sphere.center - Box.center;
// Point on surface of box that is closest to the center of the sphere
Vec3 minDist;


if (d.x < -target.getWidth()/2.0)
minDist.x = -target.getWidth()/2.0;
else if (d.x > target.getWidth()/2.0)
minDist.x = target.getWidth()/2.0;
else
minDist.x = d.x;

// ...same for Y axis
if (d.y < -target.getHeight()/2.0)
minDist.y = -target.getHeight()/2.0;
else if (d.y > target.getHeight()/2.0)
minDist.y = target.getHeight()/2.0;
else
minDist.y = d.y;

// ... same for Z axis
if (d.z < -target.getLength()/2.0)
minDist.z = -target.getLength()/2.0;
else if (d.x > target.getLength()/2.0)
minDist.z = target.getLength()/2.0;
else
minDist.z = d.z;

// Now we have the closest point on the box, so get the distance from
// that to the sphere center, and see if it's less than the radius

Vec3 dist = d - minDist;

if (dist.x*dist.x + dist.y*dist.y + distz*dist.z < Sphere.radius*Sphere.radius)
return true;
else
return false;
*/