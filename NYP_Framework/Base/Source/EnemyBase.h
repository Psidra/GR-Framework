#pragma once

#include "GenericEntity.h"
#include "Animation.h"
#include "Vector3.h"
#include <string>

class Mesh;
class CStrategy;
class Inventory;
class CWeaponInfo;

class EnemyBase : public GenericEntity, public CAnimation
{
public:
	EnemyBase() {};
	virtual ~EnemyBase() {};

	virtual void SetTypeOfEnemy(int _enemyType);
	virtual void Update(double dt = 0.0333f);
	virtual void Render();
	virtual void Shoot(double dt, Vector3 _pos);
	virtual void Reload(double dt);

	virtual void SetSpeed(double speed);

	virtual void SetPos(Vector3 _pos);
	virtual Vector3 GetPos();

	virtual void setInvuln(bool _invul);
	virtual bool getInvuln();

	virtual void setHealth(float _health);
	virtual float getHealth();
	virtual void editHealth(float _health);

	virtual void CollisionCheck();
	// Collision Response
	virtual void CollisionResponse(GenericEntity* ThatEntity);

	// Strategy
	virtual void ChangeStrategy(CStrategy* theNewStrategy, bool bDelete = true);

	virtual GenericEntity* FetchEnemy();

protected:
	Mesh* modelMesh;
	Vector3 position, direction, target;
	Vector3 maxBoundary, minBoundary;
	double speed;
	float health;

	CStrategy* theStrategy;
	GenericEntity** enemyAnimated;
	Inventory* enemyInventory;
	CWeaponInfo* primaryWeapon;
	int weaponIndex;
	bool isShooting;
	bool m_bLookingUp;
	double reloadElapsedTime;
	double hurtElapsedTime;
	double reloadDuration;
	bool isHurt;
	bool invulnerable;
};