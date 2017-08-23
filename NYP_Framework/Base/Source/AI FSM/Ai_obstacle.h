#pragma once
#include "..\Strategy.h"
#include "Vector3.h"

class CStrategy_AI_Obstacle
	: public CStrategy
{
public:
	CStrategy_AI_Obstacle(void);
	~CStrategy_AI_Obstacle(void);


	void Update(Vector3& theDestination, Vector3 theEnemyPosition, Vector3& theEnemyDirection, bool& isShooting, double speed, double dt);

	inline void SetDestination(const float x, const float y) { this->theDestination.Set(x, y, 0); };
	inline int GetDestination_x(void) { return this->theDestination.x; };
	inline int GetDestination_y(void) { return this->theDestination.y; };
	inline Vector3 GetDestination(void) { return this->theDestination; };
	inline bool GetIsMoving(void) { return this->m_bIsMoving; };
	inline bool SetIsMoving(bool _isMoving) { this->m_bIsMoving = _isMoving; };

	// The AI states of the enemy
	enum CURRENT_STATE
	{
		IDLE,
		ATTACK,
		NUM_ENEMY_STATE,
	};

	CStrategy_AI_Obstacle::CURRENT_STATE GetState(void);
	void SetState(CStrategy_AI_Obstacle::CURRENT_STATE);

private:
	// Enemy AI State
	CStrategy_AI_Obstacle::CURRENT_STATE CurrentState;
	int maxDistFromPlayer;
	double shootElapsedTime, timeBetweenShots;
};
