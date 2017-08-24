#include "Bow.h"
#include "../WeaponManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"

Bow::Bow(GenericEntity::OBJECT_TYPE _bulletType) : CWeaponInfo(_bulletType)
{
	WeaponManager::GetInstance()->addWeapon(this);
}

Bow::~Bow()
{
}

// Initialise this instance to default values
void Bow::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 50;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 50;
	// The current total number of rounds currently carried by this player
	totalRounds = 100;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 100;

	// The time between shots
	timeBetweenShots = 0.33333;
	// The elapsed time (between shots)
	elapsedTime = 0.33333;
	// Boolean flag to indicate if weapon can fire now
	bFire = false;
	// Weapon Damage 
	m_fWeaponDamage = 40;
	// boolean flag for dots
	m_bDots = false;
	// projectile scale
	scale.Set(0.3, 0.3, 0.3);
	// projectile ricochet
	m_bRicochet = true;
	// is laserBeam
	m_bLaserBeam = false;
	// projectile speed
	m_fSpeed = 6.0f;
	// is active
	m_bActive = false;
	// Player/enemy angle to rotate
	m_fRotateAngle = 0.f;
	// num of bullet
	m_iNumBullet = 8;
}

void Bow::Render()
{
	float rotate = Math::RadianToDegree(atan2(gunDir.y, gunDir.x));
	//std::cout << rotate << std::endl;
	if (rotate < 120 && rotate > -120) //right side
	{
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(gunPos.x + 0.5, gunPos.y, gunPos.z - 1);
		modelStack.Rotate(rotate, 0, 0, 1);
		modelStack.Scale(1, 1, 1);
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("bow"));
		modelStack.PopMatrix();
	}
	else//left side
	{
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(gunPos.x - 0.5, gunPos.y, gunPos.z - 1);
		modelStack.Rotate(rotate, 0, 0, 1);
		modelStack.Scale(-1, -1, 1);
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("bowLeft"));
		modelStack.PopMatrix();
	}
}

// Discharge this weapon
void Bow::Discharge(Vector3 position, Vector3 target)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second	
			target = rotateDirection(target, m_fRotateAngle);
			generateBullet(position, target, m_iNumBullet, 45);

			bFire = false;
			m_fRotateAngle += 10;
			if (bulletType == GenericEntity::PLAYER_BULLET)
			--magRounds;
		}
	}

	if (m_fRotateAngle > 360)
		m_fRotateAngle = 0;
}

// Number of bullet to create and pattern
void Bow::generateBullet(Vector3 position, Vector3 target, const int numBullet, const float angle)
{
	if (numBullet < 0)
		return;

	float totalAngle = numBullet * angle * 0.5; //half the total angle for rotation
	Vector3 temp = target;

	for (int i = 0;i < numBullet;++i)
	{
		//rotate vector
		target = rotateDirection(temp, totalAngle);
		totalAngle -= angle;

		CProjectile* aProjectile = Create::Projectile("cube",
			position,
			target.Normalized(),
			scale,
			2.0f,
			m_fSpeed);

		aProjectile->type = bulletType;
		aProjectile->setProjectileDamage(m_fWeaponDamage / numBullet);
		aProjectile->setIsDots(m_bDots);
		aProjectile->setIsRicochet(m_bRicochet);
		aProjectile->setIsLaserbeam(m_bLaserBeam);
	}
}