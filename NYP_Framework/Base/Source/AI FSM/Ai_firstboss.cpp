#include "Ai_firstboss.h"
#include <iostream>
#include "../EntityManager.h"
#include "../CollisionManager.h"

using namespace std;

/********************************************************************************
Constructor
********************************************************************************/
CStrategy_AI_FirstBoss::CStrategy_AI_FirstBoss() :maxDistFromPlayer(3), shootElapsedTime(0.0), timeBetweenShots(1.0)
{
	CurrentState = ATTACK_SET_ONE;
	prevRoll = -1;
}

/********************************************************************************
Destructor
********************************************************************************/
CStrategy_AI_FirstBoss::~CStrategy_AI_FirstBoss()
{
}

/********************************************************************************
Update method
********************************************************************************/
void CStrategy_AI_FirstBoss::Update(Vector3& theDestination, Vector3 theEnemyPosition, Vector3& theEnemyDirection, double speed, double dt)
{
}

void CStrategy_AI_FirstBoss::UpdateBoss(Vector3& _destination, Vector3 _enemypos, Vector3& _enemydir, double _speed, int& _weaponIndex, float _health, double dt)
{
	shootElapsedTime += dt;

	if (this->CurrentState == ATTACK_SET_ONE && _health < 50)
		this->SetState(ATTACK_SET_TWO);

	RNG = rand() % 3;

	if (RNG == prevRoll)
		Math::Wrap(++RNG, 0, 3);

	prevRoll = RNG;

	switch (CurrentState)
	{
	case ATTACK_SET_ONE:
	{
		if (shootElapsedTime > timeBetweenShots)
		{
			SetIsMoving(false);		//stop animate moving
			SetIsShooting(true);	//set animate shoot & enable shoot
			shootElapsedTime = 0.0;
		}
		else
			SetIsShooting(false);	//stop animate shoot & disable shoot
		break;
	}
	case ATTACK_SET_TWO:
	{

		break;
	}

	default:
		// Do nothing if idling
		SetIsMoving(false);
		break;
	}
}
/********************************************************************************
Set the destination for this strategy
********************************************************************************/
void CStrategy_AI_FirstBoss::SetDestination(const float x, const float y)
{
	theDestination.x = x;
	theDestination.y = y;
}

/********************************************************************************
Get the destination for this strategy
********************************************************************************/
int CStrategy_AI_FirstBoss::GetDestination_x(void)
{
	return theDestination.x;
}

/********************************************************************************
Get the destination for this strategy
********************************************************************************/
int CStrategy_AI_FirstBoss::GetDestination_y(void)
{
	return theDestination.y;
}

/********************************************************************************
Get the destination for this strategy
********************************************************************************/
Vector3 CStrategy_AI_FirstBoss::GetDestination(void)
{
	return theDestination;
}

bool CStrategy_AI_FirstBoss::GetIsMoving(void)
{
	return m_bIsMoving;
}

bool CStrategy_AI_FirstBoss::SetIsMoving(bool _isMoving)
{
	m_bIsMoving = _isMoving;
	return _isMoving;
}

bool CStrategy_AI_FirstBoss::GetIsShooting(void)
{
	return m_bIsShooting;
}

bool CStrategy_AI_FirstBoss::SetIsShooting(bool _isShooting)
{
	m_bIsShooting = _isShooting;
	return _isShooting;
}

/********************************************************************************
Get the FSM state for this strategy
********************************************************************************/
CStrategy_AI_FirstBoss::CURRENT_STATE CStrategy_AI_FirstBoss::GetState(void)
{
	return CurrentState;
}

/********************************************************************************
Set the FSM state for this strategy
********************************************************************************/
void CStrategy_AI_FirstBoss::SetState(CStrategy_AI_FirstBoss::CURRENT_STATE theEnemyState)
{
	CurrentState = theEnemyState;
}
