#include "Ai_firstboss.h"
#include <iostream>
#include "../EntityManager.h"
#include "../CollisionManager.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../AudioEngine.h"

using namespace std;

/********************************************************************************
Constructor
********************************************************************************/
CStrategy_AI_FirstBoss::CStrategy_AI_FirstBoss() :maxDistFromPlayer(3), shootElapsedTime(0.0), timeBetweenShots(0.5), RNG(0), prevRoll(-1), m_dElapsedTime(0.0), m_dAttackDuration(0.0)
{
	CurrentState = IDLE;
	this->m_bIsShooting = false;
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

	if (distancePlayerToEnemy < 40 && this->CurrentState == IDLE)
	{
		this->SetState(ATTACK_SET_ONE);
		AudioEngine::GetInstance()->StopAllSounds();
		AudioEngine::GetInstance()->PlayASound("lastbattle", true);
	}

	if (this->CurrentState == ATTACK_SET_ONE && _health < 500)
		this->SetState(ATTACK_SET_TWO);

	if (this->CurrentState == IDLE)
		return;

	if (m_dElapsedTime > m_dAttackDuration + 1.f) // 1s cd after doing attack
	{
		int rolls;

		if (this->CurrentState == ATTACK_SET_ONE)
			rolls = 3;
		else
			rolls = 2;

		RNG = Math::RandIntMinMax(0, rolls);

		if (RNG == prevRoll)
		{
			++RNG;
			RNG = Math::Wrap(RNG, 0, rolls);
		}

		prevRoll = RNG;

		Player::GetInstance()->m_bProjectileCircle = false; // I hate this move so much
		Player::GetInstance()->m_bPullEffect = false;

		// Time before attack changes

		if (this->CurrentState == ATTACK_SET_ONE)
		{
			switch (RNG) {
			case 0:
				_weaponIndex = 0;
				m_dAttackDuration = m_dElapsedTime + 3.f;
				Player::GetInstance()->m_bProjectileCircle = true;
				break;
			case 1:
				m_dAttackDuration = m_dElapsedTime + 5.f;
				Player::GetInstance()->m_bPullEffect = true;
				break;
			case 2:
				_weaponIndex = 1;
				m_dAttackDuration = m_dElapsedTime + 3.f;
				break;
			case 3:
				_weaponIndex = 2;
				m_dAttackDuration = m_dElapsedTime + 4.f;
				break;

			default:
				break;
			}
		}
		else if (this->CurrentState == ATTACK_SET_TWO)
		{
			switch (RNG) {
			case 0:
				_weaponIndex = Math::RandIntMinMax(1, 2);
				m_dAttackDuration = m_dElapsedTime + 4.f;
				Player::GetInstance()->m_bPullEffect = true;
				break;
			case 1:
				m_dAttackDuration = m_dElapsedTime + 4.f;
				break;
			case 2:
				_weaponIndex = 0;
				m_dAttackDuration = m_dElapsedTime + 4.f;
				Player::GetInstance()->m_bProjectileCircle = true;
				Player::GetInstance()->m_bHunted = true;
				break;

			default:
				break;
			}
		}
	}

	switch (CurrentState)
	{
	case ATTACK_SET_ONE:
	{
		switch (RNG) {
		case 0:
		{
			float y_rand, x_diff;
			y_rand = Math::RandFloatMinMax(-5, 5);
			float neg_x = Math::RandIntMinMax(0, 1);
			if (neg_x == 0)
				x_diff = (5 - abs(y_rand));
			else
				x_diff = -(5 - abs(y_rand));

			_shootpos = _destination + Vector3(x_diff, y_rand, 0.f);
			timeBetweenShots = 0.5f;
			break;
		}
		case 1:
			timeBetweenShots = m_dAttackDuration;
			break;
		case 2:
			timeBetweenShots = 0.f;
			break;
		case 3:
			timeBetweenShots = 0.f;
			break;

		default:
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
		switch (RNG) {
		case 0:
			timeBetweenShots = 0.f;
			break;
		case 1:
			if (_weaponIndex != 1)
				_weaponIndex = 1;
			else
				_weaponIndex = 2;

			timeBetweenShots = 0.f;
			break;
		case 2:
		{
			float y_rand, x_diff;
			y_rand = Math::RandFloatMinMax(-5, 5);
			float neg_x = Math::RandIntMinMax(0, 1);
			if (neg_x == 0)
				x_diff = (5 - abs(y_rand));
			else
				x_diff = -(5 - abs(y_rand));

			_shootpos = _destination + Vector3(x_diff, y_rand, 0.f);
			timeBetweenShots = 0.5f;
			break;
		}

		default:
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
