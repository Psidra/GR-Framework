#ifndef Enemies
#define Enemies

#include "GenericEntity.h"
#include "Collider\Collider.h"

#include "Strategy.h"
#include "Animation.h"

#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "PlayerInfo\PlayerInfo.h"
#include "Inventory.h"
#include "WeaponInfo\WeaponInfo.h"
class Mesh;

class CEnemy : public GenericEntity, public CAnimation
{
private:
	Mesh* modelMesh;
	Vector3 position, direction;
	Vector3 maxBoundary, minBoundary;
	Vector3 target;
	double speed;
	float health;
	
	CStrategy* theStrategy;
	GenericEntity** enemyAnimated;
	Inventory* enemyInventory;
	CWeaponInfo* primaryWeapon;
	int weaponIndex;
	bool isShooting;
	
public:
	CEnemy();
	CEnemy(Vector3 pos);
	virtual ~CEnemy();

	void Init();

	void Update(double dt = 0.0333f);
	void Render();
	void Shoot(double dt);

	void SetSpeed(double speed);
	float GetHP();
	Vector3 GetPos();
	void editHP(float _health);

	void CollisionCheck();
	// Collision Response
	void CollisionResponse(GenericEntity* ThatEntity);

	// Strategy
	void ChangeStrategy(CStrategy* theNewStrategy, bool bDelete = true);
};

namespace Create
{
	CEnemy* Enemy(Vector3 position, const string& _meshName, Vector3 scale = Vector3(1, 1, 1));
};

#endif // !Enemy