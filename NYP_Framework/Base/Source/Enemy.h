#ifndef Enemies
#define Enemies

#include "EnemyBase.h"

class CEnemy : public EnemyBase
{	
public:
	CEnemy();
	CEnemy(Vector3 pos);
	virtual ~CEnemy();

	void Init(float _hp = 100.0f, double _speed = 2.0, int _enemyType = 1, bool _invul = false);
	void SetTypeOfEnemy(int _enemyType);
	void Update(double dt = 0.0333f);
	void Shoot(double dt);

	void CollisionCheck();
	// Collision Response
	void CollisionResponse(GenericEntity* ThatEntity);

private:

};

namespace Create
{
	CEnemy* Enemy(Vector3 position, const std::string& _meshName, Vector3 scale = Vector3(1, 1, 1));
};

#endif // !Enemy