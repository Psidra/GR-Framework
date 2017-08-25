#include "EnemyBase.h"

#include "PlayerInfo\PlayerInfo.h"
#include "Enemy.h"

#include "Inventory.h"
#include "WeaponInfo\WeaponInfo.h"

#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "EntityBase.h"
#include "EntityManager.h"

#include <list>

void EnemyBase::SetTypeOfEnemy(int _enemyType)
{
}

void EnemyBase::Update(double dt)
{
}

void EnemyBase::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(enemyAnimated[GetAnimationIndex()]->GetMesh());
	modelStack.PopMatrix();
}

void EnemyBase::Shoot(double dt, Vector3 _pos)
{
	enemyInventory->getWeaponList()[weaponIndex]->Discharge(_pos, Player::GetInstance()->GetPos() - _pos);
}

void EnemyBase::Reload(double dt)
{
	this->enemyInventory->getWeaponList()[weaponIndex]->Reload();
}

void EnemyBase::SetSpeed(double speed)
{
	this->speed = speed;
}

void EnemyBase::SetPos(Vector3 _pos)
{
	this->position = _pos;
}

Vector3 EnemyBase::GetPos()
{
	return this->position;
}

void EnemyBase::setInvuln(bool _invul)
{
	this->invulnerable = _invul;
}

bool EnemyBase::getInvuln()
{
	return this->invulnerable;
}

void EnemyBase::setHealth(float _health)
{
	this->health = _health;
}

float EnemyBase::getHealth()
{
	return this->health;
}

void EnemyBase::editHealth(float _health)
{
	this->health += _health;
}

void EnemyBase::CollisionCheck()
{
}

void EnemyBase::CollisionResponse(GenericEntity * ThatEntity)
{
}

void EnemyBase::ChangeStrategy(CStrategy * theNewStrategy, bool bDelete)
{
	if (bDelete == true)
	{
		if (theStrategy != NULL)
		{
			delete theStrategy;
			theStrategy = NULL;
		}
	}

	theStrategy = theNewStrategy;
}

GenericEntity * EnemyBase::FetchEnemy()
{
	std::list<EntityBase*> cpy = EntityManager::GetInstance()->getCollisionList();
	std::list<EntityBase*>::iterator it;

	for (it = cpy.begin(); it != cpy.end(); ++it)
	{
		if ((*it)->IsActive())
			continue;

		GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it);

		if (thatEntity->type == GenericEntity::ENEMY)
			return thatEntity;
	}

	CEnemy* AnEnemy;

	for (size_t i = 0; i < 10; ++i)
	{
		AnEnemy = Create::Enemy(Vector3(0, 0, 0), "player", Vector3(1, 1, 1), false);
	}

	AnEnemy->SetIsActive(true);

	return AnEnemy;
}
