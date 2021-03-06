#include "../DetectMemoryLeak.h"
#include "Ai_obstacle.h"
#include <iostream>
#include "../EntityManager.h"
#include "../CollisionManager.h"

using namespace std;

/********************************************************************************
Constructor
********************************************************************************/
CStrategy_AI_Obstacle::CStrategy_AI_Obstacle() :maxDistFromPlayer(2), shootElapsedTime(0.0), timeBetweenShots(5.0)
{
	CurrentState = ATTACK;
}

/********************************************************************************
Destructor
********************************************************************************/
CStrategy_AI_Obstacle::~CStrategy_AI_Obstacle()
{
}

/********************************************************************************
Update method
********************************************************************************/
void CStrategy_AI_Obstacle::Update(Vector3& theDestination, Vector3 theEnemyPosition, Vector3& theEnemyDirection, double speed, double dt)
{
	shootElapsedTime += dt * 10;

	int distancePlayerToEnemy = CalculateDistance(theDestination, theEnemyPosition);

	if (distancePlayerToEnemy > 20) // TODO : Not make this hardcoded lmao
		this->SetState(IDLE);
	else
		this->SetState(ATTACK);

	if (CurrentState == ATTACK && shootElapsedTime > timeBetweenShots)
	{
		SetIsShooting(true);
		shootElapsedTime = 0.0;
	}
	else
		SetIsShooting(false);
}

/********************************************************************************
Get the FSM state for this strategy
********************************************************************************/
CStrategy_AI_Obstacle::CURRENT_STATE CStrategy_AI_Obstacle::GetState(void)
{
	return CurrentState;
}

/********************************************************************************
Set the FSM state for this strategy
********************************************************************************/
void CStrategy_AI_Obstacle::SetState(CStrategy_AI_Obstacle::CURRENT_STATE theEnemyState)
{
	CurrentState = theEnemyState;
}