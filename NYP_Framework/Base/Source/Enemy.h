#pragma once
#include "Vector3.h"
#include "Strategy.h"
#include "Animation.h"
#include "Collider\Collider.h"
#include "EntityBase.h"

class Mesh;

class CEnemy : public CAnimation, public Collision, public EntityBase
{
	//	friend class CStrategy;
public:
	CEnemy(Mesh* modelMesh);
	~CEnemy(void);

	// ENEMY Update
	void Update(double dt);
	void Render(void);


	// Strategy
	void ChangeStrategy(CStrategy* theNewStrategy, bool bDelete = true);

private:
	Mesh* modelMesh;
	Vector3 position;

	CStrategy* theStrategy;
};
