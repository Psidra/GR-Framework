#include "Enemy.h"

#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"


/********************************************************************************
Constructor
********************************************************************************/
CEnemy::CEnemy() :speed(1.0), position(0,5,0), health(100.f)
{
}

CEnemy::CEnemy(Vector3 pos, float _health) : position(pos), health(_health)
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

void CEnemy::Init()
{
	this->SetCollider(true);
	this->SetSpeed(2.0);
}

void CEnemy::Update(double dt)
{
	if (this->theStrategy != NULL)
	{
		this->theStrategy->Update(Player::GetInstance()->GetPos(), this->position, speed, dt);
	}

	this->SetPosition(this->position);
	this->SetAABB(this->GetScale() * 0.5f + this->GetPos(), this->GetScale() * -0.5f + this->GetPos());

	if (health <= 0)
		this->SetIsDone(true);
}

void CEnemy::SetSpeed(double speed)
{
	this->speed = speed;
}

void CEnemy::SetHP(float _health)
{
	health = _health;
}

float CEnemy::GetHP()
{
	return health;
}

Vector3 CEnemy::GetPos()
{
	return position;
}

void CEnemy::editHP(float _health)
{
	this->health += _health;
}

void CEnemy::CollisionResponse(GenericEntity* thatEntity)
{
	switch (thatEntity->type) {
	case GenericEntity::OBJECT_TYPE::PLAYER_BULLET:
		thatEntity->SetIsDone(true);
		editHP(-20);
		std::cout << "player bullet collide with enemy" << std::endl;
		break;
	case GenericEntity::OBJECT_TYPE::WALL:
		std::cout << "enemy collide with wall" << std::endl;
		break;
	default:
		break;
	}
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