#include "LaserBlaster.h"
#include "../Projectile/Laser.h"
#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "../BulletfireSprite.h"
#include "../AudioManager.h"

CLaserBlaster::CLaserBlaster()
{
}


CLaserBlaster::~CLaserBlaster()
{
}

// Initialise this instance to default values
void CLaserBlaster::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 300;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 300;
	// The current total number of rounds currently carried by this player
	totalRounds = 300;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 300;

	// The time between shots
	timeBetweenShots = 0.1667;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

// Discharge this weapon
void CLaserBlaster::Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			Vector3 _direction = (target - position).Normalized();
			// Create a laser with a laser mesh. The length is 10.0f, mesh is also set at 10.0f
			CLaser* aLaser = Create::Laser("laser",
											position,
											_direction,
											15.0f,
											2.0f,
											700.0f,
											_source);
			aLaser->SetIsLaser(true);
			aLaser->SetCollider(false);
			//aLaser->SetCollider(true);
			//aLaser->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = false;
			magRounds--;
			//vec3df pos(target.x, target.y, target.z);
			vec3df pos(target.x - position.x, target.y - position.y, target.z - position.z);
			AudioManager::GetInstance()->Sound_Engine->play3D("Music/RifleShot.mp3", pos, false);
			//AudioManager::GetInstance()->Sound_Engine->play2D("Music/RifleShot.mp3");
		}
	}
}
