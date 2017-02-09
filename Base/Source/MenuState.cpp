#include <iostream>
using namespace std;

#include "MenuState.h"
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
#include "Lua\LuaInterface.h"
CMenuState::CMenuState()
{

}

CMenuState::~CMenuState()
{

}

void CMenuState::Init()
{
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.f;
	MenuStateBackground = Create::Sprite2DObject("MENUSTATE_BACKGROUND", Vector3(halfWindowWidth, halfWindowHeight, -9.f), Vector3(halfWindowWidth * 2, halfWindowHeight * 2, 0.f));
	Select = Create::Text2DObject("text", Vector3(400, 320, 0), ">", Vector3(50, 50, 2), Color(0.f, 0.0f, 0.0f));
	int i;
	i = CLuaInterface::GetInstance()->getIntValue("MenuStateSelectPos");
	menu = static_cast<MENU>(i);
}

void CMenuState::Update(double _dt)
{
	switch (menu) {
	case STARTGAME:
		Select->SetPosition(Vector3(400, 320, 0));
		break;
	case OPTIONS:
		Select->SetPosition(Vector3(420, 260, 0));
		break;
	case HIGHSCORE:
		Select->SetPosition(Vector3(410, 200, 0));
		break;
	case ACHIEVEMENTS:
		Select->SetPosition(Vector3(390, 140, 0));
		break;
	case EXIT:
		Select->SetPosition(Vector3(450, 80, 0));
		break;
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_UP)) {
		if (menu != STARTGAME) {
			menu = static_cast<MENU>(menu - 1);
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_DOWN)) {
		if (menu != EXIT) {
			menu = static_cast<MENU>(menu + 1);
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_RETURN))
	{
		switch (menu) {
		case STARTGAME:
			SceneManager::GetInstance()->SetActiveScene("GameState");
			break;
		case OPTIONS:
			SceneManager::GetInstance()->SetActiveScene("OptionState");
			break;
		case HIGHSCORE:
			SceneManager::GetInstance()->SetActiveScene("HighscoreState");
			break;
		case ACHIEVEMENTS:
			SceneManager::GetInstance()->SetActiveScene("AchievementState");
			break;
		case EXIT:
			Application::GetInstance().ExitGame();
		}
	}
}

void CMenuState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.f, 4.f / 3.f, 0.1f, 10000.f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	EntityManager::GetInstance()->Render();

	GraphicsManager::GetInstance()->SetOrthographicProjection(0, Application::GetInstance().GetWindowWidth(), 0, Application::GetInstance().GetWindowHeight(), -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	EntityManager::GetInstance()->RenderUI();
}

void CMenuState::Exit()
{
	EntityManager::GetInstance()->RemoveEntity(MenuStateBackground);
	EntityManager::GetInstance()->RemoveEntity(Select);
	CLuaInterface::GetInstance()->saveIntValue("MenuStateSelectPos", static_cast<int>(menu), true);
	GraphicsManager::GetInstance()->DetachCamera();
}