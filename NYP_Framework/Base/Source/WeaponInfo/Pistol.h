#pragma once

#include "WeaponInfo.h"

class MeshBuilder;
class Pistol :
	public CWeaponInfo
{
public:
	Pistol(GenericEntity::OBJECT_TYPE _bulletType);
	virtual ~Pistol();

	// Initialise this instance to default values
	void Init(void);
	//render weapon
	void Render();
	//Reolad gun
	void Reload();
	// Discharge this weapon
	void Discharge(Vector3 position, Vector3 target);
	// Get mesh
	Mesh* GetMesh();

private:
	// Number of bullet to create and pattern
	void generateBullet(Vector3 position, Vector3 target, const int numBullet = 1, const float angle = 0.f);
	//void generateEnemyBullet(Vector3 position, Vector3 target, const int numBullet = 1, const float angle = 0.f);
};

