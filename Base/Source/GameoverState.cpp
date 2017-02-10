#include <iostream>
using namespace std;

#include "GameoverState.h"
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

GameoverState::GameoverState()
{

}

GameoverState::~GameoverState()
{

}

void GameoverState::Init()
{
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.f;

	GameOverBackground = Create::Sprite2DObject("GAMEOVER_BACKGROUND", Vector3(halfWindowWidth, halfWindowHeight, 0.f), Vector3(halfWindowWidth * 2, halfWindowHeight * 2, 0.f));

	cout << "GameoverState loaded\n" << endl;
}

void GameoverState::Update(double _dt)
{

}

void GameoverState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.f, 4.f / 3.f, 0.1f, 10000.f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	EntityManager::GetInstance()->Render();

	GraphicsManager::GetInstance()->SetOrthographicProjection(0, Application::GetInstance().GetWindowWidth(), 0, Application::GetInstance().GetWindowHeight(), -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	EntityManager::GetInstance()->RenderUI();
}

void GameoverState::Exit()
{
	EntityManager::GetInstance()->RemoveEntity(GameOverBackground);
	GraphicsManager::GetInstance()->DetachCamera();
}