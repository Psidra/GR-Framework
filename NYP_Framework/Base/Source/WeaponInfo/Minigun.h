#ifndef _MINIGUN_H
#define _MINIGUN_H
#include "../WeaponInfo/WeaponInfo.h"

class Minigun : public CWeaponInfo
{
public:
	Minigun(GenericEntity::OBJECT_TYPE _bulletType);
	virtual ~Minigun();

	// Initialise this instance to default values
	void Init(void);
	//render weapon
	void Render();
	// Discharge this weapon
	void Discharge(Vector3 position, Vector3 target);
	// Get mesh
	Mesh* GetMesh();
private:
	// generate bullet(num of bullet,dir)
	void generateBullet(Vector3 position, Vector3 target, const int numBullet = 1, const float angle = 0.f);

};


#endif