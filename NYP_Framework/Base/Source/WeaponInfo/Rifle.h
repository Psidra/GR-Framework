#ifndef _RIFLE_H
#define _RIFLE_H
#include "../WeaponInfo/WeaponInfo.h"

class Rifle : public CWeaponInfo
{
public:
	Rifle(GenericEntity::OBJECT_TYPE _bulletType);
	virtual ~Rifle();

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
