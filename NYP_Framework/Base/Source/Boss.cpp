#include "Boss.h"

Boss::Boss()
{

}

Boss::Boss(Vector3 pos) : position(pos)
{
}

Boss::~Boss()
{

}

void Boss::CollisionResponse(GenericEntity * thatEntity)
{
}

void Boss::setMaxHealth(float _health)
{
	this->m_fMaxHealth = _health;
}

float Boss::getMaxHealth()
{
	return this->m_fMaxHealth;
}

void Boss::editMaxHealth(float _health)
{
	this->m_fMaxHealth += _health;
}

void Boss::setHealth(float _health)
{
	this->m_fHealth = _health;
}

float Boss::getHealth()
{
	return this->m_fHealth;
}

void Boss::editHealth(float _health)
{
	this->m_fHealth += _health;
}

Boss * Create::SpawnBoss(Vector3 position, const std::string& _meshName, Vector3 scale)
{
	Boss* result = new Boss(position);
	result->SetMesh(MeshList::GetInstance()->GetMesh(_meshName));
	result->SetPosition(position);
	result->SetScale(scale);
	result->SetCollider(true);
	result->type = GenericEntity::OBJECT_TYPE::ENEMY;
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}
