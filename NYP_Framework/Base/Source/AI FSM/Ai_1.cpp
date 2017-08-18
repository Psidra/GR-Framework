#include "Ai_1.h"
#include <iostream>

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
void CStrategy_AI_1::Update(Vector3& theDestination, Vector3& theEnemyPosition, double speed, double dt)
{
	// Decide which state to change to
	int distanceHeroToEnemy = CalculateDistance(theDestination, theEnemyPosition);
	if (distanceHeroToEnemy < AI_ATTACK_RANGE)
		CurrentState = CHASE;
	else
		CurrentState = IDLE;

	// Based on the current state, move the enemy
	switch (CurrentState)
	{
	case CHASE:
		SetIsMoving(true);
		if (theDestination.x - 2 > theEnemyPosition.x)
			theEnemyPosition.x += dt * speed;
		else if (theDestination.x + 2 < theEnemyPosition.x)
			theEnemyPosition.x -= dt * speed;

		if (theDestination.y - 2 > theEnemyPosition.y)
			theEnemyPosition.y += dt * speed;
		else if (theDestination.y + 2 < theEnemyPosition.y)
			theEnemyPosition.y -= dt * speed;
		break;
	case ATTACK:
		if (theDestination.x - 2 == theEnemyPosition.x || theDestination.x + 2 == theEnemyPosition.x ||
			theDestination.y - 2 == theEnemyPosition.y || theDestination.y + 2 == theEnemyPosition.y)
		{
			//shoot
		}
		  
		break;
	default:
		// Do nothing if idling
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
