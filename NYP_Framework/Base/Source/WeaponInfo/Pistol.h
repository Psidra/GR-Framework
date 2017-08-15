#pragma once

#include "WeaponInfo.h"

class CPistol :
	public CWeaponInfo
{
public:
	CPistol(GenericEntity::OBJECT_TYPE _bulletType);
	virtual ~CPistol();

	// Initialise this instance to default values
	void Init(void);
	// Discharge this weapon
	void Discharge(Vector3 position, Vector3 target);

private:
	// Number of bullet to create
	void generateBullet(Vector3 position, Vector3 target, const int numBullet = 1, const float angle = 0.f);
};

