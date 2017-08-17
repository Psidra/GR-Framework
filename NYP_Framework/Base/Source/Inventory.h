#ifndef _INVENTORY_H
#define _INVENTORY_H



#include <map>
#include <vector>

class CWeaponInfo;
class Inventory
{
private:
	std::vector<CWeaponInfo*> weaponList;
public:

	Inventory();
	~Inventory();

	void addWeaponToInventory(CWeaponInfo* _newObject);
	bool removeWeaponFromInventory(CWeaponInfo* _existingObject);
	std::vector<CWeaponInfo*> getWeaponList();
};


#endif
