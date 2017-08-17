#include "Pistol.h"
#include "../WeaponManager.h"

Pistol::Pistol(GenericEntity::OBJECT_TYPE _bulletType) : CWeaponInfo(_bulletType)
{
	WeaponManager::GetInstance()->addWeapon(this);
}

Pistol::~Pistol()
{
}

// Initialise this instance to default values
void Pistol::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 12;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 12;
	// The current total number of rounds currently carried by this player
	totalRounds = 40;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 40;

	// The time between shots
	timeBetweenShots = 0.3333;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
	// Weapon Damage 
	weaponDamage = 5;
	// boolean flag for dots
	isDots = false;
}

// Discharge this weapon
void Pistol::Discharge(Vector3 position, Vector3 target)
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
			if (bulletType == GenericEntity::PLAYER_BULLET)
			--magRounds;
		}
	}
}

// Number of bullet to create and pattern
void Pistol::generateBullet(Vector3 position, Vector3 target, const int numBullet, const float angle)
{
	if (numBullet < 0)
		return;

	for (int i = 0;i < numBullet;++i)
	{
		CProjectile* aProjectile = Create::Projectile("sphere",
			position,
			target.Normalized(),
			2.0f,
			10.0f);
		aProjectile->type = bulletType;
		aProjectile->setProjectileDamage(weaponDamage / numBullet);
		aProjectile->setIsDots(isDots);
	}
}

