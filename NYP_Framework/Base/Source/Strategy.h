#ifndef Strategy
#define Strategy

#include "Vector3.h"

class CStrategy
{
public:
	CStrategy(void);
	~CStrategy(void);

	virtual void Update(Vector3& theDestination, Vector3 theEnemyPosition, Vector3& theEnemyDirection,double speed, double dt) = 0;
	virtual void UpdateBoss(Vector3& theDestination, Vector3 theEnemyPosition, Vector3& theEnemyDirection, double speed, double dt, int& weaponIndex);

	virtual void SetDestination(const float x, const float y) = 0;
	virtual int GetDestination_x(void) = 0;
	virtual int GetDestination_y(void) = 0;
	virtual Vector3 GetDestination(void) = 0;
	virtual bool GetIsMoving(void) = 0;
	virtual bool GetIsShooting(void) = 0;
	//virtual bool GetIsReloading(void) = 0;

	virtual void MoveUp(Vector3& theEnemyDirection);
	virtual void MoveDown(Vector3& theEnemyDirection);
	virtual void MoveRight(Vector3& theEnemyDirection);
	virtual void MoveLeft(Vector3& theEnemyDirection);

protected:
	int CalculateDistance(Vector3 theDestination, Vector3 theEnemyPosition);

	// The Destination for this Strategy
	Vector3 theDestination;
	bool m_bIsMoving;
	bool m_bIsShooting;
};
#endif // !