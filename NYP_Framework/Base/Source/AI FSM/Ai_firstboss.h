#pragma once

#include "..\Strategy.h"
#include "Vector3.h"

class CStrategy_AI_FirstBoss : public CStrategy // FINAL BOSS (so much for 'first' boss)
{
public:
	CStrategy_AI_FirstBoss(void);
	~CStrategy_AI_FirstBoss(void);

	void Update(Vector3& theDestination, Vector3 theEnemyPosition, Vector3& theEnemyDirection, double speed, double dt);
	void UpdateBoss(Vector3& theDestination, Vector3 theEnemyPosition, Vector3& theEnemyDirection, double speed, double dt, int& weaponIndex);

	void SetDestination(const float x, const float y);
	int GetDestination_x(void);
	int GetDestination_y(void);
	Vector3 GetDestination(void);
	bool GetIsMoving(void);
	bool SetIsMoving(bool _isMoving);
	bool GetIsShooting(void);
	bool SetIsShooting(bool _isShooting);

	// The AI states of the enemy
	enum CURRENT_STATE
	{
		IDLE,
		ATTACK,
		NUM_ENEMY_STATE,
	}CurrentState;

	CStrategy_AI_FirstBoss::CURRENT_STATE GetState(void);
	void SetState(CStrategy_AI_FirstBoss::CURRENT_STATE);

private:
	// Enemy AI State
	int maxDistFromPlayer;
	double shootElapsedTime, timeBetweenShots;
};