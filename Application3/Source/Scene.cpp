#include "Scene.h"
#include "GL\glew.h"
#include "Time.h"
#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "stdint.h"
#include <iostream>
#include <sstream>
#include "collisionSphere.h"
#include "Object.h"


Scene::Scene()
{
}

bool Scene::FloorLevel = false;

void Scene::Teleport()
{
}

void Scene::UpdateCrosshair(SceneAssets Target)
{
	Vector3 Temp2(3, 0, 0);
	Mtx44 charRotate;
	charRotate.SetToRotation(Target.Call("Player").OBJcV->getFace(), 0, 1, 0);
	Temp2 = charRotate * Temp2;
	Temp2 = Target.Call("Player").OBJcV->getCentre() + Temp2;
	Target.Call("Crosshair").OBJcV->setCentre(Vector3(Temp2.x, Target.Call("Crosshair").OBJcV->getCOORD(1), Temp2.z));

	if (Application::IsKeyPressed(VK_UP))
	{
		if (Target.Call("Crosshair").OBJcV->getCOORD(1) < 8)
		{
			Target.Call("Crosshair").OBJcV->setCOORD(Target.Call("Crosshair").OBJcV->getCOORD(0),
				Target.Call("Crosshair").OBJcV->getCOORD(1) + 0.1f,
				Target.Call("Crosshair").OBJcV->getCOORD(2));
		}
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		if (Target.Call("Crosshair").OBJcV->getCOORD(1) > 0)
		{
			Target.Call("Crosshair").OBJcV->setCOORD(Target.Call("Crosshair").OBJcV->getCOORD(0),
				Target.Call("Crosshair").OBJcV->getCOORD(1) - 0.1f,
				Target.Call("Crosshair").OBJcV->getCOORD(2));
		}
	}

	Vector3 NewTarget = Target.Call("Crosshair").OBJcV->getCentre();
	camera.position = Target.Call("Player").OBJcV->getCentre();
	camera.position.y = 5;
	camera.target = NewTarget;
	camera.target.y = 5;
	camera.up.Set(0, 1, 0);
	

}






void Scene::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);// TEST TEST TEST
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once 
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}



void Scene::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)// TEST TEST TEST
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}


void Scene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;// TEST TEST TEST
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.5f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}



void Scene::RenderImageOnScreen(Mesh* mesh, float size, float x, float y)
{

}


void Scene::RenderSkybox()
{
	modelStack.PushMatrix();
	//scale, translate, rotate// TEST TEST TEST
	modelStack.Translate(0, 0, -0.498);
	modelStack.Rotate(-90, 0, 0, 1);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0, 0, 0.498);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Rotate(180, 1, 0, 0);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(-0.498, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0.498, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();// TEST TEST TEST
	//scale, translate, rotate
	modelStack.Translate(0, -0.498, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0, 0.498, 0);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	/*
	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0,0.2,0);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	// TEST TEST TEST


	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0.8,0,0);
	modelStack.Rotate(-90,0,0,1);
	modelStack.Rotate(90,1,0,0);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0,0,-0.2);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
	*/
}