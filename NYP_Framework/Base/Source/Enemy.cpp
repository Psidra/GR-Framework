#include "Enemy.h"
#include "CollisionManager.h"
#include "WeaponInfo\Pistol.h"
#include "WeaponInfo\Shotgun.h"

/********************************************************************************
Constructor
********************************************************************************/
CEnemy::CEnemy() 
	:speed(1.0), 
	position(0,5,0), 
	health(50.f),
	weaponIndex(0),
	isShooting(false),
	m_bLookingUp(false),
	reloadElapsedTime(0.0),
	reloadDuration(3.0),
	hurtElapsedTime(0.0),
	isHurt(false)
{
}

CEnemy::CEnemy(Vector3 pos) : position(pos)
{
}

/********************************************************************************
Destructor
********************************************************************************/
CEnemy::~CEnemy(void)
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
}

void CEnemy::Init(float _hp, double _speed, int _enemyType)
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

	SetTypeOfEnemy(_enemyType);
}

void CEnemy::SetTypeOfEnemy(int _enemyType)
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
		enemyInventory->addWeaponToInventory(new Pistol(GenericEntity::ENEMY_BULLET));
		break;
	case 2:
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
		enemyInventory->addWeaponToInventory(new Shotgun(GenericEntity::ENEMY_BULLET));
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

void CEnemy::Update(double dt)
{
	this->SetPosition(this->position);
	this->SetAABB(this->GetScale() * 0.5f + this->GetPos(), this->GetScale() * -0.5f + this->GetPos());
	
	if (this->theStrategy != NULL)
	{
		this->theStrategy->Update(Player::GetInstance()->GetPos(), this->position, this->direction, this->speed, dt);
		this->CollisionCheck();
		this->position += this->direction * this->speed * (float)dt;
		
		if (this->theStrategy->GetIsShooting() && enemyInventory->getWeaponList()[weaponIndex]->GetMagRound() > 0)
			this->Shoot(dt);

		if (enemyInventory->getWeaponList()[weaponIndex]->GetMagRound() == 0)
			reloadElapsedTime += dt;
		if (reloadElapsedTime > reloadDuration)
		{
			Reload(dt);
			reloadElapsedTime = 0.0;
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
}

void CEnemy::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(enemyAnimated[GetAnimationIndex()]->GetMesh());
	modelStack.PopMatrix();
}

void CEnemy::Shoot(double dt)
{
	enemyInventory->getWeaponList()[weaponIndex]->Discharge(position, Player::GetInstance()->GetPos() - position);
	//std::cout << enemyInventory->getWeaponList()[weaponIndex]->GetMagRound() << "/" << enemyInventory->getWeaponList()[weaponIndex]->GetMaxMagRound() << std::endl;
}

void CEnemy::Reload(double dt)
{
	enemyInventory->getWeaponList()[weaponIndex]->Reload();
}


void CEnemy::SetSpeed(double speed)
{
	this->speed = speed;
}

float CEnemy::GetHP()
{
	return health;
}

Vector3 CEnemy::GetPos()
{
	return position;
}

void CEnemy::editHP(float _health)
{
	this->health += _health;
}

void CEnemy::CollisionCheck()
{
	Vector3 tempMax = this->GetMaxAABB();
	Vector3 tempMin = this->GetMinAABB();
	std::list<EntityBase*> cpy = EntityManager::GetInstance()->getCollisionList();

	float checkoffset = 0.5f;

	if (direction.y == 1) //up
	{
		this->SetAABB(tempMax + Vector3(0.f, checkoffset, 0.f), tempMin + Vector3(0.f, checkoffset, 0.f));

		std::list<EntityBase*>::iterator it, end;
		end = cpy.end();
		for (it = cpy.begin(); it != end; ++it)
		{
			if (CollisionManager::GetInstance()->CheckAABBCollision(this, *it))
			{
				if (this == (*it))
					continue;

				GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it);

				if (thatEntity->type == WALL || thatEntity->type == ENEMY )
				{
					std::cout << "Something is blocking up" << std::endl;
					direction.y = 0;
					break;
				}
			}
			else if (CollisionManager::GetInstance()->CheckAABBCollision(this, Player::GetInstance()))
			{
				std::cout << "Something is blocking enemy up" << std::endl;
				direction.y = 0;
				break;
			}
		}
		this->SetAABB(tempMax, tempMin);
	}
	else if (direction.y == -1)
	{
		this->SetAABB(tempMax - Vector3(0.f, checkoffset, 0.f), tempMin - Vector3(0.f, checkoffset, 0.f));

		std::list<EntityBase*>::iterator it, end;
		end = cpy.end();

		for (it = cpy.begin(); it != end; ++it)
		{
			if (CollisionManager::GetInstance()->CheckAABBCollision(this, *it))
			{
				if (this == (*it))
					continue;

				GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it);
				if (thatEntity->type == WALL || thatEntity->type == ENEMY )
				{
					std::cout << "Something is blocking down" << std::endl;
					direction.y = 0;
					break;
				}
			}
			else if (CollisionManager::GetInstance()->CheckAABBCollision(this, Player::GetInstance()))
			{
				std::cout << "Something is blocking enemy down" << std::endl;
				direction.y = 0;
				break;
			}
		}
		this->SetAABB(tempMax, tempMin);
	}
	if (direction.x == 1)
	{
		this->SetAABB(tempMax + Vector3(checkoffset, 0.f, 0.f), tempMin + Vector3(checkoffset, 0.f, 0.f));

		std::list<EntityBase*>::iterator it, end;
		end = cpy.end();

		for (it = cpy.begin(); it != end; ++it)
		{
			if (CollisionManager::GetInstance()->CheckAABBCollision(this, *it))
			{
				if (this == (*it))
					continue;

				GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it);
				if (thatEntity->type == WALL || thatEntity->type == ENEMY)
				{
					std::cout << "Something is blocking right" << std::endl;
					direction.x = 0;
					break;
				}
			}
			else if (CollisionManager::GetInstance()->CheckAABBCollision(this, Player::GetInstance()))
			{
				std::cout << "Something is blocking enemy right" << std::endl;
				direction.x = 0;
				break;
			}
		}
		this->SetAABB(tempMax, tempMin);
	}
	else if (direction.x == -1)
	{
		this->SetAABB(tempMax - Vector3(checkoffset, 0.f, 0.f), tempMin - Vector3(checkoffset, 0.f, 0.f));

		std::list<EntityBase*>::iterator it, end;
		end = cpy.end();

		for (it = cpy.begin(); it != end; ++it)
		{
			if (CollisionManager::GetInstance()->CheckAABBCollision(this, *it))
			{
				if (this == (*it))
					continue;

				GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it);
				if (thatEntity->type == WALL || thatEntity->type == ENEMY)
				{
					std::cout << "Something is blocking left" << std::endl;
					direction.x = 0;
					break;
				}
			}
			else if (CollisionManager::GetInstance()->CheckAABBCollision(this, Player::GetInstance()))
			{
				std::cout << "Something is blocking enemy left" << std::endl;
				direction.x = 0;
				break;
			}
		}
		this->SetAABB(tempMax, tempMin);
	}
}

void CEnemy::CollisionResponse(GenericEntity* thatEntity)
{
	switch (thatEntity->type) {
	//case GenericEntity::OBJECT_TYPE::PLAYER_BULLET:
	//	thatEntity->SetIsDone(true);
	//	editHP(-20);
	//	std::cout << "player bullet collide with enemy" << std::endl;
	//	break;
	case GenericEntity::OBJECT_TYPE::WALL:
		//std::cout << "enemy collide with wall" << std::endl;
		break;
	case GenericEntity::OBJECT_TYPE::ENEMY:
		//std::cout << "enemy collide with enemy" << std::endl;
		break;
	case GenericEntity::OBJECT_TYPE::PLAYER:
		//std::cout << "enemy collide with player" << std::endl;
		break;
	case GenericEntity::OBJECT_TYPE::PLAYER_BULLET:
		isHurt = true;
		std::cout << "enemy collide with player bullet" << std::endl;
		break;

	default:
		break;
	}
}


/********************************************************************************
Strategy
********************************************************************************/
void CEnemy::ChangeStrategy(CStrategy* theNewStrategy, bool bDelete)
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

CEnemy * Create::Enemy(Vector3 position, const string & _meshName, Vector3 scale)
{
	CEnemy* result = new CEnemy(position);
	result->SetMesh(MeshList::GetInstance()->GetMesh(_meshName));
	result->SetPosition(position);
	result->SetScale(scale);
	result->SetCollider(true);
	result->type = GenericEntity::OBJECT_TYPE::ENEMY;
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}
