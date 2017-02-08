#pragma once
#include "SingletonTemplate.h"
#include "Vector3.h"
#include <list>

class SpriteAnimation;
struct Animation;
class AnimHelper : public Singleton<AnimHelper>
{
	friend class AnimHelper;

public:
	bool AddAnimation(SpriteAnimation* s);
	void UpdateAnimation(double dt);
	void RenderAnimation();

	AnimHelper();
	~AnimHelper();

private:
	std::list<SpriteAnimation*>spriteList;
};

namespace Create
{
	SpriteAnimation* SAnimation(const std::string meshName, 
		int startFrame, int endFrame, int repeat, float time, bool active,
		Vector3 pos, Vector3 scale, Vector3 playerPOs);
}