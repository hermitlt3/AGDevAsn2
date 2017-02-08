#include "AssaultRifle.h"
#include "../Projectile/Bullet.h"
#include "MeshBuilder.h"
#include "../EntityManager.h"

CAssaultRifle::CAssaultRifle() : Sound_Engine(NULL), Sound(NULL)
{
}


CAssaultRifle::~CAssaultRifle()
{
	if (Sound_Engine != NULL)
	{
		Sound_Engine->drop();
	}
}

// Initialise this instance to default values
void CAssaultRifle::Init(void)
{
	Sound_Engine = createIrrKlangDevice();
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 30;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 30;
	// The current total number of rounds currently carried by this player
	totalRounds = 30;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 30;

	// The time between shots
	timeBetweenShots = 0.1667;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
	Sound = Sound_Engine->addSoundSourceFromFile("Music//RifleShot.mp3");
}

// Discharge this weapon
void CAssaultRifle::Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			Vector3 _direction = (target - position).Normalized();
			// Create a laser with a laser mesh. The length is 10.0f, mesh is also set at 10.0f
			CBullet* bullet = Create::Bullet("bullet",
				position,
				_direction,
				2.0f,
				2.0f,
				100.0f,
				_source);
			bullet->SetCollider(true);
			bullet->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bullet->SetType(INVULERABLE);
			//aLaser->SetCollider(true);
			//aLaser->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			Sound_Engine->play2D(Sound);
			bFire = false;
			magRounds--;
		}
	}
}