#include "WeaponManager.h"
#include "WeaponInfo\WeaponInfo.h"

//Add weapon to list
void WeaponManager::addWeapon(CWeaponInfo* _newWeapon)
{
	if (_newWeapon == NULL)
		return;

	//init the weapon when added
	_newWeapon->Init();
	weaponList.push_back(_newWeapon);
}

//Remove weapon from list
bool WeaponManager::removeWeapon(CWeaponInfo* _existingWeapon)
{
	// Find the entity's iterator
	std::list<CWeaponInfo*>::iterator findIter = std::find(weaponList.begin(), weaponList.end(), _existingWeapon);

	// Delete the entity if found
	if (findIter != weaponList.end())
	{
		delete *findIter;
		findIter = weaponList.erase(findIter);
		return true;
	}

	// Return false if not found
	std::cout << "Failed to delete this weapon! (weapon not found)\n";
	return false;
}

//Update all weapon from list
void WeaponManager::update(double dt)
{
	for (std::list<CWeaponInfo*>::iterator it = weaponList.begin(); it != weaponList.end(); ++it)
	{
		if ((*it) == NULL)
			continue;

		(*it)->Update(dt);
	}
}

// Constructor
WeaponManager::WeaponManager()
{
}

// Destructor
WeaponManager::~WeaponManager()
{
	for (std::list<CWeaponInfo*>::iterator it = weaponList.begin();it != weaponList.end();++it)
	{
		CWeaponInfo* temp = *it;
		delete temp;
		temp = NULL;
	}
	weaponList.clear();
}