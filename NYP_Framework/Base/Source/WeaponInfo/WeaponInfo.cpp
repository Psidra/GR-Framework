#include "WeaponInfo.h"
#include "../Projectile/Projectile.h"
#include "../WeaponManager.h"

#include <iostream>
using namespace std;

CWeaponInfo::CWeaponInfo(GenericEntity::OBJECT_TYPE _bulletType)
	: bulletType(_bulletType)
	, magRounds(1)
	, maxMagRounds(1)
	, totalRounds(8)
	, maxTotalRounds(8)
	, timeBetweenShots(0.5)
	, elapsedTime(0.0)
	, bFire(true)
	, weaponDamage(5)
	, isDots(false)
	, isUnlocked(true)
{	
}

CWeaponInfo::~CWeaponInfo()
{
}

// Set the number of ammunition in the magazine for this player
void CWeaponInfo::SetMagRound(const int magRounds)
{
	this->magRounds = magRounds;
}

// Set the maximum number of ammunition in the magazine for this weapon
void CWeaponInfo::SetMaxMagRound(const int magRounds)
{
	this->magRounds = magRounds;
}

// The current total number of rounds currently carried by this player
void CWeaponInfo::SetTotalRound(const int totalRounds)
{
	this->totalRounds = totalRounds;
}

// The max total number of rounds currently carried by this player
void CWeaponInfo::SetMaxTotalRound(const int maxTotalRounds)
{
	this->maxTotalRounds = maxTotalRounds;
}

// Get the number of ammunition in the magazine for this player
int CWeaponInfo::GetMagRound(void) const
{
	return magRounds;
}

// Get the maximum number of ammunition in the magazine for this weapon
int CWeaponInfo::GetMaxMagRound(void) const
{
	return maxMagRounds;
}

// Get the current total number of rounds currently carried by this player
int CWeaponInfo::GetTotalRound(void) const
{
	return totalRounds;
}

// Get the max total number of rounds currently carried by this player
int CWeaponInfo::GetMaxTotalRound(void) const
{
	return maxTotalRounds;
}

// Set the time between shots
void CWeaponInfo::SetTimeBetweenShots(const double timeBetweenShots)
{
	this->timeBetweenShots = timeBetweenShots;
}

// Set the firing rate in rounds per min
void CWeaponInfo::SetFiringRate(const int firingRate)
{
	timeBetweenShots = 60.0 / (double)firingRate;	// 60 seconds divided by firing rate
}

// Set the firing flag
void CWeaponInfo::SetCanFire(const bool bFire)
{
	this->bFire = bFire;
}

// Get the time between shots
double CWeaponInfo::GetTimeBetweenShots(void) const
{
	return timeBetweenShots;
}

// Get the firing rate
int CWeaponInfo::GetFiringRate(void) const
{
	return (int)(60.0 / timeBetweenShots);	// 60 seconds divided by timeBetweenShots
}

// Get the firing flag
bool CWeaponInfo::GetCanFire(void) const
{
	return bFire;
}

// Initialise this instance to default values
void CWeaponInfo::Init(void)
{
	// The number of ammunition in a magazine for this weapon
	magRounds = 8;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 8;
	// The current total number of rounds currently carried by this player
	totalRounds = 16;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 16;

	// The time between shots
	timeBetweenShots = 0.5;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
	// Weapon Damage 
	weaponDamage = 5;
	// boolean flag for dots
	isDots = false;
}

// Update the elapsed time
void CWeaponInfo::Update(const double dt)
{
	elapsedTime += dt;
	if (elapsedTime > timeBetweenShots)
	{
		bFire = true;
		elapsedTime = 0.0;
	}
}

// Discharge this weapon
void CWeaponInfo::Discharge(Vector3 position, Vector3 target)
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
			magRounds--;
		}
	}
}

// Reload this weapon
void CWeaponInfo::Reload(void)
{
	if (magRounds < maxMagRounds)
	{
		if (maxMagRounds - magRounds <= totalRounds)
		{
			totalRounds -= maxMagRounds - magRounds;
			magRounds = maxMagRounds;
		}
		else
		{
			magRounds += totalRounds;
			totalRounds = 0;
		}
	}
}

// Add rounds
void CWeaponInfo::AddRounds(const int newRounds)
{
	if (totalRounds + newRounds > maxTotalRounds)
		totalRounds = maxTotalRounds;
	else
		totalRounds += newRounds;
}

// Print Self
void CWeaponInfo::PrintSelf(void)
{
	cout << "CWeaponInfo::PrintSelf()" << endl;
	cout << "========================" << endl;
	cout << "magRounds\t\t:\t" << magRounds << endl;
	cout << "maxMagRounds\t\t:\t" << maxMagRounds << endl;
	cout << "totalRounds\t\t:\t" << totalRounds << endl;
	cout << "maxTotalRounds\t\t:\t" << maxTotalRounds << endl;
	cout << "timeBetweenShots\t:\t" << timeBetweenShots << endl;
	cout << "elapsedTime\t\t:\t" << elapsedTime << endl;
	cout << "bFire\t\t:\t" << bFire << endl;
}

//Set weapon damage
void CWeaponInfo::setWeaponDamage(const float _weaponDamage)
{
	weaponDamage = _weaponDamage;
}

//get weapon damage
float CWeaponInfo::getWeaponDamage()
{
	return this->weaponDamage;
}

//set bool bDots
void CWeaponInfo::setIsDots(bool _isDots)
{
	isDots = _isDots;
}

//get bDots
bool CWeaponInfo::getDots()
{
	return this->isDots;
}

void CWeaponInfo::generateBullet(Vector3 position, Vector3 target, const int numBullet, const float angle)
{
	if (numBullet < 0)
		return;

	float totalAngle = numBullet * angle * 0.5; //half the total angle for rotation
	Vector3 temp = target;

	for (int i = 0;i < numBullet;++i)
	{
		//rotate vector
		if (angle >= 0)
		{	//negative angle counter clockwise positive angle clockwise
			target.x = temp.x * cos(Math::DegreeToRadian(totalAngle)) - temp.y * sin(Math::DegreeToRadian(totalAngle));
			target.y = temp.x * sin(Math::DegreeToRadian(totalAngle)) + temp.y * cos(Math::DegreeToRadian(totalAngle));
			totalAngle -= angle;
		}

		CProjectile* aProjectile = Create::Projectile("cube",
			position,
			target.Normalized(),
			2.0f,
			10.0f);
		aProjectile->type = bulletType;
	}
}