#include "../WeaponInfo/Shotgun.h"
#include "../Projectile/Projectile.h"

Shotgun::Shotgun()
{
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
	timeBetweenShots = 0.55;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

void Shotgun::Discharge(Vector3 position, Vector3 target)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second
			
			generateBullet(position, target, 5);

			//aProjectile->SetCollider(true);
			//aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = false;
			magRounds--;
		}
	}
}

void Shotgun::generateBullet(Vector3 position, Vector3 target, const int numBullet)
{
	if (numBullet < 0)
		return;

	for (int i = 0;i < numBullet;++i)
	{
		CProjectile* aProjectile = Create::Projectile("cube",
			position,
			target.Normalized(),
			2.0f,
			10.0f);
		
		Vector3 temp = target;
		//rotate vector
		target.x = temp.x * cos(Math::DegreeToRadian(10)) - temp.y * sin(Math::DegreeToRadian(10));
		target.y = temp.x * sin(Math::DegreeToRadian(10)) + temp.y * cos(Math::DegreeToRadian(10));
	}
}