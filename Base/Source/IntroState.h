#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"

class SpriteEntity;
class SceneManager;
class CIntroState : public Scene
{
public: 
	CIntroState();
	~CIntroState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	FPSCamera camera;
	SpriteEntity* IntroStateBackground;
};