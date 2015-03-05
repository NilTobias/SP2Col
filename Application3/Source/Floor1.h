#ifndef FLOOR_1_H
#define FLOOR_1_H


#include "Scene.h";


class Floor1 : public Scene
{

public:
	Floor1();
	~Floor1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	static double DtCopy;
    int sound();
private:
	void RenderSP();
	SceneAssets SP;

	float LSPEED;
	float rotateCharacter, rotateTele;
	float engineHeat;
	float MovementSpeed;
	std::string warningTest;
	bool JetPackActivated;

	bool Teleport;
	bool renderScreenText;
    bool test;
	float FPS;

};

#endif