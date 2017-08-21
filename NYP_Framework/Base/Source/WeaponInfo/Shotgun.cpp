#include "../WeaponInfo/Shotgun.h"
#include "../WeaponManager.h"

Shotgun::Shotgun(GenericEntity::OBJECT_TYPE _bulletType) : CWeaponInfo(_bulletType)
{
	WeaponManager::GetInstance()->addWeapon(this);
}

Shotgun::~Shotgun()
{
}

// Initialise this instance to default values
void Shotgun::Init(void)
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
	timeBetweenShots = 1.0;
	// The elapsed time (between shots)
	elapsedTime = 1.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = false;
	// Weapon Damage 
	m_fWeaponDamage = 20;
	// boolean flag for dots
	m_bDots = false;
	// Player/enemy angle to rotate
	m_fRotateAngle = 0.f;
	// projectile scale
	scale.Set(0.3, 0.3, 0.3);
}

void Shotgun::Discharge(Vector3 position, Vector3 target)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			//GenerateBullet takes in player/enemy pos, dir player/enemy facing, no. of bullets, angle of bullet being shoot out
			//E.g. 5 bullets each rotation by 10 degree to create a pattern
			generateBullet(position, target, 5, 10);

			bFire = false;
			if(bulletType == GenericEntity::PLAYER_BULLET)
			--magRounds;
		}
	}
}

void Shotgun::generateBullet(Vector3 position, Vector3 target, const int numBullet, const float angle)
{
	if (numBullet < 0)
		return;

	float totalAngle = numBullet * angle * 0.5; //half the total angle for rotation
	Vector3 temp = target;

	for (int i = 0;i < numBullet;++i)
	{
		//rotate vector
		//negative angle counter clockwise positive angle clockwise
		//target.x = temp.x * cos(Math::DegreeToRadian(totalAngle)) - temp.y * sin(Math::DegreeToRadian(totalAngle));
		//target.y = temp.x * sin(Math::DegreeToRadian(totalAngle)) + temp.y * cos(Math::DegreeToRadian(totalAngle));
		target = rotateDirection(temp, totalAngle);
		totalAngle -= angle;

		CProjectile* aProjectile = Create::Projectile("cube",
			position,
			target.Normalized(),
			scale,
			2.0f,
			10.0f);
		aProjectile->type = bulletType;
		aProjectile->setProjectileDamage(m_fWeaponDamage / numBullet);
		aProjectile->setIsDots(m_bDots);
	}
}