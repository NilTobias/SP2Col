#include "SceneAssets.h"
#include "collisionSphere.h"
#include "AABB.h"

SceneAssets::SceneAssets()
{
	head = tail = current = NULL;
	TotalAssets = 0;
}

SceneAssets::~SceneAssets()
{

}

void SceneAssets::Add(Object target)
{
	TotalAssets++;
	newNode = new CNode(target);
	if (head == NULL)
	{
		head = tail = newNode;
	}
	else
	{
		newNode->prev = tail;
		tail->next = newNode;
		tail = newNode;
	}
}

Object SceneAssets::Call(std::string ObjectName)
{
		current = head;
		while (current != NULL)
		{
			if (current->Data.Name == ObjectName)
			{
				return current->Data;
			}
			current = current->next;
		}
}

bool SceneAssets::Delete(std::string toDelete)
{
	if (head != NULL)
	{
		current = head;
		while (current != NULL)
		{
			if (current->Data.Name.compare(toDelete) == 0)
				break;
			else
			{
				prev = current;
				current = current->next;
			}
		}
		if (current == head)
		{
			if (head->next != NULL)
			{
				head = head->next;
				delete current;
				return true;
			}
			else
			{
				delete current;
				head = tail = NULL;
				return true;
			}
		}
		else if (current == tail)
		{
			tail = prev;
			tail->next = NULL;
			delete current;
			return true;
		}
		else if (current == NULL)
		{
			return false;
		}
		else
		{
			prev->next = current->next;
			delete current;
			return true;
		}
	}
	else
		return false;
}

void SceneAssets::CheckCollision()
{

	collisionSphere* TempSphere;
	AABB* TempBox;
	bool isSphere;

	collisionSphere* TempSphere2;
	AABB* TempBox2;
	bool isSphere2;


	if (head != NULL)
	{
		current = head;
		
		while (current != NULL)
		{
				if (current->Data.OBJcV->getCollisionType() == 0)//It is a Sphere
				{
					TempSphere = ((collisionSphere*)current->Data.OBJcV);
					isSphere = true;
				}
				else if (current->Data.OBJcV->getCollisionType() == 1)//It is a Box
				{
					TempBox = ((AABB*)current->Data.OBJcV);
					isSphere = false;
				}
				check = current->next;

				while (check != NULL)
				{
					
					if (check->Data.OBJcV->getCollisionType() == 0)//It is a Sphere
					{
						TempSphere2 = ((collisionSphere*)check->Data.OBJcV);
						isSphere2 = true;
					}
					else if (check->Data.OBJcV->getCollisionType() == 1)//It is a Box
					{
						TempBox2 = ((AABB*)check->Data.OBJcV);
						isSphere2 = false;
					}

					if (isSphere)
					{
						if (isSphere2)
						{
							if (TempSphere2->checkCollision(TempSphere))//TS = Player, TS2 = Collision
								check->Data.OBJcV->CollisionEffect(current->Data.OBJcV);
						}
						else
						{
							if (check->Data.ReverseCollision)
							{
								if (TempSphere->checkCollision(TempBox2) == false)
									check->Data.OBJcV->CollisionEffect(current->Data.OBJcV);
							}
							else if (check->Data.ReverseCollision == false)
							{
								if (TempSphere->checkCollision(TempBox2))
									check->Data.OBJcV->CollisionEffect(current->Data.OBJcV);
							}
						}
					}
					else if (!isSphere)
					{
						if (isSphere2)
						{
							if (TempSphere->checkCollision(TempBox2))
								check->Data.OBJcV->CollisionEffect(current->Data.OBJcV);
						}	
					}
					check = check->next;
					
				}
			current = current->next;
		}
	}
}

void SceneAssets::Gravity(float force)
{
	check = head;
	while (check != NULL)
	{
		if (check->Data.Gravity)
		{
			if (check->Data.OBJcV->getCOORD(1) > check->Data.OBJcV->getAlt())
			{
			Vector3 thisVector(1, 0, 0);
			Vector3 thisOtherVector(0, 0, 1);

			Vector3 Gravity = thisVector.Cross(thisOtherVector);
			Gravity.Normalize();
			Gravity *= force;
			float NewAltitude = check->Data.OBJcV->getCOORD(1) - Gravity.Length();
			check->Data.OBJcV->setCentre(Vector3(check->Data.OBJcV->getCOORD(0),NewAltitude, check->Data.OBJcV->getCOORD(2)));
			}
		}
		check = check->next;
	}
}
