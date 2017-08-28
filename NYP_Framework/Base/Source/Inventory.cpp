#include "DetectMemoryLeak.h"
#include "Inventory.h"
#include "WeaponInfo\WeaponInfo.h"
#include "WeaponManager.h"

Inventory::Inventory() : m_iWeaponIndex(0)
{
}

Inventory::~Inventory()
{

}

void Inventory::addWeaponToInventory(CWeaponInfo* _newObject)
{
	if (_newObject == NULL)
		return;

	weaponList.push_back(_newObject);
}

bool Inventory::removeWeaponFromInventory(CWeaponInfo* _existingObject)
{
	// Find the entity's iterator
	std::vector<CWeaponInfo*>::iterator findIter = std::find(weaponList.begin(), weaponList.end(), _existingObject);
	// Remove from weaponManager
	WeaponManager::GetInstance()->removeWeapon(_existingObject);

	// Delete the entity if found
	if (findIter != weaponList.end())
	{
		//delete *findIter;
		findIter = weaponList.erase(findIter);
		return true;
	}

	// Return false if not found
	std::cout << "weapon not found\n";
	return false;
}

std::vector<CWeaponInfo*> Inventory::getWeaponList()
{
	return weaponList;
}

CWeaponInfo* Inventory::getPrimaryWeapon()
{
	if (weaponList.size() <= 0)
		return NULL;

	return weaponList[m_iWeaponIndex];
}

//set weaponIndex
void Inventory::setWeaponIndex(const int _weaponIndex)
{
	m_iWeaponIndex = _weaponIndex;
}

//get weaponIndex
int Inventory::getWeaponIndex(void)
{
	return m_iWeaponIndex;
}