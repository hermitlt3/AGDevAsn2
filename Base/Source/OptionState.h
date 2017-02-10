#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class SpriteEntity;
class TextEntity;
class SceneManager;
class COptionState : public Scene
{
public:
	COptionState();
	~COptionState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	FPSCamera camera;
	SpriteEntity* OptionStateBackground;
	TextEntity* Select;

	enum OPTIONS
	{
		MOVEFORWARD = 0,
		MOVEBACKWARD,
		MOVELEFT,
		MOVERIGHT,
		RELOAD,
		JUMP,
		SECONDARY,
		PRIMARY
	};
	OPTIONS option;

	enum STATE
	{
		CHOOSE = 0,
		SELECTED
	};
	STATE state;
};