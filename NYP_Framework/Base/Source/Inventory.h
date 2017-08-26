#ifndef _INVENTORY_H
#define _INVENTORY_H



#include <map>
#include <vector>

class CWeaponInfo;
class Inventory
{
private:
	std::vector<CWeaponInfo*> weaponList;
	int m_iWeaponIndex;
public:

	Inventory();
	~Inventory();

	//add weapon to inventory
	void addWeaponToInventory(CWeaponInfo* _newObject);
	//remove weapon from inventory
	bool removeWeaponFromInventory(CWeaponInfo* _existingObject);
	//get weaponlist
	std::vector<CWeaponInfo*> getWeaponList();
	//get weapon in use
	CWeaponInfo* getPrimaryWeapon();
	//set weaponIndex
	void setWeaponIndex(const int _weaponIndex);
	//get weaponIndex
	int getWeaponIndex(void);
};


#endif
