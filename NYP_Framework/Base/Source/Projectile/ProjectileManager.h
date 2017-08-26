#ifndef _PROJECTILE_MANAGER_H
#define _PROJECTILE_MANAGER_H


#include "SingletonTemplate.h"
#include <vector>

class CProjectile;
class ProjectileManager : public Singleton<ProjectileManager> 
{
	friend Singleton<ProjectileManager>;
private:

	std::vector<CProjectile*> projectileList;

public:

	ProjectileManager();
	~ProjectileManager();

	void AddProjectile(CProjectile* projectile);
	CProjectile* FetchProjectile();


};

#endif