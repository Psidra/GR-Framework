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

	//enemyAnimated = new GenericEntity*[8];
	//for (size_t i = 0; i < 8; i++)
	//{
	//	enemyAnimated[i] = new GenericEntity();
	//}
	//enemyAnimated[0]->SetMesh(MeshList::GetInstance()->GetMesh("Player_fstand1"));
	//enemyAnimated[1]->SetMesh(MeshList::GetInstance()->GetMesh("Player_fstand2"));
	//enemyAnimated[2]->SetMesh(MeshList::GetInstance()->GetMesh("Player_bstand1"));
	//enemyAnimated[3]->SetMesh(MeshList::GetInstance()->GetMesh("Player_bstand2"));
	//enemyAnimated[4]->SetMesh(MeshList::GetInstance()->GetMesh("Player_fwalk1"));
	//enemyAnimated[5]->SetMesh(MeshList::GetInstance()->GetMesh("Player_fwalk2"));
	//enemyAnimated[6]->SetMesh(MeshList::GetInstance()->GetMesh("Player_bwalk1"));
	//enemyAnimated[7]->SetMesh(MeshList::GetInstance()->GetMesh("Player_bwalk2"));
	///*Player::GetInstance()->SetIndices_fStand(0, 1);
	//Player::GetInstance()->SetIndices_bStand(2, 3);
	//Player::GetInstance()->SetIndices_fWalk(4, 5);
	//Player::GetInstance()->SetIndices_bWalk(6, 7);*/
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