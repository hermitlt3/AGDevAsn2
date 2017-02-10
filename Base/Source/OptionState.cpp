#include <iostream>
using namespace std;

#include "OptionState.h"
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
COptionState::COptionState()
{

}

COptionState::~COptionState()
{

}

void COptionState::Init()
{
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.f;
	OptionStateBackground = Create::Sprite2DObject("INTROSTATE_BACKGROUND", Vector3(halfWindowWidth, halfWindowHeight, -9.f), Vector3(halfWindowWidth * 2, halfWindowHeight * 2, 0.f));
	Select = Create::Text2DObject("text", Vector3(400, 320, 0), ">", Vector3(50, 50, 2), Color(0.f, 0.0f, 0.0f));

	option = MOVEFORWARD;
	state = CHOOSE;
}

void COptionState::Update(double _dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_UP)) {
		if (option != MOVEFORWARD) {
			option = static_cast<OPTIONS>(option - 1);
			Select->SetPosition(Vector3(400, Select->GetPosition().y + 20.f, 0));
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_DOWN)) {
		if (option != PRIMARY) {
			option = static_cast<OPTIONS>(option + 1);
			Select->SetPosition(Vector3(400, Select->GetPosition().y - 20.f, 0));
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_RETURN))
	{
		if (state == CHOOSE)
			state = SELECTED;

		else {
			if (KeyboardController::GetInstance()->IsKeyReleased(VK_BACK))
				state = CHOOSE;
		}
	}

	if (state == SELECTED)
	{
		switch (option) {
		case MOVEFORWARD:
			if (KeyboardController::GetInstance()->AnyKeyPressed()) {
				int key = KeyboardController::GetInstance()->ReturnKey();
				//CLuaInterface::GetInstance()->rewriteValue("moveForward", "hello");
				state = CHOOSE;
			}
			break;
		case MOVEBACKWARD:
			if (KeyboardController::GetInstance()->AnyKeyPressed()) {
				int key = KeyboardController::GetInstance()->ReturnKey();
				//CLuaInterface::GetInstance()->saveIntValue("moveBackward", key);
				state = CHOOSE;
			}
			break;
		case MOVELEFT:
			if (KeyboardController::GetInstance()->AnyKeyPressed()) {
				int key = KeyboardController::GetInstance()->ReturnKey();
				//CLuaInterface::GetInstance()->saveIntValue("moveLeft", key);
				state = CHOOSE;
			}
			break;
		case MOVERIGHT:
			if (KeyboardController::GetInstance()->AnyKeyPressed()) {
				int key = KeyboardController::GetInstance()->ReturnKey();
				//CLuaInterface::GetInstance()->saveIntValue("moveRight", key);
				state = CHOOSE;
			}
			break;
		case RELOAD:
			if (KeyboardController::GetInstance()->AnyKeyPressed()) {
				int key = KeyboardController::GetInstance()->ReturnKey();
				//CLuaInterface::GetInstance()->saveIntValue("reload", key);
				state = CHOOSE;
			}
			break;
		case JUMP:
			if (KeyboardController::GetInstance()->AnyKeyPressed()) {
				int key = KeyboardController::GetInstance()->ReturnKey();
				//CLuaInterface::GetInstance()->saveIntValue("jump", key);
				state = CHOOSE;
			}
			break;
		case SECONDARY:
			if (KeyboardController::GetInstance()->AnyKeyPressed()) {
				int key = KeyboardController::GetInstance()->ReturnKey();
				//CLuaInterface::GetInstance()->saveIntValue("secondary", key);
				state = CHOOSE;
			}
			break;
		case PRIMARY:
			if (KeyboardController::GetInstance()->AnyKeyPressed()) {
				int key = KeyboardController::GetInstance()->ReturnKey();
				//CLuaInterface::GetInstance()->saveIntValue("primary", key);
				state = CHOOSE;
			}
			break;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_BACK))
	{
		SceneManager::GetInstance()->SetActiveScene("MenuState");
	}
}

void COptionState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.f, 4.f / 3.f, 0.1f, 10000.f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	EntityManager::GetInstance()->Render();

	GraphicsManager::GetInstance()->SetOrthographicProjection(0, Application::GetInstance().GetWindowWidth(), 0, Application::GetInstance().GetWindowHeight(), -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	EntityManager::GetInstance()->RenderUI();
}

void COptionState::Exit()
{
	EntityManager::GetInstance()->RemoveEntity(OptionStateBackground);
	EntityManager::GetInstance()->RemoveEntity(Select);
	//CLuaInterface::GetInstance()->saveIntValue("MenuStateSelectPos", option);
	GraphicsManager::GetInstance()->DetachCamera();
}