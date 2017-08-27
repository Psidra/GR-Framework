#include "Boss.h"
#include "Enemy.h"

#include "WeaponManager.h"

#include "WeaponInfo\WeaponInfo.h"
#include "WeaponInfo\Pistol.h"
#include "WeaponInfo/Rifle.h"
#include "WeaponInfo\Shotgun.h"
#include "WeaponInfo/Bow.h"
#include "WeaponInfo/LaserBeam.h"

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
	for (int i = 0; i < 10; i++)
	{
		delete enemyAnimated[i];
		enemyAnimated[i] = NULL;
	}
	WeaponManager::GetInstance()->removeWeapon(enemyInventory->getWeaponList()[weaponIndex]);
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
	enemyInventory->addWeaponToInventory(new Shotgun(GenericEntity::ENEMY_BULLET));
	enemyInventory->addWeaponToInventory(new Bow(GenericEntity::ENEMY_BULLET));

	enemyInventory->getWeaponList()[weaponIndex]->setIsActive(true);

	for (size_t i = 0; i < 10; ++i)
	{
		Create::Enemy(Vector3(0, 0, 0), "player", Vector3(1, 1, 1), false);
	}
}

void Boss::SetTypeOfEnemy(int _enemyType)
{
	enemyAnimated = new GenericEntity*[10];
	for (size_t i = 0; i < 10; i++)
	{
		this->enemyAnimated[i] = new GenericEntity();
	}
	enemyInventory = new Inventory;

	switch (_enemyType) //WIP - set choice of units to spawn
	{
	case 1: //red minion
		enemyAnimated[0]->SetMesh(MeshList::GetInstance()->GetMesh("enemy1_fstand1"));
		enemyAnimated[1]->SetMesh(MeshList::GetInstance()->GetMesh("enemy1_fstand2"));
		enemyAnimated[2]->SetMesh(MeshList::GetInstance()->GetMesh("enemy1_bstand1"));
		enemyAnimated[3]->SetMesh(MeshList::GetInstance()->GetMesh("enemy1_bstand2"));
		enemyAnimated[4]->SetMesh(MeshList::GetInstance()->GetMesh("enemy1_fwalk1"));
		enemyAnimated[5]->SetMesh(MeshList::GetInstance()->GetMesh("enemy1_fwalk2"));
		enemyAnimated[6]->SetMesh(MeshList::GetInstance()->GetMesh("enemy1_bwalk1"));
		enemyAnimated[7]->SetMesh(MeshList::GetInstance()->GetMesh("enemy1_bwalk2"));
		enemyAnimated[8]->SetMesh(MeshList::GetInstance()->GetMesh("Player_fHurt"));
		enemyAnimated[9]->SetMesh(MeshList::GetInstance()->GetMesh("Player_bHurt"));
		
		break;
	case 2:
		for (size_t i = 0; i < 8; i++)
		{
			this->enemyAnimated[i]->SetMesh(MeshList::GetInstance()->GetMesh("player"));
		}
		break;
	default:
		break;
	}
	this->SetIndices_fStand(0, 1);
	this->SetIndices_bStand(2, 3);
	this->SetIndices_fWalk(4, 5);
	this->SetIndices_bWalk(6, 7);
	this->SetIndices_fHurt(8, 8);
	this->SetIndices_bHurt(9, 9);
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
			std::cout << "boss shot" << std::endl;
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
	}
	if (health <= 0)
		this->SetIsDone(true);
	if (isHurt == true)
		hurtElapsedTime += dt;
	if (hurtElapsedTime > 1.5)
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

//void Boss::CollisionResponse(GenericEntity * thatEntity)
//{
//}

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