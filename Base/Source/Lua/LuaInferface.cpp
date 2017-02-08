#include "LuaInterface.h"
#include <iostream>
using namespace std;

// Allocating and initalising CLuaInterface's static data member.
//The pointer is allocated but not the object's constructor;

CLuaInterface* CLuaInterface::s_instance = 0;

CLuaInterface::CLuaInterface()
: theLuaState(NULL)
{

}

CLuaInterface::~CLuaInterface()
{

}

// Initialisation of the Lua Interface Class
bool CLuaInterface::Init()
{
	bool result = false;

	// Create Lua state
	theLuaState = lua_open();

	if (theLuaState)
	{
		// Load lua axuiliary libraries
		luaL_openlibs(theLuaState);

		// Load lua script
		luaL_dofile(theLuaState, "Image//DM2240.lua");

		result = true;
	}
	return result;
}

// Run the Lua Interface Class
void CLuaInterface::Run()
{
	if (theLuaState == NULL)
		return;

	// Read the variables
	lua_getglobal(theLuaState, "title");
	const char* title = lua_tostring(theLuaState, -1);

	lua_getglobal(theLuaState, "width");
	int width = lua_tointeger(theLuaState, -1);

	lua_getglobal(theLuaState, "height");
	int height = lua_tointeger(theLuaState, -1);

	// Display on screen
	cout << title << endl;
	cout << "-------------------------------------" << endl;
	cout << "Width of screen : " << width << endl;
	cout << "Height of screen : " << height << endl;
}

void CLuaInterface::Drop()
{
	if (theLuaState)
	{
		// Close lua state
		lua_close(theLuaState);
	}
}

int CLuaInterface::getIntValue(const std::string& name)
{
	lua_getglobal(theLuaState, name.c_str());
	return lua_tointeger(theLuaState, -1);
}

float CLuaInterface::getFloatValue(const std::string& name)
{
	lua_getglobal(theLuaState, name.c_str());
	return (float)lua_tonumber(theLuaState, -1);
}

bool CLuaInterface::saveFloatValue(const std::string& name, const float& value, const bool& bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %6.4f\n", name.c_str(), value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 2, 0);
	cout << "-------------------------------------";
	return true;
}

bool CLuaInterface::saveIntValue(const std::string& name, const int& value, const bool& bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %d\n", name.c_str(), value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 2, 0);
	cout << "-------------------------------------";
	return true;
}