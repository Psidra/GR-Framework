#ifndef _BOW_H
#define _BOW_H

#include "WeaponInfo.h"

class Bow : public CWeaponInfo
{
public:
	Bow(GenericEntity::OBJECT_TYPE _bulletType);
	virtual ~Bow();

	// Initialise this instance to default values
	void Init(void);
	// Discharge this weapon
	void Discharge(Vector3 position, Vector3 target);

private:
	// Number of bullet to create and pattern
	void generateBullet(Vector3 position, Vector3 target, const int numBullet = 1, const float angle = 0.f);
};


#endif
