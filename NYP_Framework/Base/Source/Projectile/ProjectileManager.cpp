#include "ProjectileManager.h"
#include "Projectile.h"
#include "../EntityManager.h"
#include "../GenericEntity.h"

ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
}

void ProjectileManager::AddProjectile(CProjectile * projectile)
{
	projectileList.push_back(projectile);
}

CProjectile * ProjectileManager::FetchProjectile()
{	
	for (std::list<EntityBase*>::iterator it = EntityManager::GetInstance()->getCollisionList().begin();
		it != EntityManager::GetInstance()->getCollisionList().end(); ++it)
	{
		EntityBase* entity = (EntityBase*)(*it);
		if (!entity->IsActive())
		{
			if (dynamic_cast<GenericEntity*>(entity)->type != GenericEntity::PLAYER_BULLET
				&& dynamic_cast<GenericEntity*>(entity)->type != GenericEntity::ENEMY_BULLET)
				continue;

			entity->SetIsActive(true);
			return dynamic_cast<CProjectile*>(entity);
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		EntityManager::GetInstance()->AddEntity(new CProjectile),true;
		//CProjectile* projectile = Create::Projectile("cube",
		//	Vector3(0, 0, 0),
		//	Vector3(0, 0, 0),
		//	Vector3(1, 1, 1),
		//	10.f,
		//	10.f);
	}

	return FetchProjectile();
}
