#pragma once
#include "..\Strategy.h"
#include "Vector3.h"

class CStrategy_AI_1
	: public CStrategy
{
public:
	CStrategy_AI_1(void);
	~CStrategy_AI_1(void);

	void Update(Vector3& theDestination, Vector3& theEnemyPosition, double dt);
	void SetDestination(const float x, const float y);

	int GetDestination_x(void);
	int GetDestination_y(void);
	Vector3 GetDestination(void);

	// The AI states of the enemy
	enum CURRENT_STATE
	{
		IDLE,
		CHASE,
		ATTACK,
		NUM_ENEMY_STATE,
	};
	enum AI_STATE_RANGE
	{
		AI_STATE_ATTACK = 125,
		NUM_AI_STATE_RANGE,
	};
	CStrategy_AI_1::CURRENT_STATE GetState(void);
	void SetState(CStrategy_AI_1::CURRENT_STATE);

private:
	// Enemy AI State
	CStrategy_AI_1::CURRENT_STATE CurrentState;
};