#ifndef SCENE_ASSETS_H
#define SCENE_ASSETS_H
#include "Object.h"
#include "CNode.h"

class SceneAssets{
public:
	SceneAssets();
	~SceneAssets();

	void Add(Object target);
	bool Delete(std::string toDelete);
	Object Call(std::string ObjectName);
	void CheckCollision();
	void Gravity(float force = 1);
	Mesh* RenderAssets();
private:
	CNode *head, *tail, *current, *newNode, *prev, *check;
	int TotalAssets;
	int ForRender;
};


#endif