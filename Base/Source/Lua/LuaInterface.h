#pragma once
// includes the lua headers 
#include "lua.hpp"
#include <string>

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

	int getIntValue(const std::string& name);
	float getFloatValue(const std::string& name);

	bool saveIntValue(const std::string& newName, const int& value, const bool& bOverwrite = false);
	bool saveFloatValue(const std::string& newName, const float& value, const bool& bOverwrite = false);
};