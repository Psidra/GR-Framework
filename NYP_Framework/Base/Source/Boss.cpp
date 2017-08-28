#include "DetectMemoryLeak.h"
#include "Boss.h"
#include "Enemy.h"

#include "WeaponManager.h"

#include "WeaponInfo\WeaponInfo.h"
#include "WeaponInfo\CircularWeapon.h"
#include "WeaponInfo\FourSidedWeapon.h"
#include "WeaponInfo\Minigun.h"
#include "WeaponInfo\Pistol.h"

#include "Inventory.h"

#include "CollisionManager.h"
#include "Collider\Collider.h"

#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "GenericEntity.h"
#include "Animation.h"

#include "Strategy.h"

#include "PlayerInfo\PlayerInfo.h"
#include"AI FSM\Ai_1.h"
// no idea how many guns boss will have so w/e have 'em all

Boss::Boss()
{

}

Boss::Boss(Mesh* _mesh)
{
	this->SetMesh(_mesh, ENEMY);
}

Boss::~Boss()
{
	if (theStrategy != NULL)
	{
		delete theStrategy;
		theStrategy = NULL;
	}
	for (int i = 0; i < 9; i++)
	{
		delete enemyAnimated[i];
		enemyAnimated[i] = NULL;
	}
	delete[] enemyAnimated;
	//WeaponManager::GetInstance()->removeWeapon(enemyInventory->getWeaponList()[weaponIndex]);
	
	for (size_t i = 0; i < enemyInventory->getWeaponList().size(); ++i)
	{
		enemyInventory->removeWeaponFromInventory(enemyInventory->getWeaponList()[i]);
	}
	delete enemyInventory;
}

void Boss::Init(float _hp, double _speed, int _enemyType, bool _invul)
{
	direction.SetZero();
	this->SetCollider(true);
	this->SetSpeed(_speed);
	this->health = _hp;
	weaponIndex = 0;
	reloadElapsedTime = 0.0;
	reloadDuration = 3.0;
	hurtElapsedTime = 0.0;
	isHurt = false;
	this->invulnerable = _invul;

	SetTypeOfEnemy(_enemyType);

	enemyInventory->addWeaponToInventory(new Pistol(GenericEntity::ENEMY_BULLET));
	enemyInventory->addWeaponToInventory(new Minigun(GenericEntity::ENEMY_BULLET));
	enemyInventory->addWeaponToInventory(new CircularWeapon(GenericEntity::ENEMY_BULLET));

	enemyInventory->getWeaponList()[weaponIndex]->setIsActive(true);

	for (size_t i = 0; i < 10; ++i)
	{
		CEnemy* enemy = Create::Enemy(Vector3(0, 0, 0), "player", Vector3(1, 1, 1), false);
		enemy->Init();
		enemy->ChangeStrategy(new CStrategy_AI_1(), false);
	}
}

void Boss::SetTypeOfEnemy(int _enemyType)
{
	enemyAnimated = new GenericEntity*[9];
	for (size_t i = 0; i < 9; i++)
	{
		this->enemyAnimated[i] = new GenericEntity();
	}
	enemyInventory = new Inventory;

	switch (_enemyType) //WIP - set choice of units to spawn
	{
	case 1: //red minion
		enemyAnimated[0]->SetMesh(MeshList::GetInstance()->GetMesh("ccore_normal1"));
		enemyAnimated[1]->SetMesh(MeshList::GetInstance()->GetMesh("ccore_normal2"));
		enemyAnimated[2]->SetMesh(MeshList::GetInstance()->GetMesh("ccore_normal3"));
		enemyAnimated[3]->SetMesh(MeshList::GetInstance()->GetMesh("ccore_hurt1"));
		enemyAnimated[4]->SetMesh(MeshList::GetInstance()->GetMesh("ccore_hurt2"));
		enemyAnimated[5]->SetMesh(MeshList::GetInstance()->GetMesh("ccore_hurt3"));
		enemyAnimated[6]->SetMesh(MeshList::GetInstance()->GetMesh("ccore_succ1"));
		enemyAnimated[7]->SetMesh(MeshList::GetInstance()->GetMesh("ccore_succ2"));
		enemyAnimated[8]->SetMesh(MeshList::GetInstance()->GetMesh("ccore_succ3"));
		break;

	default:
		break;
	}
	this->SetIndices_fStand(0, 2);
	this->SetIndices_bStand(0, 2);
	this->SetIndices_fHurt(3, 5);
	this->SetIndices_bHurt(3, 5);
	this->SetIndices_Succ(6, 8);
}

void Boss::Update(double dt)
{
	this->SetPosition(this->position);
	this->shootPos = this->position;
	this->SetAABB((this->scale * 0.5f) + this->position, (this->scale * -0.5f) + this->position);

	if (this->theStrategy != NULL)
	{
		this->theStrategy->UpdateBoss(Player::GetInstance()->GetPos(), shootPos, this->direction, this->speed, this->weaponIndex, this->health, dt);
		this->CollisionCheck();
		this->position += this->direction * this->speed * (float)dt;

		if (this->theStrategy->GetIsShooting() && enemyInventory->getWeaponList()[weaponIndex]->GetMagRound() > 0)
		{
			this->Shoot(dt, shootPos);
		}

		if (enemyInventory->getWeaponList()[weaponIndex]->GetMagRound() == 0)
			reloadElapsedTime += dt;

		if (reloadElapsedTime > reloadDuration)
		{
			Reload(dt);
			reloadElapsedTime = 0.0;
			//++weaponIndex;
			//weaponIndex = Math::Wrap(weaponIndex, 0, 2);
		}

		if (Player::GetInstance()->m_bPullEffect)
		{
			Vector3 playerpos = Player::GetInstance()->GetPos();
			Vector3 bosspullpos = this->GetPos();
			Vector3 diff = playerpos - bosspullpos;

			playerpos += (bosspullpos - playerpos).Normalized() * 5.f * (float)dt;
			Player::GetInstance()->SetPos(playerpos);
		}
	}
	if (health <= 0)
		this->SetIsDone(true);
	if (isHurt)
		hurtElapsedTime += dt;
	if (hurtElapsedTime > 1.5f)
	{
		hurtElapsedTime = 0.0;
		isHurt = false;
	}

	this->SetAnimationStatus((Player::GetInstance()->GetPos().y > this->GetPos().y) ? true : false,
		this->theStrategy->GetIsMoving(), isHurt, dt);
	//set gun pos to enemy pos
	enemyInventory->getWeaponList()[weaponIndex]->setGunPos(position);
	enemyInventory->getWeaponList()[weaponIndex]->setGunDir(Player::GetInstance()->GetPos() - position);
}

void Boss::SetMaxHealth(float _health)
{
	this->maxHealth = _health;
}

float Boss::GetMaxHealth()
{
	return this->maxHealth;
}

void Boss::EditMaxHealth(float _health)
{
	this->maxHealth += _health;
}

void Boss::CollisionResponse(GenericEntity * thatEntity)
{
	switch (thatEntity->type) {
	case GenericEntity::OBJECT_TYPE::PLAYER_BULLET:
		isHurt = true;
		std::cout << "boss collide with player bullet" << std::endl;
		break;

	default:
		break;
	}
}

Boss * Create::SpawnBoss(Vector3 position, const std::string& _meshName, Vector3 scale, bool _isActive)
{
	Boss* result = new Boss(MeshList::GetInstance()->GetMesh(_meshName));
	result->SetPos(position);
	result->SetPosition(position);
	result->SetScale(scale);
	result->SetCollider(true);
	result->SetIsActive(_isActive);
	result->type = GenericEntity::OBJECT_TYPE::BOSS;
	EntityManager::GetInstance()->AddEntity(result, true);
	return result;
}