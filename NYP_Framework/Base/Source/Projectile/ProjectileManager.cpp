#include "ProjectileManager.h"
#include "Projectile.h"

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
	for (std::vector<CProjectile*>::iterator it = projectileList.begin();
		it != projectileList.end(); ++it)
	{
		if (!(*it)->IsActive())
		{
			(*it)->SetIsActive(true);
			return (*it);
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
