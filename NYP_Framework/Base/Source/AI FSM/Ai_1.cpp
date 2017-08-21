#include "Ai_1.h"
#include <iostream>
#include "../EntityManager.h"
#include "../CollisionManager.h"

using namespace std;

/********************************************************************************
Constructor
********************************************************************************/
CStrategy_AI_1::CStrategy_AI_1():maxDistFromPlayer(2), shootElapsedTime(0.0), timeBetweenShots(5.0)
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
void CStrategy_AI_1::Update(Vector3& theDestination, Vector3 theEnemyPosition, Vector3& theEnemyDirection,  bool& isShooting, double speed, double dt)
{
	shootElapsedTime += dt * 10;
	// Decide which state to change to
	int distancePlayerToEnemy = CalculateDistance(theDestination, theEnemyPosition);


	if (distancePlayerToEnemy > AI_ATTACK_RANGE)
	{
		CurrentState = CHASE;
	}
	else if (distancePlayerToEnemy < AI_ATTACK_RANGE)
	{
		CurrentState = ATTACK;
	}

	// Based on the current state, move the enemy
	switch (CurrentState)
	{
	case CHASE:
		
		isShooting = false;

		if (SetIsMoving(true))
		{
			if (theDestination.x - maxDistFromPlayer > theEnemyPosition.x)
				MoveRight(theEnemyDirection);
			else if (theDestination.x + maxDistFromPlayer < theEnemyPosition.x)
				MoveLeft(theEnemyDirection);
			else if (theDestination.y - maxDistFromPlayer > theEnemyPosition.y)
				MoveUp(theEnemyDirection);
			else if (theDestination.y + maxDistFromPlayer < theEnemyPosition.y)
				MoveDown(theEnemyDirection);
		}

		break;
	case ATTACK:
		if (shootElapsedTime > timeBetweenShots)
		{
			isShooting = true;
			shootElapsedTime = 0.0;
		}
		break;
	default:
		// Do nothing if idling
		SetIsMoving(false);
		break;
	}
}

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

bool CStrategy_AI_1::SetIsMoving(bool _isMoving)
{
	m_bIsMoving = _isMoving;
	return _isMoving;
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
