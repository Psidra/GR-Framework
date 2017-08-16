#include "Bow.h"
#include "../WeaponManager.h"

Bow::Bow(GenericEntity::OBJECT_TYPE _bulletType) : CWeaponInfo(_bulletType)
{
	WeaponManager::GetInstance()->addWeapon(this);
}

Bow::~Bow()
{
}

// Initialise this instance to default values
void Bow::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 1;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 1;
	// The current total number of rounds currently carried by this player
	totalRounds = 12;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 12;

	// The time between shots
	timeBetweenShots = 5.0;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

// Discharge this weapon
void Bow::Discharge(Vector3 position, Vector3 target)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second			
			generateBullet(position, target);

			bFire = false;
			--magRounds;
		}
	}
}

// Number of bullet to create and pattern
void Bow::generateBullet(Vector3 position, Vector3 target, const int numBullet, const float angle)
{
	if (numBullet < 0)
		return;

	for (int i = 0;i < numBullet;++i)
	{
		CProjectile* aProjectile = Create::Projectile("cube",
			position,
			target.Normalized(),
			2.0f,
			20.0f);
		aProjectile->type = bulletType;
	}
}