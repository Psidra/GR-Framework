#include "../DetectMemoryLeak.h"
#include "../WeaponInfo/Rifle.h"
#include "../WeaponManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../Projectile/ProjectileManager.h"

Rifle::Rifle(GenericEntity::OBJECT_TYPE _bulletType) : CWeaponInfo(_bulletType)
{
	WeaponManager::GetInstance()->addWeapon(this);
}

Rifle::~Rifle()
{
}

// Initialise this instance to default values
void Rifle::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 30;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 30;
	// The current total number of rounds currently carried by this player
	totalRounds = 150;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 150;

	// The time between shots
	timeBetweenShots = 0.05;
	// The elapsed time (between shots)
	elapsedTime = 0.05;
	// Boolean flag to indicate if weapon can fire now
	bFire = false;
	// Weapon Damage 
	m_fWeaponDamage = 3;
	// boolean flag for dots
	m_bDots = false;
	// Player/enemy angle to rotate
	m_fRotateAngle = 0.f;
	// projectile scale
	scale.Set(0.3, 0.3, 0.3);
	// projectile ricochet
	m_bRicochet = true;
	// is laserBeam
	m_bLaserBeam = false;
	// projectile speed
	m_fSpeed = 15.f;
	// is active
	m_bActive = false;
	// rotate angle
	m_fRotateAngle = 0;
	// num of bullet
	m_iNumBullet = 1;
}

void Rifle::Render()
{
	float rotate = Math::RadianToDegree(atan2(gunDir.y, gunDir.x));
	//std::cout << rotate << std::endl;
	if (rotate < 120 && rotate > -120) //right side
	{
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(gunPos.x + 0.5, gunPos.y, gunPos.z - 1);
		modelStack.Rotate(rotate, 0, 0, 1);
		modelStack.Scale(1.5, 1.5, 1.5);
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("rifle"));
		modelStack.PopMatrix();
	}
	else//left side
	{
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(gunPos.x - 0.5, gunPos.y, gunPos.z - 1);
		modelStack.Rotate(rotate, 0, 0, 1);
		modelStack.Scale(-1.5, -1.5, 1.5);
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("rifleLeft"));
		modelStack.PopMatrix();
	}
}

void Rifle::Discharge(Vector3 position, Vector3 target)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second
			generateBullet(position, target, m_iNumBullet);

			bFire = false;
			if (bulletType == GenericEntity::PLAYER_BULLET)
			--magRounds;
			AudioEngine::GetInstance()->PlayASound("rifle", false);
		}
	}
}

Mesh * Rifle::GetMesh()
{
	return MeshList::GetInstance()->GetMesh("rifle");
}

void Rifle::generateBullet(Vector3 position, Vector3 target, const int numBullet, const float angle)
{
	if (numBullet < 0)
		return;

	//float totalAngle = numBullet * angle * 0.5; //half the total angle for rotation
	//Vector3 temp = target;

	//float tempSpeed = 15.0f;
	for (int i = 0;i < numBullet;++i)
	{
		//rotate vector
		//negative angle counter clockwise positive angle clockwise
		//target = rotateDirection(temp, totalAngle);
		//totalAngle -= angle;

		CProjectile* projectile = ProjectileManager::GetInstance()->FetchProjectile();

		Mesh* mesh = MeshList::GetInstance()->GetMesh("cube");
		projectile->SetProjectileMesh(mesh);
		projectile->SetIsActive(true);
		projectile->SetPosition(position);
		projectile->SetDirection(target.Normalized());
		projectile->SetScale(scale);
		projectile->SetLifetime(2.f);
		projectile->SetSpeed(m_fSpeed);
		projectile->type = bulletType;
		projectile->setProjectileDamage(m_fWeaponDamage / numBullet);
		projectile->setIsDots(m_bDots);
		projectile->setIsRicochet(m_bRicochet);
		projectile->setIsLaserbeam(m_bLaserBeam);
		projectile->type = bulletType;
		projectile->projectileType = CProjectile::BULLET;
		/*CProjectile* aProjectile = Create::Projectile("cube",
			position,
			target.Normalized(),
			scale,
			2.0f,
			m_fSpeed);
		m_fSpeed += 3.f;
		aProjectile->type = bulletType;
		aProjectile->setProjectileDamage(m_fWeaponDamage / numBullet);
		aProjectile->setIsDots(m_bDots);
		aProjectile->setIsRicochet(m_bRicochet);
		aProjectile->setIsLaserbeam(m_bLaserBeam);
		aProjectile->SetIsActive(true);*/
	}

	m_fSpeed = 15.f;
}


