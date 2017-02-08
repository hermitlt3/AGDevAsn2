#pragma once
#include "irrKlang.h"
#include "WeaponInfo.h"

using namespace irrklang;

class CPlayerInfo;

class CAssaultRifle :
	public CWeaponInfo
{
public:
	CAssaultRifle();
	virtual ~CAssaultRifle();

	// Initialise this instance to default values
	void Init(void);
	// Discharge this weapon
	void Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);

	ISoundEngine* Sound_Engine;
	ISoundSource* Sound;
};