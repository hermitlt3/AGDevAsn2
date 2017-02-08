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

#include <iostream>
using namespace std;

SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
	
}

SceneText::SceneText(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("Start", this);
}

SceneText::~SceneText()
{
	CSpatialPartition::GetInstance()->RemoveCamera();
	CSceneGraph::GetInstance()->Destroy();

}

void SceneText::Init()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");
	
	// Tell the shader program to store these uniform locations
	currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled");
	currProg->AddUniform("colorTexture");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");
	
	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	currProg->UpdateInt("numLights", 1);
	currProg->UpdateInt("textEnabled", 0);
	
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
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//Default//ground_puzzle.tga");
	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("cube2", Color(1, 1, 1), 1);

	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);

	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//Default//ground_puzzle.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//Default//ground_puzzle.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//Default//ground_puzzle.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//Default//ground_puzzle.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//Default//ground_puzzle.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//Default//ground_puzzle.tga");
	MeshBuilder::GetInstance()->GenerateRay("laser", 10.0f);
	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH", Color(1, 0, 0), 10.f);

	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_topHIGH", "OBJ//Gameobject//Windmill_Top_HIG.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_topMED", "OBJ//Gameobject//Windmill_Top_MED.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_topLOW", "OBJ//Gameobject//Windmill_Top_LOW.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_botHIGH", "OBJ//Gameobject//Windmill_Bot_HIG.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_botMED", "OBJ//Gameobject//Windmill_Bot_MED.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_botLOW", "OBJ//Gameobject//Windmill_Bot_LOW.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_sailHIGH", "OBJ//Gameobject//Windmill_Sails_HIG.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_sailMED", "OBJ//Gameobject//Windmill_Sails_MED.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("Windmill_sailLOW", "OBJ//Gameobject//Windmill_Sails_LOW.obj");
		
	MeshBuilder::GetInstance()->GetMesh("Windmill_topHIGH")->textureID = LoadTGA("Image//mill_1.tga");;
	MeshBuilder::GetInstance()->GetMesh("Windmill_topMED")->textureID = LoadTGA("Image//mill_1.tga");;
	MeshBuilder::GetInstance()->GetMesh("Windmill_topLOW")->textureID = LoadTGA("Image//mill_1.tga");;
	MeshBuilder::GetInstance()->GetMesh("Windmill_botHIGH")->textureID = LoadTGA("Image//mill_2.tga");;
	MeshBuilder::GetInstance()->GetMesh("Windmill_botMED")->textureID = LoadTGA("Image//mill_2.tga");;
	MeshBuilder::GetInstance()->GetMesh("Windmill_botLOW")->textureID = LoadTGA("Image//mill_2.tga");;
	MeshBuilder::GetInstance()->GetMesh("Windmill_sailHIGH")->textureID = LoadTGA("Image//sails.tga");;
	MeshBuilder::GetInstance()->GetMesh("Windmill_sailMED")->textureID = LoadTGA("Image//sails.tga");;
	MeshBuilder::GetInstance()->GetMesh("Windmill_sailLOW")->textureID = LoadTGA("Image//sails.tga");;

	MeshBuilder::GetInstance()->GenerateOBJ("House_HIGH", "OBJ//Gameobject//House_HIG.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("House_MED", "OBJ//Gameobject//House_MID.obj");
	MeshBuilder::GetInstance()->GenerateOBJ("House_LOW", "OBJ//Gameobject//House_LOW.obj");

	MeshBuilder::GetInstance()->GetMesh("House_HIGH")->textureID = LoadTGA("Image//Farmhouse.tga");
	MeshBuilder::GetInstance()->GetMesh("House_MED")->textureID = LoadTGA("Image//Farmhouse.tga");
	MeshBuilder::GetInstance()->GetMesh("House_LOW")->textureID = LoadTGA("Image//Farmhouse.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("ak103","OBJ//Gameobject//ak103.obj");
	MeshBuilder::GetInstance()->GetMesh("ak103")->textureID = LoadTGA("Image//ak103.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("bullet", "OBJ//GameObject//Bullet.obj");
	MeshBuilder::GetInstance()->GetMesh("bullet")->textureID = LoadTGA("Image//Bullet.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Dummy", "OBJ//GameObject//Dummy.obj");
	MeshBuilder::GetInstance()->GetMesh("Dummy")->textureID = LoadTGA("Image//Dummy.tga");


	// Set up the Spatial Partition and pass it to the EntityManager to manage
	CSpatialPartition::GetInstance()->Init(50, 50, 10, 10);
	CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDetails(10000.0f, 40000.0f);
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());

	// Create entities into the scene
	Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball

	{
		// WINDMILL STUFFS
		GenericEntity* millBot = Create::Entity("Windmill_botHIGH", Vector3(0, 0, 0), Vector3(20, 20, 20));
		GenericEntity* millTop = Create::Entity("Windmill_topHIGH", Vector3(0, 0, 0), Vector3(20, 20, 20));
		GenericEntity* millSail = Create::Entity("Windmill_sailHIGH", Vector3(0, 0, 0), Vector3(15, 15, 15));

		millBot->SetType(INVULERABLE);
		millTop->SetType(INVULERABLE);
		millSail->SetType(INVULERABLE);
		
		millBot->SetCollider(true);
		millTop->SetCollider(true);
		millSail->SetCollider(true);

		millBot->SetAABB(Vector3(-350.f, 50.f, -350.f), Vector3(-450.f, -50.f, -450.f));

		millBot->InitLOD("Windmill_botHIGH", "Windmill_botMED", "Windmill_botLOW");
		millTop->InitLOD("Windmill_topHIGH", "Windmill_topMED", "Windmill_topLOW");
		millSail->InitLOD("Windmill_sailHIGH", "Windmill_sailMED", "Windmill_sailLOW");

		CSceneNode* baseNode = CSceneGraph::GetInstance()->AddNode(millBot);
		baseNode->ApplyTranslate(-400, -10, -400);

		CUpdateTransformation* baseMtx = new CUpdateTransformation();
		baseMtx->ApplyUpdate(0.05f, 0.0f, 1.0f, 0.0f);
		baseNode->SetUpdateTransformation(baseMtx);

		CSceneNode* childNode = baseNode->AddChild(millTop);
		childNode->ApplyTranslate(0.0f, 100.0f, 0.0f);

		CSceneNode* grandchildNode = childNode->AddChild(millSail);
		grandchildNode->ApplyTranslate(45.0f, 40.0f, 8.0f);

		CUpdateTransformation* aRotateMtx = new CUpdateTransformation();
		aRotateMtx->ApplyUpdate(0.5f, 1.0f, 0.0f, 0.0f);
		grandchildNode->SetUpdateTransformation(aRotateMtx);
	}

	{
		GenericEntity* house = Create::Entity("House_HIGH", Vector3(300, -10, -200), Vector3(5, 5, 5));
		house->InitLOD("House_HIGH", "Windmill_botMED", "Windmill_topLOW");
	}
	/*
	GenericEntity* anotherCube = Create::Entity("cube", Vector3(-20.0f, 1.1f, -20.0f));
	anotherCube->SetCollider(true);
	anotherCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	CSceneNode* anotherNode = theNode->AddChild(anotherCube);
	if (anotherNode == NULL)
	{
		cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	}
	
	GenericEntity* baseCube = Create::Asset("cube", Vector3(0.0f, 0.0f, 0.0f));
	CSceneNode* baseNode = CSceneGraph::GetInstance()->AddNode(baseCube);

	CUpdateTransformation* baseMtx = new CUpdateTransformation();
	baseMtx->ApplyUpdate(1.0f, 0.0f, 1.0f, 0.0f);
	baseNode->SetUpdateTransformation(baseMtx);

	GenericEntity* childCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	CSceneNode* childNode = baseNode->AddChild(childCube);
	childNode->ApplyTranslate(0.0f, 1.0f, 0.0f);

	GenericEntity* grandchildCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	CSceneNode* grandchildNode = childNode->AddChild(grandchildCube);
	grandchildNode->ApplyTranslate(0.0f, 0.0f, 1.0f);

	CUpdateTransformation* aRotateMtx = new CUpdateTransformation();
	aRotateMtx->ApplyUpdate(-2.0f, 0.0f, 0.0f, 1.0f);
	grandchildNode->SetUpdateTransformation(aRotateMtx);
	*/
	
	//Target example
	{
		GenericEntity* torso = Create::Asset("cube", Vector3(0.0f, 0.0f, 0.0f));
		CSceneNode* torsocube = CSceneGraph::GetInstance()->AddNode(torso);
		GenericEntity* headcube = Create::Asset("cube2", Vector3(0.0f, 0.0f, 0.0f));
		GenericEntity* handcube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
		GenericEntity* handcube2 = Create::Asset("cubeSG", Vector3(0.f, 0.f, 0.f));
		GenericEntity* legcube = Create::Asset("cube", Vector3(0.0f, 0.0f, 0.0f));
		GenericEntity* legcube2 = Create::Asset("cube", Vector3(0.f, 0.f, 0.f));
		CSceneNode* headcubeNode = torsocube->AddChild(headcube);
		headcubeNode->SetScale(0.7, 0.7, 0.7);
		headcubeNode->ApplyTranslate(0.001f, 1, 0.001f);
		CSceneNode* handcubenode = torsocube->AddChild(handcube);
		handcubenode->SetScale(0.3, 1.5, 0.3);
		handcubenode->ApplyTranslate(1.8f, -0.2f, 0.f);
		CSceneNode* handcube2Node = torsocube->AddChild(handcube2);
		handcube2Node->SetScale(0.3, 1.5, 0.3);
		handcube2Node->ApplyTranslate(-1.8f, -0.2f, 0.f);
		CSceneNode* legcubeNode = torsocube->AddChild(legcube);
		legcubeNode->SetScale(0.3, 1.5, 0.3);
		legcubeNode->ApplyTranslate(0.7f, -0.7f, 0.f);
		CSceneNode* legcube2Node = torsocube->AddChild(legcube2);
		legcube2Node->SetScale(0.3, 1.5, 0.3);
		legcube2Node->ApplyTranslate(-0.7f, -0.7f, 0.f);
	}
	
	GenericEntity* torso = Create::Asset("cube", Vector3(0.0f, 0.0f, 0.0f));
	CSceneNode* torsocube = CSceneGraph::GetInstance()->AddNode(torso);
	GenericEntity* headcube = Create::Asset("cube2", Vector3(0.0f, 0.0f, 0.0f));
	GenericEntity* handcube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	GenericEntity* handcube2 = Create::Asset("cubeSG", Vector3(0.f, 0.f, 0.f));
	GenericEntity* legcube = Create::Asset("cube", Vector3(0.0f, 0.0f, 0.0f));
	GenericEntity* legcube2 = Create::Asset("cube", Vector3(0.f, 0.f, 0.f));
	CSceneNode* headcubeNode = torsocube->AddChild(headcube);
	headcubeNode->SetScale(0.7, 0.7, 0.7);
	headcubeNode->ApplyTranslate(0, 1, 0);
	CSceneNode* handcubenode = torsocube->AddChild(handcube);
	handcubenode->SetScale(0.3, 1.5, 0.3);
	handcubenode->ApplyTranslate(1.8f, -0.2f, 0.f);
	CSceneNode* handcube2Node = torsocube->AddChild(handcube2);
	handcube2Node->SetScale(0.3, 1.5, 0.3);
	handcube2Node->ApplyTranslate(-1.8f, -0.2f, 0.f);
	CSceneNode* legcubeNode = torsocube->AddChild(legcube);
	legcubeNode->SetScale(0.3, 1.5, 0.3);
	legcubeNode->ApplyTranslate(0.7f, -0.7f, 0.f);
	CSceneNode* legcube2Node = torsocube->AddChild(legcube2);
	legcube2Node->SetScale(0.3, 1.5, 0.3);
	legcube2Node->ApplyTranslate(-0.7f, -0.7f, 0.f);

	GenericEntity* wall;
	wall = Create::Entity("cube", Vector3(5.f, 0.f, 5.f), Vector3(2.f, 20.f, 20.f));
	wall->SetCollider(true);
	wall->SetType(INVULERABLE);
	wall->SetAABB(Vector3(1.f, 10.f, 10.f), Vector3(-1.f, -10.f, -10.f));

	// Create a CEnemy instance
	theEnemy = new CEnemy();
	theEnemy->Init();

	anotherEnemy = new CMovingEnemy();
	anotherEnemy->Init();
	anotherEnemy->SetPos(Vector3(15, 0, 15));

	groundEntity = Create::Ground("GRASS_DARKGREEN");
//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));
	playerInfo->SetTerrain(groundEntity);
	theEnemy->SetTerrain(groundEntity);

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
}

void SceneText::Update(double dt)
{
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
	
	if(KeyboardController::GetInstance()->IsKeyDown('5'))
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

	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

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
	ss1 << "Player:" << playerInfo->GetPos();
	textObj[2]->SetText(ss1.str());
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();

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
	delete lights[0];
	delete lights[1];
}
