#include <iostream>
using namespace std;

#include "IntroState.h"
#include "GL\glew.h"
#include "Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "TextEntity.h"
#include "RenderHelper.h"
#include "SpriteEntity.h"
#include "EntityManager.h"
#include "KeyboardController.h"
#include "SceneManager.h"

#include "EntityManager.h"
#include "Lua\LuaInterface.h"
CIntroState::CIntroState()
{

}

CIntroState::~CIntroState()
{

}

void CIntroState::Init()
{
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	MeshBuilder::GetInstance()->GenerateQuad("INTROSTATE_BACKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("INTROSTATE_BACKGROUND")->textureID = LoadTGA("Image//IntroState//IntroState.tga");

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.f;

	IntroStateBackground = Create::Sprite2DObject("INTROSTATE_BACKGROUND", Vector3(halfWindowWidth, halfWindowHeight, 0.f), Vector3(halfWindowWidth * 2, halfWindowHeight * 2, 0.f));

	cout << "CIntroState loaded\n" << endl;

	int a, b, c, d;
	a = b = c = d = 5;
	CLuaInterface::GetInstance()->getVariableValues("TestSum", a, b, c, d);
}

void CIntroState::Update(double _dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
	{
		cout << "Loading MenuState" << endl;
		SceneManager::GetInstance()->SetActiveScene("MenuState");
	}
}

void CIntroState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.f, 4.f / 3.f, 0.1f, 10000.f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	EntityManager::GetInstance()->Render();

	GraphicsManager::GetInstance()->SetOrthographicProjection(0, Application::GetInstance().GetWindowWidth(), 0, Application::GetInstance().GetWindowHeight(), -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	EntityManager::GetInstance()->RenderUI();
}

void CIntroState::Exit()
{
	EntityManager::GetInstance()->RemoveEntity(IntroStateBackground);
	MeshBuilder::GetInstance()->RemoveMesh("INTROSTATE_BACKGROUND");
	GraphicsManager::GetInstance()->DetachCamera();
}