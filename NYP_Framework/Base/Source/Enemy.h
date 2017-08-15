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

class Mesh;

class CEnemy :public Collision, public EntityBase, public CAnimation
{
protected:
	Mesh* modelMesh;

	Vector3 defaultPosition;
	Vector3 position, scale;
	Vector3 maxBoundary, minBoundary;
	Vector3 target;
	
	CStrategy* theStrategy;
	
public:
	CEnemy();
	virtual ~CEnemy();

	void Init();
	void Update(double dt = 0.0333f);
	void SetEnemyGE(GenericEntity* _enemyModel);
	Vector3 GetPos();
	GenericEntity* enemyModel;

	// Strategy
	void ChangeStrategy(CStrategy* theNewStrategy, bool bDelete = true);
};

#endif // !Enemy