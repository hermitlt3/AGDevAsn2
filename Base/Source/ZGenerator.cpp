#include "ZGenerator.h"
#include "Zombi.h"
#include "Steve.h"
#include "AudioManager.h"

ZGenerator::ZGenerator() :
doGenerate(false),
areaStart(Vector3(-5 ,0, -5)),
areaEnd(Vector3(5, 0, 5))
{
	for (int i = 0; i < 5; ++i)
	{
		zoms[i] = nullptr;
		stevs[i] = nullptr;
	}
}

ZGenerator::~ZGenerator()
{

}

void ZGenerator::Update(double dt)
{
	for (int i = 0; i < 5; ++i)
	{
		if (zoms[i])
			zoms[i]->Update(dt);
		if (stevs[i])
			stevs[i]->Update(dt);
		if (zoms[i] && zoms[i]->GetIsDead())
			zoms[i] = nullptr;
		if (stevs[i] && stevs[i]->GetIsDead())
			stevs[i] = nullptr;
	}
}

void ZGenerator::GenerateZombies(Vector3 target, std::string type)
{
	if (doGenerate)
	{
		/*for (int i = 0; i < 5; ++i)
		{
			Vector3 a;
			a = Vector3(Math::RandFloatMinMax(areaStart.x, areaEnd.x), -3, Math::RandFloatMinMax(areaStart.z, areaEnd.z));
			if (type == "zombie")
			{
				CZombie *zomzom = new CZombie();
				zomzom->Init(a);
				zomzom->SetTarget(target);
				if (i == 4)
				{
					vec3df va(a.x, a.y, a.z);
					va = va.normalize() * 2.f;
					AudioManager::GetInstance()->Sound_Engine->play3D("Music/ZombieMoan.mp3", va, false);
				}
				zoms[i] = zomzom;
			}
			else if (type == "stevie")
			{
				CSteve *stevie = new CSteve();
				stevie->Init(a);
				stevie->SetTarget(target);
				if (i == 4)
				{
					vec3df va(a.x, a.y, a.z);
					va = va.normalize();
					AudioManager::GetInstance()->Sound_Engine->play3D("Music/NPCHi.mp3", va, false);
				}
				stevs[i] = stevie;
			}
		}
		doGenerate = false;*/
	}
}

bool ZGenerator::IsAllDead()
{
	for (int i = 0; i < 5; ++i)
	{
		if (zoms[i])
			return false;
		if (stevs[i])
			return false;
	}
	return true;
}