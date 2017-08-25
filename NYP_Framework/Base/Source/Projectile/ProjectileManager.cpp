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

CProjectile * ProjectileManager::FetchProjectile(std::string _mesh)
{	
	for (std::list<EntityBase*>::iterator it = EntityManager::GetInstance()->getEntityList().begin();
		it != EntityManager::GetInstance()->getEntityList().end(); ++it)
	{
		if (!(*it)->IsActive())
		{
			(*it)->SetIsActive(true);
			return dynamic_cast<CProjectile*>((*it));
		}
	}

	for (int i = 0;i < 10;++i)
	{
		CProjectile* projectile = Create::Projectile(_mesh,
			Vector3(0,0,0),
			Vector3(0, 0, 0),
			Vector3(1, 1, 1),
			10.f,
			10.f);
	}

	return FetchProjectile(_mesh);
}
