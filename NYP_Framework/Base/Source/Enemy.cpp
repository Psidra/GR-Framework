#include "Enemy.h"

#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"



/********************************************************************************
Constructor
********************************************************************************/
CEnemy::CEnemy() :speed(1.0), position(0,5,0)
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
	if (this->theStrategy != NULL)
	{
		this->theStrategy->Update(Player::GetInstance()->GetPos(), this->position, speed, dt);
	}
	this->enemyModel->SetPosition(this->position);
	this->SetPosition(this->position);
	this->enemyModel->SetAABB(this->GetScale() * 0.5f + this->GetPos(), this->GetScale() * -0.5f + this->GetPos());
}

void CEnemy::SetEnemyGE(GenericEntity * _enemyModel)
{
	this->enemyModel = _enemyModel;
	this->enemyModel->type = GenericEntity::OBJECT_TYPE::ENEMY;
}

void CEnemy::SetSpeed(double speed)
{
	this->speed = speed;
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