#include "PlayerInfo.h"
#include <iostream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../WeaponInfo/Pistol.h"
#include "../WeaponInfo/Shotgun.h"
#include "../WeaponInfo/Rifle.h"
#include "../WeaponInfo/Bow.h"
#include "../WeaponManager.h"
#include "../CollisionManager.h"
#include "../EntityManager.h"
#include "MeshList.h"
#include "../Application.h"
#include "../Minimap/Minimap.h"

// Allocating and initializing Player's static data member.  
// The pointer is allocated but not the object's constructor.

Player::Player(void)
	: m_dSpeed(10.0)
	, m_dAcceleration(10.0)
	, m_dElapsedTime(0.0)
	, attachedCamera(NULL)
	, m_pTerrain(NULL)
	, primaryWeapon(NULL)
	, m_bDodge(false)
	, m_dRollTime(0.0)
	, m_bMoving(false)
	, weaponIndex(0)
	, m_fHealth(100.f)
	, m_dAnimElapsedTime(0.0)
	, m_bLookingUp(false)
{
	playerInventory = new Inventory;
	playerInventory->addWeaponToInventory(new Pistol(GenericEntity::PLAYER_BULLET));
	playerInventory->addWeaponToInventory(new Rifle(GenericEntity::PLAYER_BULLET));
	playerInventory->addWeaponToInventory(new Bow(GenericEntity::PLAYER_BULLET));
	playerInventory->addWeaponToInventory(new Shotgun(GenericEntity::PLAYER_BULLET));
}

Player::~Player(void)
{
	m_pTerrain = NULL;
}

// Initialise this class instance
void Player::Init(void)
{
	// Set the default values
	defaultPosition.Set(0, 0, 0);

	// Set the current values
	position.Set(0, 0, 0);

	// Set direction
	direction.SetZero();

	// Set view direction
	view.SetZero();

	// Set Boundary
	maxBoundary.Set(1,1,1);
	minBoundary.Set(-1, -1, -1);

	//set weaponIndex
	weaponIndex = 0;
	//init weapon
	//primaryWeapon = new Pistol(GenericEntity::PLAYER_BULLET);
	//primaryWeapon->Init();

	this->SetCollider(true);

	// Audio Related adding sound
	AudioEngine::GetInstance()->Init();
	AudioEngine::GetInstance()->AddSound("testjump", "Audio/Mario-jump-sound.mp3");

	playerAnimated = new GenericEntity*[8];
	for (size_t i = 0; i < 8; i++)
	{
	playerAnimated[i] = new GenericEntity();
	}
	playerAnimated[0]->SetMesh(MeshList::GetInstance()->GetMesh("Player_fstand1"));
	playerAnimated[1]->SetMesh(MeshList::GetInstance()->GetMesh("Player_fstand2"));
	playerAnimated[2]->SetMesh(MeshList::GetInstance()->GetMesh("Player_bstand1"));
	playerAnimated[3]->SetMesh(MeshList::GetInstance()->GetMesh("Player_bstand2"));
	playerAnimated[4]->SetMesh(MeshList::GetInstance()->GetMesh("Player_fwalk1"));
	playerAnimated[5]->SetMesh(MeshList::GetInstance()->GetMesh("Player_fwalk2"));
	playerAnimated[6]->SetMesh(MeshList::GetInstance()->GetMesh("Player_bwalk1"));
	playerAnimated[7]->SetMesh(MeshList::GetInstance()->GetMesh("Player_bwalk2"));
	this->SetIndices_fStand(0, 1);
	this->SetIndices_bStand(2, 3);
	this->SetIndices_fWalk(4, 5);
	this->SetIndices_bWalk(6, 7);
}

// Set position
void Player::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set the boundary for the player info
void Player::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

// Set the terrain for the player info
void Player::SetTerrain(GroundEntity* m_pTerrain)
{






}

// Reset this player instance to default
void Player::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
}

// Get position x of the player
Vector3 Player::GetPos(void) const
{
	return position;
}

void Player::MoveUp()
{
	if (direction.y != -1)
		direction.y = 1;
	else
		direction.y = 0;
	m_bMoving = true;
}

void Player::MoveDown()
{
	if (direction.y != 1)
		direction.y = -1;
	else
		direction.y = 0;
	m_bMoving = true;
}

void Player::MoveLeft()
{
	if (direction.x != 1)
		direction.x = -1;
	else
		direction.x = 0;
	m_bMoving = true;
}

void Player::MoveRight()
{
	if (direction.x != -1)
		direction.x = 1;
	else
		direction.x = 0;
	m_bMoving = true;
}

void Player::SetMovement(bool _movement)
{
	m_bMoving = _movement;
}

/********************************************************************************
 Hero Update
 ********************************************************************************/

void Player::CollisionResponse(GenericEntity* thatEntity)
{
	switch (thatEntity->type) {
	case WALL:
		std::cout << "collide" << std::endl;
		return;
	case ENEMY:
		std::cout << "enemy collide" << std::endl;
		return;
	case ENEMY_BULLET:
		std::cout << "player hit by enemy bullet" << std::endl;
		EditHealth(-10);
		return;

	default:
		break;
	}
}
void Player::CollisionCheck_Movement()
{
	Vector3 tempMax = this->GetMaxAABB();
	Vector3 tempMin = this->GetMinAABB();
	std::list<EntityBase*> cpy = EntityManager::GetInstance()->getCollisionList();

	float checkby = 0;
	
	if (!isDodging())
		checkby = 0.2f;
	else
		checkby = 0.5f;

	if (direction.y == 1)
	{
		this->SetAABB(tempMax + Vector3(0.f, checkby, 0.f), tempMin + Vector3(0.f, checkby, 0.f));

		std::list<EntityBase*>::iterator it, end;
		end = cpy.end();

		for (it = cpy.begin(); it != end; ++it)
		{
			if (CollisionManager::GetInstance()->CheckAABBCollision(this, *it))
			{
				GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it);
				if (thatEntity->type == WALL || thatEntity->type == ENEMY)
				{
					std::cout << "Something is blocking up" << std::endl;
					direction.y = 0;
					break;
				}
			}
		}
		
		this->SetAABB(tempMax, tempMin);
	}
	else if (direction.y == -1)
	{
		this->SetAABB(tempMax - Vector3(0.f, checkby, 0.f), tempMin - Vector3(0.f, checkby, 0.f));

		std::list<EntityBase*>::iterator it, end;
		end = cpy.end();

		for (it = cpy.begin(); it != end; ++it)
		{
			if (CollisionManager::GetInstance()->CheckAABBCollision(this, *it))
			{
				GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it);
				if (thatEntity->type == WALL || thatEntity->type == ENEMY)
				{
					std::cout << "Something is blocking down" << std::endl;
					direction.y = 0;
					break;
				}
			}
		}

		this->SetAABB(tempMax, tempMin);
	}
	if (direction.x == 1)
	{
		this->SetAABB(tempMax + Vector3(checkby, 0.f, 0.f), tempMin + Vector3(checkby, 0.f, 0.f));
	
		std::list<EntityBase*>::iterator it, end;
		end = cpy.end();
	
		for (it = cpy.begin(); it != end; ++it)
		{
			if (CollisionManager::GetInstance()->CheckAABBCollision(this, *it))
			{
				GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it);
				if (thatEntity->type == WALL || thatEntity->type == ENEMY)
				{
					std::cout << "Something is blocking right" << std::endl;
					direction.x = 0;
					break;
				}
			}
		}

		this->SetAABB(tempMax, tempMin);
	}
	else if (direction.x == -1)
	{
		this->SetAABB(tempMax - Vector3(checkby, 0.f, 0.f), tempMin - Vector3(checkby, 0.f, 0.f));

		std::list<EntityBase*>::iterator it, end;
		end = cpy.end();

		for (it = cpy.begin(); it != end; ++it)
		{
			if (CollisionManager::GetInstance()->CheckAABBCollision(this, *it))
			{
				GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it);
				if (thatEntity->type == WALL || thatEntity->type == ENEMY)
				{
					std::cout << "Something is blocking left" << std::endl;
					direction.x = 0;
					break;
				}
			}
		}

		this->SetAABB(tempMax, tempMin);
	}

	if (direction.x == 0 && direction.y == 0)
		SetMovement(false);
	else
		SetMovement(true);
}

void Player::Update(double dt)
{
	m_dElapsedTime += dt;
	m_dAnimElapsedTime += dt * 10;

	if (attachedCamera == NULL)
		std::cout << "No camera attached! Please make sure to attach one" << std::endl;

	// Update the position if the WASD buttons were activated
	//( SHOULDNT BE USING THIS SINCE WE HAVE CONTROLLER )
	//( *ONLY APPLIES TO KEYBOARD INPUT, MOUSE STILL WRITE HERE* )
	/*if (KeyboardController::GetInstance()->IsKeyDown('W') ||
		KeyboardController::GetInstance()->IsKeyDown('A') ||
		KeyboardController::GetInstance()->IsKeyDown('S') ||
		KeyboardController::GetInstance()->IsKeyDown('D'))
	{
		m_bMoving = true;
	}*/

	MouseController::GetInstance()->GetMousePosition(x, y);
	w = Application::GetInstance().GetWindowWidth();
	h = Application::GetInstance().GetWindowHeight();
	x = x + Player::GetInstance()->GetPos().x - (w * 0.5f);
	y = y - Player::GetInstance()->GetPos().y + (h * 0.5f);

	if (y <= h) //W.I.P - my got shitty math to compare cursor pos.y with mid of screen size
		m_bLookingUp = true;
	else
		m_bLookingUp = false;
	SetAnimationStatus(m_bLookingUp, m_bMoving, dt);

	if (direction.x == 0 && direction.y == 0)
		SetMovement(false);
	else
		SetMovement(true);
	
	// if Mouse Buttons were activated, then act on them
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
	{
	}
	
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::RMB))
	{
		if (m_dElapsedTime > (m_dRollTime + 0.43f) && !m_bDodge && m_bMoving) // cooldown on spamming roll really quickly
		{
			// SUPAH COOL
			setDodge(true);
			m_dSpeed = 30;
			m_dRollTime = m_dElapsedTime + 0.07f; // 0.07 seems like a good time tbh
			std::cout << "ROLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL" << std::endl;

			AudioEngine::GetInstance()->editVolume(-10); // just a random button to test if edit volume is working (spoiler: it is)
		}
	}

	CollisionCheck_Movement();

	if ((m_dElapsedTime > m_dRollTime && m_bDodge) || !m_bMoving)
	{
		setDodge(false);
		m_dSpeed = 10;
	}

	if (m_bMoving)
	{
		position += direction * (float)m_dSpeed * (float)dt;
		if (!m_bDodge)
		{
			direction.SetZero();
			SetMovement(false);
		}
	}

	// If the user presses R key, then reset the view to default values
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
	{
		Reset(); // I dont think we need this but w/e maybe we might use it :thinking:
	}

	// If a camera is attached to this playerInfo class, then update it
	if (attachedCamera)
	{
		Vector3 cameraView = attachedCamera->GetCameraTarget() - attachedCamera->GetCameraPos();
		attachedCamera->SetCameraPos(position + Vector3(0, 0, 15));
		attachedCamera->SetCameraTarget(position);
		attachedCamera->Update(dt);
	}

	this->SetPosition(position);
	this->SetAABB(Vector3(this->GetScale().x * 0.3f, this->GetScale().y * 0.4f, this->GetScale().z * 0.5f) + GetPos(),
		Vector3(this->GetScale().x * -0.3f, this->GetScale().y * -0.4f, this->GetScale().z * -0.5f) + GetPos());


	for (std::list<EntityBase*>::iterator it = EntityManager::GetInstance()->getCollisionList().begin()
		;it != EntityManager::GetInstance()->getCollisionList().end();++it)
	{
		if (dynamic_cast<GenericEntity*>((*it))->type != GenericEntity::WALL)
			continue;

		CMinimap::GetInstance()->setObject((*it)->GetPosition() - position,(*it)->GetScale());
	}
	//for (int i = 0; i < NUM_ENEMY; ++i)
	//{
	//	//if ((anEnemy3D[i]->GetPos() - playerInfo->GetPos()).LengthSquared() < 100 * 100)
	//	//{
	//	//cout << "IN RANGE: " << (anEnemy3D->GetPos() - playerInfo->GetPos()).Length() << endl;
	//	theMinimap->setEnemyPos(anEnemy3D[i]->GetPos() - playerInfo->GetPos(), i);
	//	//cout << anEnemy3D->GetPos() - playerInfo->GetPos() << endl;
	//	//}
	//}
}

// Constrain the position within the borders
void Player::Constrain(void)
{
}

FPSCamera * Player::getCamera()
{
	return attachedCamera;
}

void Player::AttachCamera(FPSCamera* _cameraPtr)
{
	attachedCamera = _cameraPtr;
	Vector3 target = position + Vector3(1, 0, 0);
	Vector3 view = target - position;
	Vector3 up = Vector3(0,0,1).Cross(view).Normalized();
	attachedCamera->Init(position, target, up);
	//std::cout << up << std::endl;
}

void Player::DetachCamera()
{
	attachedCamera = nullptr;
}

bool Player::isDodging(void)
{
	return m_bDodge;
}

void Player::setDodge(bool _dodge)
{
	this->m_bDodge = _dodge;
}

// Shoot Weapon
bool Player::Shoot(const float dt)
{	
	playerInventory->getWeaponList()[weaponIndex]->Discharge(position, view); //position of player, dir to shoot from
	AudioEngine::GetInstance()->PlayASound("testjump", false);
	return false;
}

// Reload Weapon
bool Player::Reload(const float dt)
{
	playerInventory->getWeaponList()[weaponIndex]->Reload(); //reload gun
	std::cout << "TotalAmmo: " << playerInventory->getWeaponList()[weaponIndex]->GetTotalRound() << std::endl;
	return false;
}

// Change Weapon
bool Player::ChangeWeapon(const float dt)
{	
	++weaponIndex;
	weaponIndex = Math::Wrap(weaponIndex, 0, (int)playerInventory->getWeaponList().size() - 1);
	std::cout << "weaponIndex: " << weaponIndex << std::endl;
	return false;
}

void Player::SetView(Vector3 _view)
{
	this->view = _view;
}

void Player::SetHealth(float _health)
{
	this->m_fHealth = _health;
}

float Player::GetHealth()
{
	return m_fHealth;
}

void Player::EditHealth(float _health)
{
	this->m_fHealth += _health;
}

GenericEntity ** Player::GetPlayerAnimated()
{
	return playerAnimated;
}