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

class CEnemy :public Collision, public GenericEntity, public CAnimation
{
protected:
	Mesh* modelMesh;
	Vector3 position;
	Vector3 maxBoundary, minBoundary;
	Vector3 target;
	double speed;
	float health;
	
	CStrategy* theStrategy;
	
public:
	CEnemy();
	virtual ~CEnemy();

	void Update(double dt = 0.0333f);
	void SetEnemyGE(GenericEntity* _enemyModel);
	void SetSpeed(double speed);
	void SetHP(float health);
	float GetHP();
	Vector3 GetPos();

	GenericEntity* enemyModel;
	// Strategy
	void ChangeStrategy(CStrategy* theNewStrategy, bool bDelete = true);
};

#endif // !Enemy