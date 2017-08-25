#ifndef _CIRCULAR_WEAPON_H
#define _CIRCULAR_WEAPON_H

#include "WeaponInfo.h"

class CircularWeapon : public CWeaponInfo
{
public:
	CircularWeapon(GenericEntity::OBJECT_TYPE _bulletType);
	virtual ~CircularWeapon();

	// Initialise this instance to default values
	void Init(void);
	// render weapon
	void Render();
	// Discharge this weapon
	void Discharge(Vector3 position, Vector3 target);
	//getmesh
	Mesh* GetMesh();
private:
	// Number of bullet to create and pattern
	void generateBullet(Vector3 position, Vector3 target, const int numBullet = 1, const float angle = 0.f);
};


#endif // !CIRCULAR_WEAPON_H
