#pragma once

#include "WeaponInfo.h"

class CPistol :
	public CWeaponInfo
{
public:
	CPistol();
	virtual ~CPistol();

	// Initialise this instance to default values
	void Init(void);
	// Discharge this weapon
	void Discharge(Vector3 position, Vector3 target);

private:
	// Number of bullet to create
	virtual void generateBullet(Vector3 position, Vector3 target, const int numBullet = 1);
};

