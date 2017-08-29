#include "../DetectMemoryLeak.h"
#include "Ai_2.h"
#include <iostream>
#include "../EntityManager.h"
#include "../CollisionManager.h"

using namespace std;

/********************************************************************************
Constructor
********************************************************************************/
CStrategy_AI_2::CStrategy_AI_2() :maxDistFromPlayer(3), shootElapsedTime(0.0), timeBetweenShots(4.0)
{
}

/********************************************************************************
Destructor
********************************************************************************/
CStrategy_AI_2::~CStrategy_AI_2()
{
}

/********************************************************************************
Update method
********************************************************************************/
void CStrategy_AI_2::Update(Vector3& theDestination, Vector3 theEnemyPosition, Vector3& theEnemyDirection, double speed, double dt)
{
	// Decide which state to change to
	int distancePlayerToEnemy = CalculateDistance(theDestination, theEnemyPosition);
	shootElapsedTime += dt;

	if (distancePlayerToEnemy <= AI_CHASE_RANGE && distancePlayerToEnemy > AI_ATTACK_RANGE)
	{
		SetIsMoving(true);
		CurrentState = CHASE;
	}
	else if (distancePlayerToEnemy <= AI_ATTACK_RANGE)
	{
		theEnemyDirection = 0;	//enemy stops
		CurrentState = ATTACK;
	}


	// Based on the current state, move the enemy
	if (CurrentState == CHASE)
	{
		SetIsShooting(false);

		if (theDestination.x - maxDistFromPlayer > theEnemyPosition.x)
			MoveRight(theEnemyDirection);
		else if (theDestination.x + maxDistFromPlayer < theEnemyPosition.x)
			MoveLeft(theEnemyDirection);
		if (theDestination.y - maxDistFromPlayer > theEnemyPosition.y)
			MoveUp(theEnemyDirection);
		else if (theDestination.y + maxDistFromPlayer < theEnemyPosition.y)
			MoveDown(theEnemyDirection);
	}
	else if (CurrentState == ATTACK)
	{
		if (shootElapsedTime > timeBetweenShots)
		{
			SetIsMoving(false);		//stop animate moving
			SetIsShooting(true);	//set animate shoot & enable shoot
			shootElapsedTime = 0.0;
		}
		else
			SetIsShooting(false);	//stop animate shoot & disable shoot
	}
	else
	{
		SetIsMoving(false);
	}
}

/********************************************************************************
Set the destination for this strategy
********************************************************************************/
void CStrategy_AI_2::SetDestination(const float x, const float y)
{
	theDestination.x = x;
	theDestination.y = y;
}

/********************************************************************************
Get the destination for this strategy
********************************************************************************/
int CStrategy_AI_2::GetDestination_x(void)
{
	return theDestination.x;
}

/********************************************************************************
Get the destination for this strategy
********************************************************************************/
int CStrategy_AI_2::GetDestination_y(void)
{
	return theDestination.y;
}

/********************************************************************************
Get the destination for this strategy
********************************************************************************/
Vector3 CStrategy_AI_2::GetDestination(void)
{
	return theDestination;
}

bool CStrategy_AI_2::GetIsMoving(void)
{
	return m_bIsMoving;
}

bool CStrategy_AI_2::SetIsMoving(bool _isMoving)
{
	m_bIsMoving = _isMoving;
	return _isMoving;
}

bool CStrategy_AI_2::GetIsShooting(void)
{
	return m_bIsShooting;
}

bool CStrategy_AI_2::SetIsShooting(bool _isShooting)
{
	m_bIsShooting = _isShooting;
	return _isShooting;
}

/********************************************************************************
Get the FSM state for this strategy
********************************************************************************/
CStrategy_AI_2::CURRENT_STATE CStrategy_AI_2::GetState(void)
{
	return CurrentState;
}

/********************************************************************************
Set the FSM state for this strategy
********************************************************************************/
void CStrategy_AI_2::SetState(CStrategy_AI_2::CURRENT_STATE theEnemyState)
{
	CurrentState = theEnemyState;
}
