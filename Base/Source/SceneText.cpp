#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "SceneGraph\SceneGraph.h"
#include "SpatialPartition\SpatialPartition.h"

#include "SpriteAnimation.h"
#include "AnimationHelper.h"
#include "Windmill.h"
#include "Barrel.h"
#include "Sack.h"
#include "Crate.h"
#include "BulletfireSprite.h"
#include "BigTextSprite.h"
#include "Steve.h"
#include "Zombi.h"
#include "ZGenerator.h"
#include "AudioManager.h"

#include <iostream>
using namespace std;

//SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText() //: Sound_Engine(NULL), Sound(NULL)
{
	
}

//SceneText::SceneText(SceneManager* _sceneMgr)
//{
	//_sceneMgr->AddScene("Start", this);
//}

SceneText::~SceneText()
{
	CSpatialPartition::GetInstance()->RemoveCamera();
	CSceneGraph::GetInstance()->Destroy();
	//if (Sound_Engine != NULL)
	{
	//	Sound_Engine->drop();
	}
}

void SceneText::Init()
{
	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//Text//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//Text//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);

	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 0.5f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(0.5f, 0.3f, 0.3f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//Default//grass_darkgreen.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//Default//grass_lightgreen.tga");

	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("cube2", Color(1, 1, 1), 1);

	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);

	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//skybox_front.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//skybox_back.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//skybox_left.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//skybox_right.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//skybox_top.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");
	MeshBuilder::GetInstance()->GenerateRay("laser", 10.0f);
	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH", Color(1, 0, 0), 1.f);

	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_topH", "OBJ//Gameobject//Windmill_Top_H.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_topM", "OBJ//Gameobject//Windmill_Top_M.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_topL", "OBJ//Gameobject//Windmill_Top_L.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_botH", "OBJ//Gameobject//Windmill_Bot_H.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_botM", "OBJ//Gameobject//Windmill_Bot_M.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_botL", "OBJ//Gameobject//Windmill_Bot_L.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_sailH", "OBJ//Gameobject//Windmill_Sails_H.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_sailM", "OBJ//Gameobject//Windmill_Sails_M.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_sailL", "OBJ//Gameobject//Windmill_Sails_L.obj");
	MeshBuilder::GetInstance()->GetMesh("Windmill_topH")->textureID = LoadTGA("Image//mill_1.tga");;
	MeshBuilder::GetInstance()->GetMesh("Windmill_topM")->textureID = LoadTGA("Image//mill_1.tga");;
	MeshBuilder::GetInstance()->GetMesh("Windmill_topL")->textureID = LoadTGA("Image//mill_1.tga");;
	MeshBuilder::GetInstance()->GetMesh("Windmill_botH")->textureID = LoadTGA("Image//mill_2.tga");;
	MeshBuilder::GetInstance()->GetMesh("Windmill_botM")->textureID = LoadTGA("Image//mill_2.tga");;
	MeshBuilder::GetInstance()->GetMesh("Windmill_botL")->textureID = LoadTGA("Image//mill_2.tga");;
	MeshBuilder::GetInstance()->GetMesh("Windmill_sailH")->textureID = LoadTGA("Image//sails.tga");;
	MeshBuilder::GetInstance()->GetMesh("Windmill_sailM")->textureID = LoadTGA("Image//sails.tga");;
	MeshBuilder::GetInstance()->GetMesh("Windmill_sailL")->textureID = LoadTGA("Image//sails.tga");;

	MeshBuilder::GetInstance()->GenerateOBJ("House_H", "OBJ//Gameobject//House_H.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("House_M", "OBJ//Gameobject//House_M.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("House_L", "OBJ//Gameobject//House_L.obj");
	MeshBuilder::GetInstance()->GetMesh("House_H")->textureID = LoadTGA("Image//Farmhouse.tga");
	MeshBuilder::GetInstance()->GetMesh("House_M")->textureID = LoadTGA("Image//Farmhouse.tga");
	MeshBuilder::GetInstance()->GetMesh("House_L")->textureID = LoadTGA("Image//Farmhouse.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Sack_H", "OBJ//Gameobject//Sack_H.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Sack_M", "OBJ//Gameobject//Sack_M.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Sack_L", "OBJ//Gameobject//Sack_L.obj");
	MeshBuilder::GetInstance()->GetMesh("Sack_H")->textureID = LoadTGA("Image//Sack.tga");
	MeshBuilder::GetInstance()->GetMesh("Sack_M")->textureID = LoadTGA("Image//Sack.tga");
	MeshBuilder::GetInstance()->GetMesh("Sack_L")->textureID = LoadTGA("Image//Sack.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Barrel_H", "OBJ//Gameobject//Barrel_H.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Barrel_M", "OBJ//Gameobject//Barrel_M.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Barrel_L", "OBJ//Gameobject//Barrel_L.obj");
	MeshBuilder::GetInstance()->GetMesh("Barrel_H")->textureID = LoadTGA("Image//Barrel.tga");
	MeshBuilder::GetInstance()->GetMesh("Barrel_M")->textureID = LoadTGA("Image//Barrel.tga");
	MeshBuilder::GetInstance()->GetMesh("Barrel_L")->textureID = LoadTGA("Image//Barrel.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Crate_H", "OBJ//Gameobject//Crate_H.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Crate_M", "OBJ//Gameobject//Crate_M.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Crate_L", "OBJ//Gameobject//Crate_L.obj");
	MeshBuilder::GetInstance()->GetMesh("Crate_H")->textureID = LoadTGA("Image//Crate.tga");
	MeshBuilder::GetInstance()->GetMesh("Crate_M")->textureID = LoadTGA("Image//Crate.tga");
	MeshBuilder::GetInstance()->GetMesh("Crate_L")->textureID = LoadTGA("Image//Crate.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("ZHand", "OBJ//Steve//Hand.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("ZLeg", "OBJ//Steve//Leg.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("ZTorso", "OBJ//Steve//Torso.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("ZHead", "OBJ//Steve//Head.obj");
	MeshBuilder::GetInstance()->GetMesh("ZHand")->textureID = LoadTGA("Image//zombie.tga");
	MeshBuilder::GetInstance()->GetMesh("ZLeg")->textureID = LoadTGA("Image//zombie.tga");
	MeshBuilder::GetInstance()->GetMesh("ZTorso")->textureID = LoadTGA("Image//zombie.tga"); 
	MeshBuilder::GetInstance()->GetMesh("ZHead")->textureID = LoadTGA("Image//zombie.tga");
	
	MeshBuilder::GetInstance()->GenerateOBJ("SHand", "OBJ//Steve//Hand.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("SLeg", "OBJ//Steve//Leg.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("STorso", "OBJ//Steve//Torso.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("SHead", "OBJ//Steve//Head.obj");
	MeshBuilder::GetInstance()->GetMesh("SHand")->textureID = LoadTGA("Image//steve.tga");
	MeshBuilder::GetInstance()->GetMesh("SLeg")->textureID = LoadTGA("Image//steve.tga");
	MeshBuilder::GetInstance()->GetMesh("STorso")->textureID = LoadTGA("Image//steve.tga");
	MeshBuilder::GetInstance()->GetMesh("SHead")->textureID = LoadTGA("Image//steve.tga");

	MeshBuilder::GetInstance()->GenerateSpriteAnimation("GrenadeExplode", 3, 5);
	MeshBuilder::GetInstance()->GetMesh("GrenadeExplode")->textureID = LoadTGA("Image//BOOM.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Bulletfire", Color(0, 0, 0), 1);
	MeshBuilder::GetInstance()->GetMesh("Bulletfire")->textureID = LoadTGA("Image//Projectiles//gunfire.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("ZGun", "OBJ//Gameobject//M4a1_s.obj");
	MeshBuilder::GetInstance()->GetMesh("ZGun")->textureID = LoadTGA("Image//M4A1.tga");

	MeshBuilder::GetInstance()->GenerateQuad("KillZombie", Color(0, 0, 0), 1.f);
	MeshBuilder::GetInstance()->GetMesh("KillZombie")->textureID = LoadTGA("Image//Text//killnpc.tga");
	MeshBuilder::GetInstance()->GenerateQuad("KillNPC", Color(0, 0, 0), 1.f);
	MeshBuilder::GetInstance()->GetMesh("KillNPC")->textureID = LoadTGA("Image//Text//killzombie.tga");
	MeshBuilder::GetInstance()->GenerateQuad("TextOR", Color(0, 0, 0), 1.f);
	MeshBuilder::GetInstance()->GetMesh("TextOR")->textureID = LoadTGA("Image//Text//or.tga");
	MeshBuilder::GetInstance()->GenerateQuad("TextTOSTART", Color(0, 0, 0), 1.f);
	MeshBuilder::GetInstance()->GetMesh("TextTOSTART")->textureID = LoadTGA("Image//Text//tostart.tga");

	// Set up the Spatial Partition and pass it to the EntityManager to manage
	CSpatialPartition::GetInstance()->Init(125, 125, 8, 8);
	CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDetails(490000.0f, 1000000.0f);
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());

	// Create entities into the scene

	groundEntity = Create::Ground("GRASS_DARKGREEN", "GRASS_LIGHTGREEN");
//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(125.0f, 125.0f, 125.0f));
	groundEntity->SetGrids(Vector3(8.0f, 8.f, 8.0f));
	playerInfo->SetTerrain(groundEntity);

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 3; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f,1.0f,0.0f));
	}
	textObj[0]->SetText("HELLO WORLD");

	GenericEntity* house = Create::Entity("House_H", Vector3(425, -10, 425), Vector3(3, 3, 3));
	house->InitLOD("House_H", "House_M", "House_L");
	house->SetAABB(Vector3(30, 55, 75), Vector3(-25, 0, -60));
	house->SetCollider(true);

	GenericEntity* gun = Create::Entity("ZGun", Vector3(playerInfo->GetPos().x - 5, playerInfo->GetPos().y - 3, playerInfo->GetPos().z - 5), Vector3(2, 2, 2));
	theGun = CSceneGraph::GetInstance()->AddNode(gun);
	theGun->ApplyRotate(-80.f, 0, 1, 0);

	new Sack(Vector3(-235, -10, -275), 15);
	new Sack(Vector3(47, -10, 275), 15);
	new Sack(Vector3(35, -10, 275), 15);
	new Sack(Vector3(25, -10, 305), 15);
	new Sack(Vector3(23, -10, 332), 15);
	new Sack(Vector3(55, -10, 255), 15);
	new Sack(Vector3(75, -10, 245), 15);

	new Barrel(Vector3(-232, -10, -285), 15);
	new Barrel(Vector3(-220, -10, -288), 15);
	new Barrel(Vector3(-225, -10, -300), 15);
	new Barrel(Vector3(65, -10, 312.5f), 15);
	new Barrel(Vector3(50, -10, 315.f), 15);
	new Barrel(Vector3(47, -10, 304), 15);
	new Barrel(Vector3(59, -10, 298), 15);
	new Barrel(Vector3(61, -10, 322), 15);

	new Crate(Vector3(-239, -10, -305), 15);
	theZombie = new CZombie();
	theZombie->Init(Vector3(-250, -3, -250));
	theZombie->SetWayPoints(Vector3(-250, -3, -250), Vector3(-250, -3, 250), Vector3(250, -3, 250), Vector3(250, -3, -250));
	theNPC = new CSteve();
	theNPC->Init(Vector3(250, -3, 250));
	theNPC->SetWayPoints(Vector3(250, -3, 250), Vector3(250, -3, -250), Vector3(-250, -3, -250), Vector3(-250, -3, 250));

	mill = new Windmill();
	fireSprite = Create::BulletSprite("Bulletfire", Vector3(0, 0, 0), Vector3(2, 2, 2));

	Create::BigText("KillZombie", Vector3(-475, 300, -150), 90, Vector3(200, 75, 200));
	Create::BigText("KillNPC", Vector3(-475, 300, 150), 90, Vector3(200, 75, 200));
	Create::BigText("TextOR", Vector3(-475, 280, 0), 90, Vector3(50, 50, 50));
	Create::BigText("TextTOSTART", Vector3(-475, 210, 0), 90, Vector3(200, 75, 200));

	zGtr = new ZGenerator();
	zGtr->SetStart(Vector3(-450, 0, -200));
	zGtr->SetEnd(Vector3(-300, 0, 200));

	startSGenerate = false;
	startZGenerate = false;
	startGame = false;
	endGame = false;
	gameTimer = 45.0;
	winlose = -1;
}

void SceneText::Update(double dt)
{
	std::string type = "";
	AnimHelper::GetInstance()->UpdateAnimation(dt);
	zGtr->Update(dt);
	mill->Update(dt);
	if (!theZombie->GetIsDead())
		theZombie->Update(dt);
	if (!theNPC->GetIsDead())
		theNPC->Update(dt);
	
	if (theZombie->GetIsDead() && !startZGenerate) {
		zGtr->SetGenerate(true);
		type = "stevie";
		startZGenerate = true;
		theNPC->Kill();
		startGame = true;
	}
	else if (theNPC->GetIsDead() && !startSGenerate) {
		zGtr->SetGenerate(true);
		type = "zombie";
		startSGenerate = true;
		theZombie->Kill();
		startGame = true;
	}
	zGtr->GenerateZombies(playerInfo->GetPos(), type);

	// Incorrect method. But too time consuming to do the correct method for now.
	if (MouseController::GetInstance()->IsButtonDown(MouseController::LMB)) {
		if (playerInfo->GetFirstWeapon()->GetMagRound() > 0)
			fireSprite->isPressed = true;
		else
			fireSprite->isPressed = false;
	}
	else {
		fireSprite->ownTimer = fireSprite->stopTimer;
		fireSprite->isPressed = false;
	}
	if (zGtr->IsAllDead() && (theZombie->GetIsDead() || theNPC->GetIsDead()))
		endGame = true;

	if (startGame && !endGame)
	{
		gameTimer -= dt;
	}
	if (gameTimer <= 0.0)
	{
		if (!endGame)
		{
			AudioManager::GetInstance()->BGM->stop();
			AudioManager::GetInstance()->Sound_Engine->play2D("Music/Lose.mp3", false);
			endGame = true;
			gameTimer = 0.0;
			winlose = 0;
		}
	}
	else
	{
		if (endGame)
		{
			AudioManager::GetInstance()->BGM->stop();
			AudioManager::GetInstance()->Sound_Engine->play2D("Music/Win.mp3", false);
			gameTimer = 0.0;
			winlose = 1;
		}
	}
	// Update our entities
	EntityManager::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if(KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	/*if(KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	if(KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);
		*/
	if (KeyboardController::GetInstance()->IsKeyReleased('M'))
	{
		CSceneNode* theNode = CSceneGraph::GetInstance()->GetNode(1);
		Vector3 pos = theNode->GetEntity()->GetPosition();
		theNode->GetEntity()->SetPosition(Vector3(pos.x + 50.0f, pos.y, pos.z + 50.0f));
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('N'))
	{
		CSpatialPartition::GetInstance()->PrintSelf();
	}

	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		cout << "Right Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		cout << "Middle Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	}
	// <THERE>

	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo->Update(dt);

	Vector3 gunDirection = (playerInfo->GetTarget() - (playerInfo->GetPos())).Normalized();

	theGun->SetRotate(Math::RadianToDegree(atan2(gunDirection.x, gunDirection.z)), 0, 1, 0);
	theGun->ApplyRotate(Math::RadianToDegree(-gunDirection.y), 1, 0, 0);
	theGun->SetTranslate(playerInfo->GetPos() + gunDirection * 5.f + Vector3(0, -2, 0));
	fireSprite->SetPosition(playerInfo->GetPos() + gunDirection * 9.f);
	fireSprite->rotationY = 180.f + Math::RadianToDegree(atan2(gunDirection.x, gunDirection.z));


	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	float fps = (float)(1.f / dt);
	ss << "FPS: " << fps;
	textObj[1]->SetText(ss.str());

	std::ostringstream ss1;
	ss1.precision(4);
	if (winlose == -1)
		ss1 << "Time left: " << gameTimer << " Kill all of them.";
	if (winlose == 0)
		ss << "You lose. Noob.";
	if (winlose == 1)
		ss << "You win. Gratz!";
	textObj[2]->SetText(ss1.str());
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	EntityManager::GetInstance()->Render();
	glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	AnimHelper::GetInstance()->RenderAnimation();
	glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
	//delete lights[0];
	//delete lights[1];
}
