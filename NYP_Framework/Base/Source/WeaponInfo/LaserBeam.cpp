#include "../DetectMemoryLeak.h"
#include "../Projectile/Projectile.h"
#include "LaserBeam.h"
#include "../WeaponManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../Projectile/ProjectileManager.h"

LaserBeam::LaserBeam(GenericEntity::OBJECT_TYPE _bulletType) : CWeaponInfo(_bulletType)
{
	WeaponManager::GetInstance()->addWeapon(this);
}

LaserBeam::~LaserBeam()
{
}

void LaserBeam::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 200;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 200;
	// The current total number of rounds currently carried by this player
	totalRounds = 1000;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 1000;

	// The time between shots
	timeBetweenShots = 0.01;
	// The elapsed time (between shots)
	elapsedTime = 0.01;
	// Boolean flag to indicate if weapon can fire now
	bFire = false;
	// Weapon Damage 
	m_fWeaponDamage = 7;
	// boolean flag for dots
	m_bDots = false;
	// Player/enemy angle to rotate
	m_fRotateAngle = 0.f;
	// projectile scale
	scale.Set(6.0, 0.3, 0.3);
	// projectile ricochet
	m_bRicochet = false;
	// is laserBeam
	m_bLaserBeam = true;
	//increment pos spawning of projectile
	m_fIncrement = 0;
	// projectile speed
	m_fSpeed = 0.f;
	// is active
	m_bActive = false;
}

void LaserBeam::Render()
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
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("laser"));
		modelStack.PopMatrix();
	}
	else //left side
	{
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(gunPos.x - 0.5, gunPos.y, gunPos.z - 1);
		modelStack.Rotate(rotate, 0, 0, 1);
		modelStack.Scale(-1.5, -1.5, -1.5);
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("laserLeft"));
		modelStack.PopMatrix();
	}
}

void LaserBeam::Discharge(Vector3 position, Vector3 target)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second
			generateBullet(position, target, 1);
			bFire = false;
			if (bulletType == GenericEntity::PLAYER_BULLET)
				--magRounds;

			//AudioEngine::GetInstance()->PlayASound("laser", false);
		}
	}
}

Mesh * LaserBeam::GetMesh()
{
	return MeshList::GetInstance()->GetMesh("laser");
}

void LaserBeam::generateBullet(Vector3 position, Vector3 target, const int numBullet, const float angle)
{
	if (numBullet < 0)
		return;

	//float totalAngle = numBullet * angle * 0.5; //half the total angle for rotation
	//Vector3 temp = target;

	for (int i = 0;i < numBullet;++i)
	{
		//rotate vector
		//negative angle counter clockwise positive angle clockwise
		//target = rotateDirection(temp, totalAngle);
		//totalAngle -= angle;

		CProjectile* projectile = ProjectileManager::GetInstance()->FetchProjectile();

		Mesh* mesh = MeshList::GetInstance()->GetMesh("laserBullet");
		projectile->SetProjectileMesh(mesh);
		projectile->SetIsActive(true);
		projectile->SetPosition(position + target * 3.5);
		projectile->SetDirection(target.Normalized());
		projectile->SetScale(scale);
		projectile->SetLifetime(0.05f);
		projectile->SetSpeed(0.0f);
		projectile->type = bulletType;
		projectile->setIsDots(m_bDots);
		projectile->setIsRicochet(m_bRicochet);
		projectile->setIsLaserbeam(m_bLaserBeam);
		projectile->type = bulletType;
		projectile->projectileType = CProjectile::LASER;
		//CProjectile* aProjectile = Create::Projectile("cube",
		//	position + target * 3.5,
		//	target.Normalized(),
		//	scale,
		//	0.05f,
		//	0.f);
		//
		//aProjectile->type = bulletType;
		//aProjectile->setProjectileDamage(m_fWeaponDamage / numBullet);
		//aProjectile->setIsDots(m_bDots);
		//aProjectile->setIsRicochet(m_bRicochet);
		//aProjectile->setIsLaserbeam(m_bLaserBeam);
		//aProjectile->SetIsActive(true);
	}

	//magic number (base of scale)
	for (int i = 0; i < (int)scale.x; ++i)
	{

		m_fIncrement += 1;

		CProjectile* projectile = ProjectileManager::GetInstance()->FetchProjectile();

		Mesh* mesh = MeshList::GetInstance()->GetMesh("laserBullet");
		projectile->SetProjectileMesh(mesh);
		projectile->SetIsActive(true);
		projectile->SetPosition(position + target * m_fIncrement);
		projectile->SetDirection(target.Normalized());
		projectile->SetScale(Vector3(0.3, 0.3, 0.3));
		projectile->SetLifetime(0.05f);
		projectile->SetSpeed(0.0f);
		projectile->type = bulletType;
		projectile->setProjectileDamage(m_fWeaponDamage / numBullet);
		projectile->setIsDots(m_bDots);
		projectile->setIsRicochet(m_bRicochet);
		projectile->setIsLaserbeam(false);
		projectile->type = bulletType;


		//CProjectile* aProjectile = Create::Projectile("cube",
		//	position + target * m_fIncrement,
		//	target.Normalized(),
		//	Vector3(0.3,0.3,0.3),
		//	0.05f,
		//	0.f);

		//aProjectile->type = bulletType;
		//aProjectile->setProjectileDamage(m_fWeaponDamage / numBullet);
		//aProjectile->setIsDots(m_bDots);
		//aProjectile->setIsRicochet(m_bRicochet);
		//aProjectile->setIsLaserbeam(false);
		//aProjectile->SetIsActive(true);
	}
	m_fIncrement = 0;
}
