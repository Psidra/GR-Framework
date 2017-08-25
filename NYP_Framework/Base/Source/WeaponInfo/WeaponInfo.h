#pragma once

#include "Vector3.h"
#include "../GenericEntity.h"
#include "../Projectile/Projectile.h"

class CWeaponInfo
{
public:
	CWeaponInfo(GenericEntity::OBJECT_TYPE _bulletType);
	virtual ~CWeaponInfo();
protected:
	// The number of ammunition in a magazine for this weapon
	int magRounds;
	// The maximum number of ammunition for this magazine for this weapon
	int maxMagRounds;
	// The current total number of rounds currently carried by this player
	int totalRounds;
	// The max total number of rounds currently carried by this player
	int maxTotalRounds;

	// The time between shots in milliseconds
	double timeBetweenShots;
	// The elapsed time (between shots) in milliseconds
	double elapsedTime;
	// Boolean flag to indicate if weapon can fire now
	bool bFire;
	// Bullet type (player/enemy)
	GenericEntity::OBJECT_TYPE bulletType;
	// Weapon Damage 
	float m_fWeaponDamage;
	// boolean flag for dots
	bool m_bDots;
	// Player/enemy angle to rotate
	float m_fRotateAngle;
	// projectile scale
	Vector3 scale;
	// projectile ricochet
	bool m_bRicochet;
	// is laserBeam
	bool m_bLaserBeam;
	// projectile speed
	float m_fSpeed;
	// gunPos
	Vector3 gunPos;
	// gunDir
	Vector3 gunDir;
	// bool flag is gun in use
	bool m_bActive;
	// int num of bullet
	int m_iNumBullet;
public:
	// Set the number of ammunition in the magazine for this player
	virtual void SetMagRound(const int magRounds);
	// Set the maximum number of ammunition in the magazine for this weapon
	virtual void SetMaxMagRound(const int magRounds);
	// The current total number of rounds currently carried by this player
	virtual void SetTotalRound(const int totalRounds);
	// The max total number of rounds currently carried by this player
	virtual void SetMaxTotalRound(const int maxTotalRounds);

	// Get the number of ammunition in the magazine for this player
	virtual int GetMagRound(void) const;
	// Get the maximum number of ammunition in the magazine for this weapon
	virtual int GetMaxMagRound(void) const;
	// Get the current total number of rounds currently carried by this player
	virtual int GetTotalRound(void) const;
	// Get the max total number of rounds currently carried by this player
	virtual int GetMaxTotalRound(void) const;

	// Set the time between shots
	virtual void SetTimeBetweenShots(const double timeBetweenShots);
	// Set the firing rate in rounds per min
	virtual void SetFiringRate(const int firingRate);
	// Set the firing flag
	virtual void SetCanFire(const bool bFire);

	// Get the time between shots
	virtual double GetTimeBetweenShots(void) const;
	// Get the firing rate
	virtual int GetFiringRate(void) const;
	// Get the firing flag
	virtual bool GetCanFire(void) const;

	// Initialise this instance to default values
	virtual void Init(void);
	// Update the elapsed time
	void Update(const double dt);
	// render the weapon
	virtual void Render() = 0;
	// Discharge this weapon
	virtual void Discharge(Vector3 position, Vector3 target) = 0;
	// Reload this weapon
	void Reload(void);
	// Add rounds
	void AddRounds(const int newRounds);

	// Print Self
	void PrintSelf(void);

	//Set weapon damage
	void setWeaponDamage(const float _weaponDamage);
	//get weapon damage
	float getWeaponDamage();
	//set bool bDots
	void setIsDots(bool _isDots);
	//get bDots
	bool getDots();
	//set speed
	void setSpeed(float _speed);
	//get speed
	float getSpeed();
	//set gun pos
	void setGunPos(Vector3 _gunPos);
	//get gun pos
	Vector3 getGunPos();
	//set gun dir
	void setGunDir(Vector3 _gunDir);
	//get gun dir
	Vector3 getGunDir();
	//set gun dir
	void setIsActive(bool _isActive);
	//get gun dir
	bool getIsActive();
	//set angle
	void setAngle(float _angle);
	//get angle
	float getAngle();
	//set number of bullets
	void setNumBullet(int _numBullet);
	//get number of bullets
	int getNumBullet();
	// Get mesh
	virtual Mesh* GetMesh();
protected:
	// Number of bullet to create
	virtual void generateBullet(Vector3 position, Vector3 target, const int numBullet, const float angle) = 0;
	// bullet pattern(using angle) by rotating dir
	Vector3 rotateDirection(Vector3 dir, float angle = 10.f);
};
