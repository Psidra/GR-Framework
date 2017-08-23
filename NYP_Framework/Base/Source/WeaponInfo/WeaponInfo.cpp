#include "WeaponInfo.h"
#include "../Projectile/Projectile.h"
#include "../WeaponManager.h"
#include "../PlayerInfo/PlayerInfo.h"

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
	, m_fWeaponDamage(5)
	, m_bDots(false)
	, m_bRicochet(true)
	, m_bActive(false)
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
	m_fWeaponDamage = 5;
	// boolean flag for dots
	m_bDots = false;
	// Player/enemy angle to rotate
	m_fRotateAngle = 10.f;
	// projectile scale
	scale.Set(0.3, 0.3, 0.3);
	// projectile ricochet
	m_bRicochet = true;
	// is laserBeam
	m_bLaserBeam = false;
	// projectile speed
	m_fSpeed = 10.f;
	// is active
	m_bActive = false;
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

//render
void CWeaponInfo::Render()
{
}

// Discharge this weapon
void CWeaponInfo::Discharge(Vector3 position, Vector3 target)
{
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
	m_fWeaponDamage = _weaponDamage;
}

//get weapon damage
float CWeaponInfo::getWeaponDamage()
{
	return this->m_fWeaponDamage;
}

//set bool bDots
void CWeaponInfo::setIsDots(bool _isDots)
{
	m_bDots = _isDots;
}

//get bDots
bool CWeaponInfo::getDots()
{
	return this->m_bDots;
}

void CWeaponInfo::setSpeed(float _speed)
{
	m_fSpeed = _speed;
}

float CWeaponInfo::getSpeed()
{
	return m_fSpeed;
}

void CWeaponInfo::setGunPos(Vector3 _gunPos)
{
	gunPos = _gunPos;
}

Vector3 CWeaponInfo::getGunPos()
{
	return gunPos;
}

void CWeaponInfo::setGunDir(Vector3 _gunDir)
{
	gunDir = _gunDir;
}

Vector3 CWeaponInfo::getGunDir()
{
	return gunDir;
}

void CWeaponInfo::setIsActive(bool _isActive)
{
	m_bActive = _isActive;
}

bool CWeaponInfo::getIsActive()
{
	return m_bActive;
}

void CWeaponInfo::generateBullet(Vector3 position, Vector3 target, const int numBullet, const float angle)
{
}

Vector3 CWeaponInfo::rotateDirection(Vector3 dir, float angle)
{
	Vector3 temp = dir;
	dir.x = temp.x * cos(Math::DegreeToRadian(angle)) - temp.y * sin(Math::DegreeToRadian(angle));
	dir.y = temp.x * sin(Math::DegreeToRadian(angle)) + temp.y * cos(Math::DegreeToRadian(angle));

	return dir;
}
