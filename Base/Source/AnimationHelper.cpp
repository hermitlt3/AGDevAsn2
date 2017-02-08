#include "AnimationHelper.h"
#include "SpriteAnimation.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

bool AnimHelper::AddAnimation(SpriteAnimation* s)
{
	spriteList.push_back(s);
	return true;
}

void AnimHelper::UpdateAnimation(double dt)
{
	for (std::list<SpriteAnimation*>::iterator it = spriteList.begin();
		it != spriteList.end();)
	if ((*it)->m_anim->animActive)
	{
		(*it)->Update(dt);
		++it;
	}
	else
	{
		it = spriteList.erase(it);
	}
}

void AnimHelper::RenderAnimation()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	for (std::list<SpriteAnimation*>::iterator it = spriteList.begin();
		it != spriteList.end(); ++it)
	{
		modelStack.PushMatrix();
		modelStack.Translate((*it)->pos.x, (*it)->pos.y, (*it)->pos.z);
		modelStack.Rotate((*it)->rotationY, 0, 1, 0);
		modelStack.Scale((*it)->scale.x, (*it)->scale.y, (*it)->scale.z);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh((*it)->name));
		modelStack.PopMatrix(); 
	}
}

SpriteAnimation* Create::SAnimation(const std::string meshName,
	int startFrame, int endFrame, int repeat, float time, bool active,
	Vector3 pos, Vector3 scale, Vector3 playerPos)
{
	SpriteAnimation* s = dynamic_cast<SpriteAnimation*>(MeshBuilder::GetInstance()->GetMesh(meshName));
	if (s){
		s->m_anim = new Animation();
		s->m_anim->Set(startFrame, endFrame, repeat, time, active);
		s->pos = pos;
		s->scale = scale;
		s->playerPos = playerPos;
		AnimHelper::GetInstance()->AddAnimation(s);
	}
	return s;
}

AnimHelper::AnimHelper()
{
	spriteList.clear();
}

AnimHelper::~AnimHelper()
{

}