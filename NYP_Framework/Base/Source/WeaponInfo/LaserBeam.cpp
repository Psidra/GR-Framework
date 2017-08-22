#include "../Projectile/Projectile.h"
#include "LaserBeam.h"
#include "../WeaponManager.h"

LaserBeam::LaserBeam(GenericEntity::OBJECT_TYPE _bulletType) : CWeaponInfo(_bulletType)
{
	WeaponManager::GetInstance()->addWeapon(this);
}

LaserBeam::~LaserBeam()
{
}

void LaserBeam::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 1000;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 1000;
	// The current total number of rounds currently carried by this player
	totalRounds = 10000;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 10000;

	// The time between shots
	timeBetweenShots = 0.01;
	// The elapsed time (between shots)
	elapsedTime = 0.01;
	// Boolean flag to indicate if weapon can fire now
	bFire = false;
	// Weapon Damage 
	m_fWeaponDamage = 5;
	// boolean flag for dots
	m_bDots = false;
	// Player/enemy angle to rotate
	m_fRotateAngle = 0.f;
	// projectile scale
	scale.Set(0.3, 0.3, 0.3);
	// projectile ricochet
	m_bRicochet = true;
	// is laserBeam
	m_bLaserBeam = true;
}

void LaserBeam::Discharge(Vector3 position, Vector3 target)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second
			generateBullet(position, target, 1);

			bFire = false;
			if (bulletType == GenericEntity::PLAYER_BULLET)
				--magRounds;
		}
	}
}

void LaserBeam::generateBullet(Vector3 position, Vector3 target, const int numBullet, const float angle)
{
	if (numBullet < 0)
		return;

	//float totalAngle = numBullet * angle * 0.5; //half the total angle for rotation
	//Vector3 temp = target;

	for (int i = 0;i < numBullet;++i)
	{
		//rotate vector
		//negative angle counter clockwise positive angle clockwise
		//target = rotateDirection(temp, totalAngle);
		//totalAngle -= angle;

		CProjectile* aProjectile = Create::Projectile("cube",
			position,
			target.Normalized(),
			scale,
			2.0f,
			0.f);

		aProjectile->type = bulletType;
		aProjectile->setProjectileDamage(m_fWeaponDamage / numBullet);
		aProjectile->setIsDots(m_bDots);
		aProjectile->setIsRicochet(m_bRicochet);
		aProjectile->setIsLaserbeam(m_bLaserBeam);
	}
}
