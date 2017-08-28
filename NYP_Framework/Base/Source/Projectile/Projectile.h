#pragma once
#include "Vector3.h"
#include "Collider\Collider.h"
#include "../GenericEntity.h"

class Mesh;
class CPlayerInfo;

class CProjectile : public GenericEntity
{
public:
	CProjectile(void);
	CProjectile(Mesh* _modelMesh);
	~CProjectile(void);
public:
	enum PROJECTILE_TYPE
	{
		NONE,
		BULLET,
		LASER,
		ROCKET,
	} projectileType;


	// Activate the projectile. true == active, false == inactive
	void SetStatus(const bool m_bStatus);
	// get status of the projectile. true == active, false == inactive
	bool GetStatus(void) const;
	// Set the position and direction of the projectile
	void Set(	Vector3 theNewPosition, 
				Vector3 theNewDirection, 
				const float m_fLifetime, 
				const float m_fSpeed);
	void SetDirection(Vector3 theNewDirection);
	// Get the direction of the projectile
	Vector3 GetDirection(void);
	// Set the lifetime of the projectile
	void SetLifetime(const float m_fLifetime);
	// Get the lifetime of the projectile
	float GetLifetime(void) const;
	// Set the speed of the projectile
	void SetSpeed(const float m_fSpeed);
	// Get the speed of the projectile
	float GetSpeed(void) const;
	// get projectile damage
	float getProjectileDamage();
	//set projectile damage
	void setProjectileDamage(const float pDamage);
	//get isDots
	bool getIsDots();
	//set isDots
	void setIsDots(bool _isDots);
	// get m_bProjectileRicochet
	bool getIsRicochet();
	//set m_bProjectileRicochet
	void setIsRicochet(bool _isRicochet);
	// get m_bProjectileLaserBeam
	bool getIsLaserbeam();
	//set m_bProjectileLaserBeam
	void setIsLaserbeam(bool _isLaserBeam);
	// Update the status of this projectile
	virtual void Update(double dt = 0.0333f);
	// Render this projectile
	virtual void Render(void);
	// Collision Response
	void CollisionResponse(GenericEntity* ThatEntity);
	//set projectile's mesh
	void SetProjectileMesh(Mesh* _mesh);
	//projectile spilt
	void ProjectileSpilt(int _numProjectile, float _angle, bool _ricochet = true);
protected:
	// The model mesh for this projectile
	Mesh* modelMesh;
	// Boolean flag to indicate if this projectile is active. If not active, then do not compute/update
	bool m_bStatus;
	// Remaining lifetime in seconds
	float m_fLifetime;
	// Speed of this projectile in unit / sec
	float m_fSpeed;
	// The direction of the projectile
	Vector3 theDirection;
	// projectile damage
	float projectileDamage;
	// bool flag to indicate if there is DOTS
	bool isDots;
	// bool flag if ricochet
	bool m_bProjectileRicochet;
	// bool flag is laserbeam
	bool m_bProjectileLaserBeam;
};

namespace Create
{
	CProjectile* Projectile(const std::string& _meshName, 
							const Vector3& _position, 
							const Vector3& _direction,
							const Vector3& _scale,
							const float m_fLifetime, 
							const float m_fSpeed);
};

