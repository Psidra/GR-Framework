#ifndef _WEAPON_MANAGER_H
#define _WEAPON_MANAGER_H

#include "SingletonTemplate.h"
#include <list>

class CWeaponInfo;
class WeaponManager : public Singleton<WeaponManager>
{
	friend Singleton<WeaponManager>;
private:
	std::list<CWeaponInfo*> weaponList;

	WeaponManager();
	~WeaponManager();
public:
	//Add weapon to list
	void addWeapon(CWeaponInfo* _newWeapon);
	//Remove weapon from list
	bool removeWeapon(CWeaponInfo* _existingWeapon);
	//Update all weapon from list
	void update(double dt);
	//Render all weapon in list
	void Render();
};


#endif
