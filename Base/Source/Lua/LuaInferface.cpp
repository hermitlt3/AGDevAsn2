#include "LuaInterface.h"
#include <iostream>
using namespace std;

// Allocating and initalising CLuaInterface's static data member.
//The pointer is allocated but not the object's constructor;
#define _CRT_SECURE_NO_WARNINGS

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
	if (theLuaState) {
		// Load lua axuiliary libraries
		luaL_openlibs(theLuaState);
		// Load lua script
		luaL_dofile(theLuaState, "Image//DM2240.lua");
		result = true;
	}

	theSavedState = lua_open();
	if (theSavedState) {
		// Load lua axuiliary libraries
		luaL_openlibs(theSavedState);
		// Load lua script
		luaL_dofile(theSavedState, "Image//SavedData.lua");
		result = true;
	}

	theMeshLua = lua_open();
	if (theMeshLua) {
		// Load lua axuiliary libraries
		luaL_openlibs(theMeshLua);
		result = true;
	}
	theErrorState = lua_open();
	if ((theLuaState) && (theErrorState) && (theSavedState) && (theMeshLua)) {
		// 2. Load lua auxiliary libraries
		luaL_openlibs(theLuaState);
		luaL_openlibs(theSavedState);
		luaL_openlibs(theMeshLua);
		// Load the error lua script
		luaL_dofile(theErrorState, "Image//errorLookup.lua");
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
	if (theSavedState)
	{
		lua_close(theSavedState);
	}
	if (theMeshLua)
	{
		//lua_close(theMeshLua);
	}
	if (theErrorState)
	{
		lua_close(theErrorState);
	}
}

int CLuaInterface::getIntValue(const char *name)
{
	lua_getglobal(theLuaState, name);
	return lua_tointeger(theLuaState, -1);
}
float CLuaInterface::getFloatValue(const char *name)
{
	lua_getglobal(theLuaState, name);
	return (float)lua_tonumber(theLuaState, -1);
}
char CLuaInterface::getCharValue(const char *name)
{
	lua_getglobal(theLuaState, name);
	
	size_t len;
	const char* cstr = lua_tolstring(theLuaState, -1, &len);
	if (len > 0)
		return cstr[0];
	else
		return ' ';
}
Vector3 CLuaInterface::getVector3Values(const char *name)
{
	lua_getglobal(theLuaState, name);
	
	lua_rawgeti(theLuaState, -1, 1);
	float x = (float)lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);

	lua_rawgeti(theLuaState, -1, 2);
	float y = (float)lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);

	lua_rawgeti(theLuaState, -1, 3);
	float z = (float)lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	
	return Vector3(x, y, z);
}

int CLuaInterface::getIntValue(lua_State* L, const char *name)
{
	lua_getglobal(L, name);
	return lua_tointeger(theLuaState, -1);
}
float CLuaInterface::getFloatValue(lua_State* L, const char *name)
{
	lua_getglobal(L, name);
	return (float)lua_tonumber(theLuaState, -1);
}
char CLuaInterface::getCharValue(lua_State* L, const char *name)
{
	lua_getglobal(L, name);

	size_t len;
	const char* cstr = lua_tolstring(L, -1, &len);
	if (len > 0)
		return cstr[0];
	else
		return ' ';
}
Vector3 CLuaInterface::getVector3Values(lua_State* L, const char *name)
{
	lua_getglobal(L, name);

	lua_rawgeti(L, -1, 1);
	float x = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_rawgeti(L, -1, 2);
	float y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_rawgeti(L, -1, 3);
	float z = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	return Vector3(x, y, z);
}

float CLuaInterface::getDistanceSquareValue(const char *name, Vector3 source, Vector3 destination)
{
	lua_getglobal(theLuaState, name);
	lua_pushnumber(theLuaState, source.x);
	lua_pushnumber(theLuaState, source.y);
	lua_pushnumber(theLuaState, source.z);
	lua_pushnumber(theLuaState, destination.x);
	lua_pushnumber(theLuaState, destination.y);
	lua_pushnumber(theLuaState, destination.z);
	lua_call(theLuaState, 6, 1);
	float distanceSquared = (float)lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	return distanceSquared;
}

bool CLuaInterface::getVariableValues(const char *name, int& a, int& b, int& c, int& d)
{
	lua_getglobal(theLuaState, name);
	lua_pushnumber(theLuaState, a);
	lua_pushnumber(theLuaState, b);
	lua_pushnumber(theLuaState, c);
	lua_pushnumber(theLuaState, d);
	lua_call(theLuaState, 4, 4);
	a = lua_tointeger(theLuaState, -1);
	lua_pop(theLuaState, 1);
	b = lua_tointeger(theLuaState, -1);
	lua_pop(theLuaState, 1);
	c = lua_tointeger(theLuaState, -1);
	lua_pop(theLuaState, 1);
	d = lua_tointeger(theLuaState, -1);
	lua_pop(theLuaState, 1);
	
	return true;
}

bool CLuaInterface:: saveFloatValue(const char *name, const float& value, const bool& bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf_s(outputString, "%s = %6.4f\n", name, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 2, 0);
	cout << "-------------------------------------";
	return true;
}

bool CLuaInterface::saveIntValue(const char *name, const int& value, const bool& bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf_s(outputString, "%s = %d\n", name, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 2, 0);
	cout << "-------------------------------------";
	return true;
}

bool CLuaInterface::saveVector3Values(const char *name, const Vector3& value, const bool& bOverwrite)
{

	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf_s(outputString, "%s =%s%6.4f,%6.4f,%6.4f%s\n", name, "{",value.x,value.y,value.z,"}");
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 2, 0);
	cout << "-------------------------------------";
	return true;
}

// Extract a field from a table
float CLuaInterface::GetField(const char *key)
{
	int result = false;

	// Check if the variables in the Lua stack belongs to a table
	if (!lua_istable(theLuaState, -1))
		error("Not a table");

	lua_pushstring(theLuaState, key);
	lua_gettable(theLuaState, -2);
	if (!lua_isnumber(theLuaState, -1))
		error("Not a number");
	result = (float)lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);  /* remove number */
	return result;
}

string CLuaInterface::GetField_s(const char *key)
{
	string result;

	// Check if the variables in the Lua stack belongs to a table
	if (!lua_istable(theLuaState, -1))
		error("Not a table");

	lua_pushstring(theLuaState, key);
	lua_gettable(theLuaState, -2);
	if (!lua_isnumber(theLuaState, -1))
		error("Not a number");
	result = lua_tostring(theLuaState, -1);
	lua_pop(theLuaState, 1);  /* remove number */
	return result;
}

// Get error message using an error code
void CLuaInterface::error(const char *errorCode)
{
	if (theErrorState == NULL)
		return;

	lua_getglobal(theErrorState, errorCode);
	const char *errorMsg = lua_tostring(theErrorState, -1);
	if (errorMsg != NULL)
		cout << errorMsg << endl;
	else
		cout << errorCode << " is not valid.\n";
}
