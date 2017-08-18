#include "EnemyManager.h"
#include "EntityManager.h"

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Update(double dt)
{
	// Update Enemy
	std::list<CEnemy*>::iterator it, end;
	end = EnemyList.end();
	for (it = EnemyList.begin(); it != end; ++it)
	{
		(*it)->Update(dt);
	}

	// If enemy is killed, delete
	it = EnemyList.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = EnemyList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

void EnemyManager::Render()
{
	std::list<CEnemy*>::iterator it, end;
	end = EnemyList.end();
	for (it = EnemyList.begin(); it != end; ++it)
	{
		(*it)->Render();
	}
}

void EnemyManager::spawnEnemy(Vector3 _position, CStrategy* _strategy, const string& _meshName, float _health)
{
	CEnemy* Enemy = new CEnemy(_position, _health);
	Enemy->Init();
	Enemy->ChangeStrategy(_strategy, false);
	Enemy->SetMesh(MeshList::GetInstance()->GetMesh(_meshName), GenericEntity::OBJECT_TYPE::ENEMY);
	this->EnemyList.push_back(Enemy);
}