#include "Ai_1.h"
#include <iostream>
#include "../EntityManager.h"
#include "../CollisionManager.h"

using namespace std;

/********************************************************************************
Constructor
********************************************************************************/
CStrategy_AI_1::CStrategy_AI_1()
{
}

/********************************************************************************
Destructor
********************************************************************************/
CStrategy_AI_1::~CStrategy_AI_1()
{
}

/********************************************************************************
Update method
********************************************************************************/
void CStrategy_AI_1::Update(Vector3& theDestination, Vector3 theEnemyPosition, Vector3& theEnemyDirection, double speed, double dt)
{
	// Decide which state to change to
	int distanceHeroToEnemy = CalculateDistance(theDestination, theEnemyPosition);


	if (distanceHeroToEnemy < AI_ATTACK_RANGE)
	{
		//SetIsMoving(false);
		CurrentState = ATTACK;
	}
	else if (distanceHeroToEnemy <= AI_CHASE_RANGE)
	{
		SetIsMoving(true);
		CurrentState = CHASE;
	}
	else
	{
		SetIsMoving(false);
		CurrentState = IDLE;
	}

	// Based on the current state, move the enemy
	switch (CurrentState)
	{
	case CHASE:
		if (theDestination.x - 2 > theEnemyPosition.x)//right
		{
			MoveRight(theEnemyDirection);
			//theEnemyPosition.x += dt * speed;
		}
			
		else if (theDestination.x + 2 < theEnemyPosition.x) // left
		{
			MoveLeft(theEnemyDirection);
			//theEnemyPosition.x -= dt * speed;
		}

		if (theDestination.y - 2 > theEnemyPosition.y) // up
		{
			MoveUp(theEnemyDirection);
			//theEnemyPosition.y += dt * speed;
		}
		else if (theDestination.y + 2 < theEnemyPosition.y) //down
		{
			MoveDown(theEnemyDirection);
			//theEnemyPosition.y -= dt * speed;
		}
		break;
	case ATTACK:
		//TODO - enemy shoot
		  
		break;
	default:
		// Do nothing if idling
		break;
	}
}

void CStrategy_AI_1::MoveUp(Vector3& theEnemyDirection)
{
	if (theEnemyDirection.y != -1)
		theEnemyDirection.y = 1;
	else
		theEnemyDirection.y = 0;
}

void CStrategy_AI_1::MoveDown(Vector3 & theEnemyDirection)
{
	if (theEnemyDirection.y != 1)
		theEnemyDirection.y = -1;
	else
		theEnemyDirection.y = 0;
}

void CStrategy_AI_1::MoveRight(Vector3 & theEnemyDirection)
{
	if (theEnemyDirection.x != -1)
		theEnemyDirection.x = 1;
	else
		theEnemyDirection.x = 0;
}

void CStrategy_AI_1::MoveLeft(Vector3 & theEnemyDirection)
{
	if (theEnemyDirection.x != 1)
		theEnemyDirection.x = -1;
	else
		theEnemyDirection.x = 0;
}

//void CStrategy_AI_1::CollisionCheck(Vector3 & theEnemyDirection, Vector3& MaxAABB, Vector3& MinAABB)
//{
//	Vector3 tempMax = MaxAABB;
//	Vector3 tempMin = MinAABB;
//	std::list<EntityBase*> cpy = EntityManager::GetInstance()->getCollisionList();
//
//	float checkoffset = 0.5f;
//
//	if (theEnemyDirection.y == 1)
//	{
//		MaxAABB = (tempMax + Vector3(0.f, checkoffset, 0.f));
//		MinAABB = (tempMin + Vector3(0.f, checkoffset, 0.f));
//
//		std::list<EntityBase*>::iterator it, end;
//		end = cpy.end();
//		for (it = cpy.begin(); it != end; ++it)
//		{
//			if (CollisionManager::GetInstance()->CheckAABBCollision(enemy, *it))
//			{
//				GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it);
//				if (thatEntity->type == WALL || thatEntity->type == ENEMY)
//				{
//					std::cout << "Something is blocking up" << std::endl;
//					theEnemyDirection.y = 0;
//					break;
//				}
//			}
//		}
//	}
//}

/********************************************************************************
Set the destination for this strategy
********************************************************************************/
void CStrategy_AI_1::SetDestination(const float x, const float y)
{
	theDestination.x = x;
	theDestination.y = y;
}

/********************************************************************************
Get the destination for this strategy
********************************************************************************/
int CStrategy_AI_1::GetDestination_x(void)
{
	return theDestination.x;
}

/********************************************************************************
Get the destination for this strategy
********************************************************************************/
int CStrategy_AI_1::GetDestination_y(void)
{
	return theDestination.y;
}

/********************************************************************************
Get the destination for this strategy
********************************************************************************/
Vector3 CStrategy_AI_1::GetDestination(void)
{
	return theDestination;
}

bool CStrategy_AI_1::GetIsMoving(void)
{
	return m_bIsMoving;
}

void CStrategy_AI_1::SetIsMoving(bool _isMoving)
{
	m_bIsMoving = _isMoving;
}

/********************************************************************************
Get the FSM state for this strategy
********************************************************************************/
CStrategy_AI_1::CURRENT_STATE CStrategy_AI_1::GetState(void)
{
	return CurrentState;
}

/********************************************************************************
Set the FSM state for this strategy
********************************************************************************/
void CStrategy_AI_1::SetState(CStrategy_AI_1::CURRENT_STATE theEnemyState)
{
	CurrentState = theEnemyState;
}
