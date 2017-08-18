#pragma once
#include "..\Strategy.h"
#include "Vector3.h"

class CStrategy_AI_1
	: public CStrategy
{
public:
	CStrategy_AI_1(void);
	~CStrategy_AI_1(void);


	void Update(Vector3& theDestination, Vector3& theEnemyPosition, Vector3& theEnemyDirection, double speed, double dt);

	void MoveUp(Vector3& theEnemyDirection);
	void MoveDown(Vector3& theEnemyDirection);
	void MoveRight(Vector3& theEnemyDirection);
	void MoveLeft(Vector3& theEnemyDirection);
	//void CollisionCheck(Vector3 & theEnemyDirection, Vector3& MaxAABB, Vector3& MinAABB);

	void SetDestination(const float x, const float y);
	int GetDestination_x(void);
	int GetDestination_y(void);
	Vector3 GetDestination(void);
	bool GetIsMoving(void);
	void SetIsMoving(bool _isMoving);

	

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
		AI_CHASE_RANGE = 25,
		AI_ATTACK_RANGE = 5,
		NUM_AI_STATE_RANGE,
	};
	CStrategy_AI_1::CURRENT_STATE GetState(void);
	void SetState(CStrategy_AI_1::CURRENT_STATE);

private:
	// Enemy AI State
	CStrategy_AI_1::CURRENT_STATE CurrentState;
};
