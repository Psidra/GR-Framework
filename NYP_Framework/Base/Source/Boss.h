#ifndef BOSS_H
#define BOSS_H

#include "EnemyBase.h"

class Boss : public EnemyBase
{
public:
	Boss();
	Boss(Mesh* _mesh);
	virtual ~Boss();
	
	void Init(float _hp = 100.0f, double _speed = 2.0, int _enemyType = 1, bool _invul = false);
	void SetTypeOfEnemy(int _enemyType);

	void Update(double dt);

	// Set Max Health
	void SetMaxHealth(float _maxHealth);
	// Get Max Health
	float GetMaxHealth();
	// Edit Max Health
	void EditMaxHealth(float _value);

	//// Collision Response
	//void CollisionResponse(GenericEntity* thatEntity);



private:
	float maxHealth;
};

namespace Create
{
	Boss* SpawnBoss(Vector3 position, const std::string& _meshName, Vector3 scale = Vector3(1, 1, 1));
};

#endif // !BOSS_H