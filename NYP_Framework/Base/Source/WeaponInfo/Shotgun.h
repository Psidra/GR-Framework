#ifndef _SHOTGUN_H
#define _SHOTGUN_H

#include "WeaponInfo.h"

class Shotgun : public CWeaponInfo
{
public:
	Shotgun();
	virtual ~Shotgun();

	// Initialise this instance to default values
	void Init(void);
	// Discharge this weapon
	void Discharge(Vector3 position, Vector3 target);

private:
	// generate bullet(num of bullet,dir)
	void generateBullet(Vector3 position, Vector3 target, const int numBullet = 1);
};

#endif
