#include "RocketLauncher.h"
#include "../WeaponManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../Projectile/ProjectileManager.h"

RocketLauncher::RocketLauncher(GenericEntity::OBJECT_TYPE _bulletType) : CWeaponInfo(_bulletType)
{
	WeaponManager::GetInstance()->addWeapon(this);
}

RocketLauncher::~RocketLauncher()
{
}

// Initialise this instance to default values
void RocketLauncher::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 4;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 4;
	// The current total number of rounds currently carried by this player
	totalRounds = 20;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 20;

	// The time between shots
	timeBetweenShots = 1.5;
	// The elapsed time (between shots)
	elapsedTime = 1.5;
	// Boolean flag to indicate if weapon can fire now
	bFire = false;
	// Weapon Damage 
	m_fWeaponDamage = 50;
	// boolean flag for dots
	m_bDots = false;
	// projectile scale
	scale.Set(0.6, 0.6, 0.6);
	// projectile ricochet
	m_bRicochet = false;
	// is laserBeam
	m_bLaserBeam = false;
	// projectile speed
	m_fSpeed = 8.0f;
	// is active
	m_bActive = false;
	// Player/enemy angle to rotate
	m_fRotateAngle = 0.f;
	// num of bullet
	m_iNumBullet = 1;
}

void RocketLauncher::Render()
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
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("rocketlauncher"));
		modelStack.PopMatrix();
	}
	else//left side
	{
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(gunPos.x - 0.5, gunPos.y, gunPos.z - 1);
		modelStack.Rotate(rotate, 0, 0, 1);
		modelStack.Scale(-1, -1, 1);
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("rocketlauncherLeft"));
		modelStack.PopMatrix();
	}
}

// Discharge this weapon
void RocketLauncher::Discharge(Vector3 position, Vector3 target)
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
			//m_fRotateAngle += 10;
			if (bulletType == GenericEntity::PLAYER_BULLET)
				--magRounds;

			AudioEngine::GetInstance()->PlayASound("rifle", false);
		}
	}

	//	if (m_fRotateAngle > 360)
	//	m_fRotateAngle = 0;
}

Mesh * RocketLauncher::GetMesh()
{
	return MeshList::GetInstance()->GetMesh("rocketlauncher");
}

// Number of bullet to create and pattern
void RocketLauncher::generateBullet(Vector3 position, Vector3 target, const int numBullet, const float angle)
{
	if (numBullet < 0)
		return;

	//float totalAngle = numBullet * angle * 0.5; //half the total angle for rotation
	//Vector3 temp = target;

	for (int i = 0;i < numBullet;++i)
	{
		//rotate vector
		//target = rotateDirection(temp, totalAngle);
		//totalAngle -= angle;

		CProjectile* projectile = ProjectileManager::GetInstance()->FetchProjectile();

		Mesh* mesh = MeshList::GetInstance()->GetMesh("pistolBullet");
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
		projectile->projectileType = CProjectile::ROCKET;
		//CProjectile* aProjectile = Create::Projectile("cube",
		//	position,
		//	target.Normalized(),
		//	scale,
		//	2.0f,
		//	m_fSpeed);

		//aProjectile->type = bulletType;
		//aProjectile->setProjectileDamage(m_fWeaponDamage / numBullet);
		//aProjectile->setIsDots(m_bDots);
		//aProjectile->setIsRicochet(m_bRicochet);
		//aProjectile->setIsLaserbeam(m_bLaserBeam);
		//aProjectile->SetIsActive(true);
	}
}