#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

//#include "irrKlang.h"
#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"


class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class Windmill;
class CSceneNode;
class BulletfireSprite;
class GenericEntity;
class GroundEntity;
class CZombie;
class CSteve;
class ZGenerator;

class SceneText : public Scene
{	
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	//SceneText(SceneManager* _sceneMgr); // This is used to register to SceneManager

	CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	TextEntity* textObj[3];

	GenericEntity* theCube;
	CSteve* theNPC;
	CZombie* theZombie;
	Windmill *mill;
	CSceneNode* theGun;
	BulletfireSprite* fireSprite;
	ZGenerator* zGtr;

	bool startZGenerate;
	bool startSGenerate;
	bool startGame;
	bool endGame;
	int winlose;
	double gameTimer;

	//static SceneText* sInstance; // The pointer to the object that gets registered
};

#endif