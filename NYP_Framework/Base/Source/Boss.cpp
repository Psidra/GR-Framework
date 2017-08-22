#include "Boss.h"

Boss::Boss()
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
