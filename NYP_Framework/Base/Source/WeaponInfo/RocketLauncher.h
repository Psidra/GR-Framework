#pragma once

#include "WeaponInfo.h"

class RocketLauncher : public CWeaponInfo
{
public:
	RocketLauncher(GenericEntity::OBJECT_TYPE _bulletType);
	virtual ~RocketLauncher();

	// Initialise this instance to default values
	void Init(void);
	//render weapon
	void Render();
	// Discharge this weapon
	void Discharge(Vector3 position, Vector3 target);
	// Get mesh
	Mesh* GetMesh();
private:
	// Number of bullet to create and pattern
	void generateBullet(Vector3 position, Vector3 target, const int numBullet = 1, const float angle = 0.f);
};
