#include "../WeaponInfo/Rifle.h"
#include "../WeaponManager.h"

Rifle::Rifle(GenericEntity::OBJECT_TYPE _bulletType) : CWeaponInfo(_bulletType)
{
	WeaponManager::GetInstance()->addWeapon(this);
}

Rifle::~Rifle()
{
}

// Initialise this instance to default values
void Rifle::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 30;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 30;
	// The current total number of rounds currently carried by this player
	totalRounds = 150;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 150;

	// The time between shots
	timeBetweenShots = 0.1111;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
	// Weapon Damage 
	weaponDamage = 3;
	// boolean flag for dots
	isDots = false;

}

void Rifle::Discharge(Vector3 position, Vector3 target)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second
			generateBullet(position, target, 5);

			bFire = false;
			if (bulletType == GenericEntity::PLAYER_BULLET)
			--magRounds;
		}
	}
}

void Rifle::generateBullet(Vector3 position, Vector3 target, const int numBullet, const float angle)
{
	if (numBullet < 0)
		return;

	//float totalAngle = numBullet * angle * 0.5; //half the total angle for rotation
	//Vector3 temp = target;
	float tempSpeed = 10.0f;
	for (int i = 0;i < numBullet;++i)
	{
		//rotate vector
		//negative angle counter clockwise positive angle clockwise
		/*target.x = temp.x * cos(Math::DegreeToRadian(totalAngle)) - temp.y * sin(Math::DegreeToRadian(totalAngle));
		target.y = temp.x * sin(Math::DegreeToRadian(totalAngle)) + temp.y * cos(Math::DegreeToRadian(totalAngle));
		totalAngle -= angle;*/

		CProjectile* aProjectile = Create::Projectile("cube",
			position,
			target.Normalized(),
			2.0f,
			tempSpeed);
		tempSpeed += 3.f;
		aProjectile->type = bulletType;
		aProjectile->setProjectileDamage(weaponDamage / numBullet);
		aProjectile->setIsDots(isDots);
	}
}
