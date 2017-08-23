#ifndef _LASER_BEAM_H
#define _LASER_BEAM_H

#include "WeaponInfo.h"

class LaserBeam :
	public CWeaponInfo
{
public:
	LaserBeam(GenericEntity::OBJECT_TYPE _bulletType);
	virtual ~LaserBeam();

	// Initialise this instance to default values
	void Init(void);
	// Discharge this weapon
	void Discharge(Vector3 position, Vector3 target);

private:
	//increment pos spawning of projectile
	float m_fIncrement;
	// Number of bullet to create and pattern
	void generateBullet(Vector3 position, Vector3 target, const int numBullet = 1, const float angle = 0.f);
};


#endif
