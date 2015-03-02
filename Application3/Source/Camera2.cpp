#include "Camera2.h"
#include "Application.h"
#include "Mtx44.h"

Camera2::Camera2()
{
}

Camera2::~Camera2()
{
}

void Camera2::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	lockCamera = true;
}

void Camera2::Update(double dt)
{
	if (!lockCamera)
	{
	static const float CAMERA_SPEED = 25.f;
	if(Application::IsKeyPressed('A')){
		Vector3 view = (target - position).Normalize();
		Vector3 right = view.Cross(up);
		position -= right* CAMERA_SPEED * dt;
		target -= right * CAMERA_SPEED * dt;
        view.y = 0;
	}

	if(Application::IsKeyPressed('D')){
		Vector3 view = (target - position).Normalize();
		Vector3 right = view.Cross(up);
		position += right* CAMERA_SPEED * dt;
		target += right * CAMERA_SPEED * dt;
        view.y = 0;
	}

	if(Application::IsKeyPressed('W')){
		Vector3 view = (target - position).Normalize();
		position += view * CAMERA_SPEED * dt;
		target += view * CAMERA_SPEED * dt;
        view.y = 0;
	}

	if(Application::IsKeyPressed('S')){
		Vector3 view = (target - position).Normalize();
		position -= view * CAMERA_SPEED * dt;
		target -= view * CAMERA_SPEED * dt;
        view.y = 0;
	}

	if(Application::IsKeyPressed(VK_LEFT))
	{
		float yaw = (float)(CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		target = rotation * (target - position) + position;
		up = rotation * up;
		temp_store += yaw;
	}

	if(Application::IsKeyPressed(VK_RIGHT))
	{
		float yaw = (float)(-CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		target = rotation * (target - position) + position;
		up = rotation * up;
		temp_store += yaw;
	}

	if(Application::IsKeyPressed(VK_UP))
	{
        float pitch;
        //if (limit <= 110.f)
        //{
		pitch = (float)(CAMERA_SPEED * dt);
        limit += pitch;
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		target = rotation * (target - position) + position;
        //}
	}

	if(Application::IsKeyPressed(VK_DOWN))
	{   
        float pitch;
        //if(limit >= -45.f)
		//{
		pitch = (float)(-CAMERA_SPEED * dt);
        limit -= pitch;
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		target = rotation * (target - position) + position;
        //}
	}

	if(Application::IsKeyPressed('N'))
	{
		Vector3 direction = target - position;
		if(direction.Length() > 5)
		{
			Vector3 view = (target - position).Normalized();
			position += view * (float)(10.f * dt);
		}
	}
	if(Application::IsKeyPressed('M'))
	{
		Vector3 direction = target - position;
		if (direction.Length() < 500)
		{
		Vector3 view = (target - position).Normalized();
		position -= view * (float)(10.f * dt);
		}
	}
	}
	if (Application::IsKeyPressed('V'))
	{
		lockCamera = false;
	}
	else if (Application::IsKeyPressed('B'))
	{
		lockCamera = true;
	}

	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void Camera2::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}