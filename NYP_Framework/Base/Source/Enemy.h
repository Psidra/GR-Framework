#ifndef Enemies
#define Enemies

#include "GenericEntity.h"
#include "Collider\Collider.h"

#include "Strategy.h"
#include "Animation.h"

#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "PlayerInfo\PlayerInfo.h"
#include "Inventory.h"

class Mesh;

class CEnemy : public GenericEntity, public CAnimation
{
protected:
	Mesh* modelMesh;
	Vector3 position;
	Vector3 maxBoundary, minBoundary;
	Vector3 target;
	double speed;
	float health;
	
	CStrategy* theStrategy;
	GenericEntity** enemyAnimated;
	
public:
	CEnemy();
	CEnemy(Vector3 pos);
	virtual ~CEnemy();

	void Init();

	void Update(double dt = 0.0333f);
	void SetSpeed(double speed);
	void SetHP(float health);
	float GetHP();
	Vector3 GetPos();
	void editHP(float _health);

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