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
class CAchievementState : public Scene
{
public:
	CAchievementState();
	~CAchievementState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	FPSCamera camera;
	SpriteEntity* AchievementStateBackground;
};