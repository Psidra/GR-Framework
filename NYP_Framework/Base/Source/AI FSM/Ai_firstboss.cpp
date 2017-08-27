#include "Ai_firstboss.h"
#include <iostream>
#include "../EntityManager.h"
#include "../CollisionManager.h"
#include "../PlayerInfo/PlayerInfo.h"

using namespace std;

/********************************************************************************
Constructor
********************************************************************************/
CStrategy_AI_FirstBoss::CStrategy_AI_FirstBoss() :maxDistFromPlayer(3), shootElapsedTime(0.0), timeBetweenShots(0.5), RNG(0), prevRoll(-1), m_dElapsedTime(0.0), m_dAttackDuration(0.0)
{
	CurrentState = ATTACK_SET_ONE;
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

void CStrategy_AI_FirstBoss::UpdateBoss(Vector3& _destination, Vector3& _shootpos, Vector3& _enemydir, double _speed, int& _weaponIndex, float _health, double dt)
{
	m_dElapsedTime += dt;
	shootElapsedTime += dt;

	int distancePlayerToEnemy = CalculateDistance(_destination, _shootpos);

	if (distancePlayerToEnemy > 50) // TODO : Not make this hardcoded lmao
		this->SetState(IDLE);
	else
		this->SetState(ATTACK_SET_ONE);

	if (this->CurrentState == ATTACK_SET_ONE && _health < 100)
		this->SetState(ATTACK_SET_TWO);

	if (m_dElapsedTime > m_dAttackDuration + 1.f) // 1s cd after doing attack
	{
		RNG = Math::RandIntMinMax(0, 2);

		if (RNG == prevRoll)
		{
			++RNG;
			RNG = Math::Wrap(RNG, 0, 2);
		}

		prevRoll = RNG;

		Player::GetInstance()->m_bProjectileCircle = false;

		// Time before attack changes
		switch (RNG) {
		case 0:
			_weaponIndex = 0;
			m_dAttackDuration = m_dElapsedTime + 3.f;
			Player::GetInstance()->m_bProjectileCircle = true;
			break;
		case 1:
			m_dAttackDuration = m_dElapsedTime + 2.f;
			break;
		case 2:
			m_dAttackDuration = m_dElapsedTime + 2.f;
			break;

		default:
			break;
		}
	}

	switch (CurrentState)
	{
	case ATTACK_SET_ONE:
	{
		switch (RNG) {
		case 0:
			_shootpos = _destination + Vector3(Math::RandFloatMinMax(-4, 4), Math::RandFloatMinMax(-4, 4), 0.f);
			timeBetweenShots = 0.5f;
			break;
		case 1:
			break;
		case 2:
			break;

		}

		if (shootElapsedTime > timeBetweenShots && m_dAttackDuration > m_dElapsedTime)
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
		SetIsShooting(false);
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
