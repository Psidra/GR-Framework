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
	magRounds = 50;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 50;
	// The current total number of rounds currently carried by this player
	totalRounds = 100;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 100;

	// The time between shots
	timeBetweenShots = 0.33333;
	// The elapsed time (between shots)
	elapsedTime = 0.33333;
	// Boolean flag to indicate if weapon can fire now
	bFire = false;
	// Weapon Damage 
	weaponDamage = 40;
	// boolean flag for dots
	isDots = false;
	// Player/enemy angle to rotate
	m_fRotateAngle = 0.f;
	// projectile scale
	scale.Set(0.3, 0.3, 0.3);
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
			target = rotateDirection(target, m_fRotateAngle);
			generateBullet(position, target, 8, 45);

			bFire = false;
			m_fRotateAngle += 10;
			if (bulletType == GenericEntity::PLAYER_BULLET)
			--magRounds;
		}
	}

	if (m_fRotateAngle > 360)
		m_fRotateAngle = 0;
}

// Number of bullet to create and pattern
void Bow::generateBullet(Vector3 position, Vector3 target, const int numBullet, const float angle)
{
	if (numBullet < 0)
		return;

	float totalAngle = numBullet * angle * 0.5; //half the total angle for rotation
	Vector3 temp = target;

	for (int i = 0;i < numBullet;++i)
	{
		//rotate vector
		target = rotateDirection(temp, totalAngle);
		totalAngle -= angle;

		CProjectile* aProjectile = Create::Projectile("cube",
			position,
			target.Normalized(),
			scale,
			2.0f,
			6.0f);

		aProjectile->type = bulletType;
		aProjectile->setProjectileDamage(weaponDamage / numBullet);
		aProjectile->setIsDots(isDots);
	}
}