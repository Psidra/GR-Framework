#include "Strategy.h"
#include <iostream>

using namespace std;

CStrategy::CStrategy()
{
}

CStrategy::~CStrategy()
{

}

void CStrategy::Update(Vector3& theDestination, Vector3 theEnemyPosition, Vector3& theEnemyDirection, bool& isShooting, double speed, double dt)
{
	cout << "void CStrategy::Update()" << endl;
}

void CStrategy::MoveUp(Vector3 & theEnemyDirection)
{
	if (theEnemyDirection.y != -1)
		theEnemyDirection.y = 1;
	else
		theEnemyDirection.y = 0;
}

void CStrategy::MoveDown(Vector3 & theEnemyDirection)
{
	if (theEnemyDirection.y != 1)
		theEnemyDirection.y = -1;
	else
		theEnemyDirection.y = 0;
}

void CStrategy::MoveRight(Vector3 & theEnemyDirection)
{
	if (theEnemyDirection.x != -1)
		theEnemyDirection.x = 1;
	else
		theEnemyDirection.x = 0;
}

void CStrategy::MoveLeft(Vector3 & theEnemyDirection)
{
	if (theEnemyDirection.x != 1)
		theEnemyDirection.x = -1;
	else
		theEnemyDirection.x = 0;
}

int CStrategy::CalculateDistance(Vector3 theDestination, Vector3 theEnemyPosition)
{
	return ((theDestination.x - theEnemyPosition.x)*(theDestination.x - theEnemyPosition.x) +
		(theDestination.y - theEnemyPosition.y)*(theDestination.y - theEnemyPosition.y));
}
