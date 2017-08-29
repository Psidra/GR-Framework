#ifndef AI_3
#define AI_3

#include "..\Strategy.h"
#include "Vector3.h"

class CStrategy_AI_3
	: public CStrategy
{
public:
	CStrategy_AI_3(void);
	~CStrategy_AI_3(void);


	void Update(Vector3& theDestination, Vector3 theEnemyPosition, Vector3& theEnemyDirection, double speed, double dt);

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
		CHASE,
		ATTACK,
		NUM_ENEMY_STATE,
	};
	enum AI_STATE_RANGE
	{
		AI_ATTACK_RANGE = 15,
		AI_CHASE_RANGE = 25,
		NUM_AI_STATE_RANGE,
	};
	CStrategy_AI_3::CURRENT_STATE GetState(void);
	void SetState(CStrategy_AI_3::CURRENT_STATE);

private:
	// Enemy AI State
	CStrategy_AI_3::CURRENT_STATE CurrentState;
	int maxDistFromPlayer;
	double shootElapsedTime, timeBetweenShots;
	bool targetFound;
};
#endif // !AI_3