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

<<<<<<< HEAD
class CEnemy :public Collision, public GenericEntity, public CAnimation
=======
class CEnemy : public GenericEntity, public CAnimation
>>>>>>> 44f8e4bc87483804927345f66645eba5f9b21036
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

#endif // !Enemy