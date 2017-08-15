#include "Enemy.h"

#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"



/********************************************************************************
Constructor
********************************************************************************/
CEnemy::CEnemy() :defaultPosition(0, 0, 0)
{
}

/********************************************************************************
Destructor
********************************************************************************/
CEnemy::~CEnemy(void)
{
	if (theStrategy != NULL)
	{
		delete theStrategy;
		theStrategy = NULL;
	}
}

void CEnemy::Update(double dt)
{
	if (theStrategy != NULL)
	{
		theStrategy->Update(Player::GetInstance()->GetPos(), position, dt);
	}
	enemyModel->SetPosition(position);
}

void CEnemy::Init()
{
	// Set the current values
	position.Set(0, 5, 0);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);
}

void CEnemy::SetEnemyGE(GenericEntity * _enemyModel)
{
	this->enemyModel = _enemyModel;
	this->enemyModel->type = GenericEntity::OBJECT_TYPE::ENEMY;
}

Vector3 CEnemy::GetPos()
{
	return position;
}

/********************************************************************************
Strategy
********************************************************************************/
void CEnemy::ChangeStrategy(CStrategy* theNewStrategy, bool bDelete)
{
	if (bDelete == true)
	{
		if (theStrategy != NULL)
		{
			delete theStrategy;
			theStrategy = NULL;
		}
	}

	theStrategy = theNewStrategy;
}
<<<<<<< HEAD
=======

CEnemy * Create::Enemy(const std::string & _meshName, const Vector3 & position, const Vector3 & scale, const Vector3 & maxAABB, const Vector3 & minAABB)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CEnemy* result = new CEnemy(modelMesh);
	result->SetPosition(position);
	result->SetScale(scale);
	result->SetAABB(maxAABB, minAABB);
	result->enemyType->type = GenericEntity::OBJECT_TYPE::ENEMY;
	EntityManager::GetInstance()->AddEntity(result);


	return result;
}
>>>>>>> c8ef45636c3bdf97f9493622309e51f390ea85d8
