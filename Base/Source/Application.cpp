#include "Application.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "SceneText.h"
#include "OptionState.h"
#include "HighscoreState.h"
#include "AchievementState.h"
#include "Lua/LuaInterface.h"
#include "MeshBuilder.h"
#include "SpriteAnimation.h"
#include "LoadTGA.h"
#include "Vector3.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application()
: m_window_width(640),
m_window_height(480)
{
}

Application::~Application()
{
}

static int luaCreateMesh(lua_State *L)
{
	int n = lua_gettop(L);
	if (n < 3)
	{
		std::cout << "Error: createObj" << std::endl;
		lua_error(L);
		return 0;
	}
	// 1st meshName
	// 2nd objpath
	// 3rd tgapath

	const std::string &meshName = lua_tostring(L, 1);
	const std::string &obj_path = lua_tostring(L, 2);
	const std::string &tga_path = lua_tostring(L, 3);

	Mesh* tempo;
	tempo = MeshBuilder::GetInstance()->GenerateOBJ(meshName, obj_path);
	tempo->textureID = LoadTGA(tga_path.c_str());
	return 1;
}
static int luaCreateQuad(lua_State *L)
{
	int n = lua_gettop(L);
	if (n < 4)
	{
		std::cout << "Error: createQuad" << std::endl;
		lua_error(L);
		return 0;
	}
	// 1st meshName
	// 2nd color
	// 3rd length
	// 4th tgapath

	const std::string &meshName = lua_tostring(L, 1);
	Vector3 color;

	lua_pushstring(L, "x");
	lua_gettable(L, 2);
	color.x = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_pushstring(L, "y");
	lua_gettable(L, 2);
	color.y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_pushstring(L, "z");
	lua_gettable(L, 2);
	color.z = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	const float length = (float)lua_tonumber(L, 3);
	const std::string &tga_path = lua_tostring(L, 4);

	Mesh* tempo;
	tempo = MeshBuilder::GetInstance()->GenerateQuad(meshName, Color(color.x, color.y, color.z), length);
	if (tga_path != "")
		tempo->textureID = LoadTGA(tga_path.c_str());
	return 1;
}
static int luaCreateRay(lua_State *L)
{
	int n = lua_gettop(L);
	if (n < 2)
	{
		std::cout << "Error: createRay" << std::endl;
		lua_error(L);
		return 0;
	}
	// 1st meshName
	// 2nd length

	const std::string &meshName = lua_tostring(L, 1);
	const float length = (float)lua_tonumber(L, 2);

	Mesh* tempo;
	tempo = MeshBuilder::GetInstance()->GenerateRay(meshName, length);
	return 1;
}
static int luaCreateAxes(lua_State *L)
{
	int n = lua_gettop(L);
	if (n < 1)
	{
		std::cout << "Error: createAxes" << std::endl;
		lua_error(L);
		return 0;
	}
	// 1st meshName
	const std::string &meshName = lua_tostring(L, 1);
	const float length = (float)lua_tonumber(L, 2);

	Mesh* tempo;
	tempo = MeshBuilder::GetInstance()->GenerateAxes(meshName);
	return 1;
}
static int luaCreateCrossHair(lua_State *L)
{
	int n = lua_gettop(L);
	if (n < 1)
	{
		std::cout << "Error: createCrossHair" << std::endl;
		lua_error(L);
		return 0;
	}
	// 1st meshName
	const std::string &meshName = lua_tostring(L, 1);

	Mesh* tempo;
	tempo = MeshBuilder::GetInstance()->GenerateCrossHair(meshName);
	return 1;
}
static int luaCreateText(lua_State *L)
{
	int n = lua_gettop(L);
	if (n < 4)
	{
		std::cout << "Error: createText" << std::endl;
		lua_error(L);
		return 0;
	}
	// 1st meshName
	// 2nd rows
	// 3rd cols
	// 4th tgapath

	const std::string &meshName = lua_tostring(L, 1);
	const int rows = lua_tonumber(L, 2);
	const int cols = lua_tonumber(L, 3);
	const std::string &tga_path = lua_tostring(L, 4);

	Mesh* tempo;
	tempo = MeshBuilder::GetInstance()->GenerateText(meshName, rows, cols);
	if (tga_path != "")
		tempo->textureID = LoadTGA(tga_path.c_str());
	return 1;
}
static int luaCreateRing(lua_State *L)
{
	int n = lua_gettop(L);
	if (n < 6)
	{
		std::cout << "Error: createRing" << std::endl;
		lua_error(L);
		return 0;
	}
	// 1st meshName
	// 2nd color
	// 3rd slice
	// 4th outer length
	// 5th inner length
	// 6th tga

	const std::string &meshName = lua_tostring(L, 1);
	Vector3 color;

	lua_pushstring(L, "x");
	lua_gettable(L, 2);
	color.x = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_pushstring(L, "y");
	lua_gettable(L, 2);
	color.y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_pushstring(L, "z");
	lua_gettable(L, 2);
	color.z = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	const float slice = (float)lua_tonumber(L, 3);
	const float outer = (float)lua_tonumber(L, 4);
	const float inner = (float)lua_tonumber(L, 5);
	const std::string &tga_path = lua_tostring(L, 6);

	Mesh* tempo;
	tempo = MeshBuilder::GetInstance()->GenerateRing(meshName, Color(color.x, color.y, color.z), slice, outer, inner);
	if (tga_path != "")
		tempo->textureID = LoadTGA(tga_path.c_str());
	return 1;
}
static int luaCreateSphere(lua_State *L)
{
	int n = lua_gettop(L);
	if (n < 6)
	{
		std::cout << "Error: createSphere" << std::endl;
		lua_error(L);
		return 0;
	}
	// 1st meshName
	// 2nd color
	// 3rd stack
	// 4th slice
	// 5th radius
	// 6th tga

	const std::string &meshName = lua_tostring(L, 1);
	Vector3 color;

	lua_pushstring(L, "x");
	lua_gettable(L, 2);
	color.x = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_pushstring(L, "y");
	lua_gettable(L, 2);
	color.y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_pushstring(L, "z");
	lua_gettable(L, 2);
	color.z = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	const float stack = (float)lua_tonumber(L, 3);
	const float slice = (float)lua_tonumber(L, 4);
	const float radius = (float)lua_tonumber(L, 5);
	const std::string &tga_path = lua_tostring(L, 6);

	Mesh* tempo;
	tempo = MeshBuilder::GetInstance()->GenerateSphere(meshName, Color(color.x, color.y, color.z), stack, slice, radius);
	if (tga_path != "")
		tempo->textureID = LoadTGA(tga_path.c_str());
	return 1;
}
static int luaCreateCube(lua_State *L)
{
	int n = lua_gettop(L);
	if (n < 4)
	{
		std::cout << "Error: createCube" << std::endl;
		lua_error(L);
		return 0;
	}
	// 1st meshName
	// 2nd color
	// 3rd length
	// 4th tga

	const std::string &meshName = lua_tostring(L, 1);
	Vector3 color;

	lua_pushstring(L, "x");
	lua_gettable(L, 2);
	color.x = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_pushstring(L, "y");
	lua_gettable(L, 2);
	color.y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_pushstring(L, "z");
	lua_gettable(L, 2);
	color.z = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	const float length = (float)lua_tonumber(L, 3);
	const std::string &tga_path = lua_tostring(L, 4);

	Mesh* tempo;
	tempo = MeshBuilder::GetInstance()->GenerateCube(meshName, Color(color.x, color.y, color.z), length);
	if (tga_path != "")
		tempo->textureID = LoadTGA(tga_path.c_str());
	return 1;
}
static int luaCreateCone(lua_State *L)
{
	int n = lua_gettop(L);
	if (n < 6)
	{
		std::cout << "Error: createCone" << std::endl;
		lua_error(L);
		return 0;
	}
	// 1st meshName
	// 2nd color
	// 3rd numslice
	// 4th radius
	// 5th height
	// 6th tga

	const std::string &meshName = lua_tostring(L, 1);
	Vector3 color;

	lua_pushstring(L, "x");
	lua_gettable(L, 2);
	color.x = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_pushstring(L, "y");
	lua_gettable(L, 2);
	color.y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_pushstring(L, "z");
	lua_gettable(L, 2);
	color.z = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	const float numslice = (float)lua_tonumber(L, 3);
	const float radius = (float)lua_tonumber(L, 4);
	const float height = (float)lua_tonumber(L, 5);

	const std::string &tga_path = lua_tostring(L, 6);

	Mesh* tempo;
	tempo = MeshBuilder::GetInstance()->GenerateCone(meshName, Color(color.x, color.y, color.z), numslice, radius, height);
	if (tga_path != "")
		tempo->textureID = LoadTGA(tga_path.c_str());
	return 1;
}
static int luaCreateSpriteAnimation(lua_State *L)
{
	int n = lua_gettop(L);
	if (n < 4)
	{
		std::cout << "Error: createSprite" << std::endl;
		lua_error(L);
		return 0;
	}
	// 1st meshName
	// 2nd numrow
	// 3rd numcol
	// 4th tga

	const std::string &meshName = lua_tostring(L, 1);
	const float numrow = (float)lua_tonumber(L, 2);
	const float numcol = (float)lua_tonumber(L, 3);
	const std::string &tga_path = lua_tostring(L, 4);

	SpriteAnimation* tempo;
	tempo = MeshBuilder::GetInstance()->GenerateSpriteAnimation(meshName, numrow, numcol);
	if (tga_path != "")
		MeshBuilder::GetInstance()->GetMesh(meshName)->textureID = LoadTGA(tga_path.c_str());
	return 1;
}

void Application::Init()
{
	// Initialise the Lua system
	CLuaInterface::GetInstance()->Init();

	m_window_width = CLuaInterface::GetInstance()->getIntValue("width");
	m_window_height = CLuaInterface::GetInstance()->getIntValue("height");
	exitGame = false;

	CLuaInterface::GetInstance()->Run();

	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_window_width, m_window_height, "NYP Framework", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// Hide the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetMouseButtonCallback(m_window, &Application::MouseButtonCallbacks);
	glfwSetScrollCallback(m_window, &Application::MouseScrollCallbacks);

	// Init systems
	GraphicsManager::GetInstance()->Init();

	lua_register(CLuaInterface::GetInstance()->theMeshLua, "luaCreateMesh", luaCreateMesh);
	lua_register(CLuaInterface::GetInstance()->theMeshLua, "luaCreateQuad", luaCreateQuad);
	lua_register(CLuaInterface::GetInstance()->theMeshLua, "luaCreateRay", luaCreateRay);
	lua_register(CLuaInterface::GetInstance()->theMeshLua, "luaCreateAxes", luaCreateAxes);
	lua_register(CLuaInterface::GetInstance()->theMeshLua, "luaCreateCrossHair", luaCreateCrossHair);
	lua_register(CLuaInterface::GetInstance()->theMeshLua, "luaCreateText", luaCreateText);
	lua_register(CLuaInterface::GetInstance()->theMeshLua, "luaCreateRing", luaCreateRing);
	lua_register(CLuaInterface::GetInstance()->theMeshLua, "luaCreateSphere", luaCreateSphere);
	lua_register(CLuaInterface::GetInstance()->theMeshLua, "luaCreateCube", luaCreateCube);
	lua_register(CLuaInterface::GetInstance()->theMeshLua, "luaCreateCone", luaCreateCone);
	lua_register(CLuaInterface::GetInstance()->theMeshLua, "luaCreateSpriteAnimation", luaCreateSpriteAnimation);

	luaL_dofile(CLuaInterface::GetInstance()->theMeshLua, "Image//MeshLua.lua");
	lua_close(CLuaInterface::GetInstance()->theMeshLua);

	SceneManager::GetInstance()->AddScene("IntroState", new CIntroState());
	SceneManager::GetInstance()->AddScene("MenuState", new CMenuState());
	SceneManager::GetInstance()->AddScene("GameState", new SceneText());
	SceneManager::GetInstance()->AddScene("OptionState", new COptionState());
	SceneManager::GetInstance()->AddScene("HighscoreState", new CHighscoreState());
	SceneManager::GetInstance()->AddScene("AchievementState", new CAchievementState());


	SceneManager::GetInstance()->SetActiveScene("IntroState");
	AudioManager::GetInstance()->Init();
}

void Application::InitDisplay(void)
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

	currProg->UpdateInt("numLights", 0);
	currProg->UpdateInt("textEnabled", 0);
}

void Application::Run()
{
	InitDisplay();
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !exitGame)
	{
		glfwPollEvents();
		UpdateInput();
		
		if (IsKeyPressed(VK_ESCAPE))
			exitGame = true;

		SceneManager::GetInstance()->Update(m_timer.getElapsedTime());
		SceneManager::GetInstance()->Render();

		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...

        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   
		
		PostInputUpdate();
	}
	SceneManager::GetInstance()->Exit();
	AudioManager::GetInstance()->Destroy();
}

void Application::Exit()
{
	// Drop the Lua system
	CLuaInterface::GetInstance()->Drop();
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}

void Application::UpdateInput()
{
	// Update Mouse Position
	double mouse_currX, mouse_currY;
	glfwGetCursorPos(m_window, &mouse_currX, &mouse_currY);
	MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);

	// Update Keyboard Input
	for (int i = 0; i < KeyboardController::MAX_KEYS; ++i)
		KeyboardController::GetInstance()->UpdateKeyboardStatus(i, IsKeyPressed(i));
}

void Application::PostInputUpdate()
{
	// If mouse is centered, need to update the center position for next frame
	if (MouseController::GetInstance()->GetKeepMouseCentered())
	{
		double mouse_currX, mouse_currY;
		mouse_currX = m_window_width >> 1;
		mouse_currY = m_window_height >> 1;
		MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);
		glfwSetCursorPos(m_window, mouse_currX, mouse_currY);
	}

	// Call input systems to update at end of frame
	MouseController::GetInstance()->EndFrameUpdate();
	KeyboardController::GetInstance()->EndFrameUpdate();
}

void Application::MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods)
{
	// Send the callback to the mouse controller to handle
	if (action == GLFW_PRESS)
		MouseController::GetInstance()->UpdateMouseButtonPressed(button);
	else
		MouseController::GetInstance()->UpdateMouseButtonReleased(button);
}

void Application::MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset)
{
	MouseController::GetInstance()->UpdateMouseScroll(xoffset, yoffset);
}

int Application::GetWindowHeight()
{
	return m_window_height;
}

int Application::GetWindowWidth()
{
	return m_window_width;
}