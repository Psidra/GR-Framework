#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <list>
#include "Enemy.h"
#include "SingletonTemplate.h"

class EnemyManager : public Singleton<EnemyManager>
{
	friend Singleton<EnemyManager>;
public:
	EnemyManager();
	~EnemyManager();

	void Update(double dt = 0.0333f);
	void Render();

	void spawnEnemy(Vector3 position, CStrategy* _strategy, const string& _meshName, float _health = 100.f, double _speed = 2.0);
	std::list<CEnemy*> EnemyList;

private:

};

#endif // ENEMYMANAGER_H