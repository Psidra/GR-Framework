#include "../DetectMemoryLeak.h"
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
	//for (std::vector<CProjectile*>::iterator it = projectileList.begin();
	//	it != projectileList.end(); ++it)
	//{
	//	if (!(*it)->IsActive())
	//	{
	//		if ((*it)->type != GenericEntity::PLAYER_BULLET
	//			&& (*it)->type != GenericEntity::ENEMY_BULLET)
	//			continue;

	//		(*it)->SetIsActive(true);
	//		EntityManager::GetInstance()->AddEntity(*it, true);
	//		return *it;
	//	}
	//}

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
		//projectileList.push_back(new CProjectile);
		EntityManager::GetInstance()->AddEntity(new CProjectile, true);
	}

	return FetchProjectile();
}
