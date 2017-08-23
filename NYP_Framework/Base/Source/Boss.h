#ifndef BOSS_H
#define BOSS_H

#include "GenericEntity.h"
#include "Strategy.h"
#include "Animation.h"
#include "MeshBuilder.h"
#include "EntityManager.h"

class Boss : public GenericEntity, public CAnimation
{
public:
	Boss();
	Boss(Vector3 pos);
	~Boss();

	// Collision Response
	void CollisionResponse(GenericEntity* thatEntity);

	// Set Max HP
	void setMaxHealth(float _health);
	// Get Max HP
	float getMaxHealth();
	// Edit Max HP
	void editMaxHealth(float _health);

	// Set HP
	void setHealth(float _health);
	// Get HP
	float getHealth();
	// Edit HP
	void editHealth(float _health);

private:
	Vector3 position;
	Vector3 direction;

	double m_dSpeed;

	float m_fHealth;
	float m_fMaxHealth;

	CStrategy* theStrategy;
};

namespace Create
{
	Boss* SpawnBoss(Vector3 position, const std::string& _meshName, Vector3 scale = Vector3(1, 1, 1));
};

#endif // !BOSS_H