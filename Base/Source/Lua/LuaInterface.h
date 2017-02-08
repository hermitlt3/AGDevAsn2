#pragma once
// includes the lua headers 
#include "lua.hpp"
#include <string>
#include "Vector3.h"

class CLuaInterface {
protected: 
	static CLuaInterface *s_instance; 
	CLuaInterface();
public: 
	static CLuaInterface *GetInstance() { 
			if (!s_instance) s_instance = new CLuaInterface; 
			return s_instance; 
	} 
	static bool DropInstance() {
			if (s_instance) { // Drop the Lua Interface Class 
				s_instance->Drop();
				delete s_instance; 
				s_instance = nullptr; 
				return true;
			} 
			return false;
} 
	virtual ~CLuaInterface();
		// Initialisation of the Lua Interface Class 
	bool Init();
		// Run the Lua Interface Class 
	void Run();
		// Drop the Lua Interface Class 
	void Drop();
		// Pointer to the Lua State 
	lua_State *theLuaState; 
	lua_State *theErrorState;
	lua_State *theMeshLua;

	int getIntValue(const char *name);
	float getFloatValue(const char *name);
	char getCharValue(const char *name);
	Vector3 getVector3Values(const char *name);

	bool getVariableValues(const char *name, int &a, int&b, int&c, int&d);

	bool saveIntValue(const char *name, const int& value, const bool& bOverwrite = false);
	bool saveFloatValue(const char *name, const float& value, const bool& bOverwrite = false);
	bool saveVector3Values(const char *name, const Vector3& value);

	float getDistanceSquareValue(const char *name, Vector3 source, Vector3 destination);
	// Extract a field from a table
	float GetField(const char *key);

	// Get error message using an error code
	void error(const char *errorCode);
};