#include "PlayerInfo.h"
#include <iostream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../WeaponInfo/Pistol.h"
#include "../WeaponInfo/Shotgun.h"
#include "../CollisionManager.h"
#include "../EntityManager.h"
// Allocating and initializing Player's static data member.  
// The pointer is allocated but not the object's constructor.

Player::Player(void)
	: m_dSpeed(10.0)
	, m_dAcceleration(10.0)
	, m_bJumpUpwards(false)
	, m_dJumpSpeed(10.0)
	, m_dJumpAcceleration(-10.0)
	, m_bFallDownwards(false)
	, m_dFallSpeed(0.0)
	, m_dFallAcceleration(-10.0)
	, m_dElapsedTime(0.0)
	, attachedCamera(NULL)
	, m_pTerrain(NULL)
	, primaryWeapon(NULL)
	, m_bDodge(false)
	, m_dRollTime(0.0)
{
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

	//init weapon
	primaryWeapon = new Shotgun(GenericEntity::PLAYER_BULLET);
	primaryWeapon->Init();

	this->SetCollider(true);
	//this->type = GenericEntity::OBJECT_TYPE::PLAYER; // this doesnt even fuccin work lol
}

// Returns true if the player is on ground
bool Player::isOnGround(void)
{



	return false;
}

// Returns true if the player is jumping upwards
bool Player::isJumpUpwards(void)
{



	return false;
}

// Returns true if the player is on freefall
bool Player::isFreeFall(void)
{



	return false;
}

// Set the player's status to free fall mode
void Player::SetOnFreeFall(bool isOnFreeFall)
{





}

// Set the player to jumping upwards
void Player::SetToJumpUpwards(bool isOnJumpUpwards)
{





}

// Set position
void Player::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set m_dJumpAcceleration of the player
void Player::SetJumpAcceleration(const double m_dJumpAcceleration)
{
	this->m_dJumpAcceleration = m_dJumpAcceleration;
}

// Set Fall Acceleration of the player
void Player::SetFallAcceleration(const double m_dFallAcceleration)
{
	this->m_dFallAcceleration = m_dFallAcceleration;
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

// Stop the player's movement
void Player::StopVerticalMovement(void)
{
	m_bJumpUpwards = false;
	m_bFallDownwards = false;
}

// Reset this player instance to default
void Player::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;

	// Stop vertical movement too
	StopVerticalMovement();
}

// Get position x of the player
Vector3 Player::GetPos(void) const
{
	return position;
}

// Get m_dJumpAcceleration of the player
double Player::GetJumpAcceleration(void) const
{
	return m_dJumpAcceleration;
}

void Player::MoveUp()
{
	Vector3 tempMax = this->GetMaxAABB();
	Vector3 tempMin = this->GetMinAABB();

	this->SetAABB(tempMax + Vector3(0.f, 0.1f, 0.f), tempMin + Vector3(0.f, 0.1f, 0.f));

	std::list<EntityBase*> cpy = EntityManager::GetInstance()->getCollisionList();
	std::list<EntityBase*>::iterator it, end;

	end = cpy.end();
	for (it = cpy.begin(); it != end; ++it)
	{
		if (!CollisionManager::GetInstance()->CheckAABBCollision(this, *it))
		{
			if (direction.y != -1)
			{
				direction.y = 1;
				SetMovement(true);
			}
			else
				direction.y = 0;
		}
		else
		{
			std::cout << "Something is blocking up" << std::endl;
			if (direction.y == 1)
			{
				direction.y = 0;
				SetMovement(false);
			}
			break;
		}
	}

	this->SetAABB(tempMax, tempMin);
}

void Player::MoveDown()
{
	Vector3 tempMax = this->GetMaxAABB();
	Vector3 tempMin = this->GetMinAABB();

	this->SetAABB(tempMax - Vector3(0.f, 0.1f, 0.f), tempMin - Vector3(0.f, 0.1f, 0.f));

	std::list<EntityBase*> cpy = EntityManager::GetInstance()->getCollisionList();
	std::list<EntityBase*>::iterator it, end;

	end = cpy.end();
	for (it = cpy.begin(); it != end; ++it)
	{
		if (!CollisionManager::GetInstance()->CheckAABBCollision(this, *it))
		{
			if (direction.y != 1)
			{
				direction.y = -1;
				SetMovement(true);
			}
			else
				direction.y = 0;
		}
		else
		{
			std::cout << "Something is blocking down" << std::endl;
			if (direction.y == -1)
			{
				direction.y = 0;
				SetMovement(false);
			}
			break;
		}
	}

	this->SetAABB(tempMax, tempMin);
}

void Player::MoveLeft()
{
	//Vector3 up(0, 1, 0);
	//Vector3 rightUV;

	//rightUV = (direction.Normalized()).Cross(up);
	//rightUV.y = 0;
	//rightUV.Normalize();
	//position -= rightUV * (float)m_dSpeed * (float)dt;

	Vector3 tempMax = this->GetMaxAABB();
	Vector3 tempMin = this->GetMinAABB();

	this->SetAABB(tempMax - Vector3(0.1f, 0.f, 0.f), tempMin - Vector3(0.1f, 0.f, 0.f));

	std::list<EntityBase*> cpy = EntityManager::GetInstance()->getCollisionList();
	std::list<EntityBase*>::iterator it, end;

	end = cpy.end();
	for (it = cpy.begin(); it != end; ++it)
	{
		if (!CollisionManager::GetInstance()->CheckAABBCollision(this, *it))
		{
			if (direction.x != 1)
			{
				direction.x = -1;
				SetMovement(true);
			}
			else
				direction.x = 0;
		}
		else
		{
			std::cout << "Something is blocking left" << std::endl;
			if (direction.y == -1)
			{
				direction.y = 0;
				SetMovement(false);
			}
			break;
		}
	}

	this->SetAABB(tempMax, tempMin);
}

void Player::MoveRight()
{
	//Vector3 up(0, 1, 0);
	//Vector3 rightUV;

	//rightUV = (direction.Normalized()).Cross(up);
	//rightUV.y = 0;
	//rightUV.Normalize();
	//position += rightUV * (float)m_dSpeed * (float)dt;

	Vector3 tempMax = this->GetMaxAABB();
	Vector3 tempMin = this->GetMinAABB();

	this->SetAABB(tempMax + Vector3(0.1f, 0.f, 0.f), tempMin + Vector3(0.1f, 0.f, 0.f));

	std::list<EntityBase*> cpy = EntityManager::GetInstance()->getCollisionList();
	std::list<EntityBase*>::iterator it, end;

	end = cpy.end();
	for (it = cpy.begin(); it != end; ++it)
	{
		if (!CollisionManager::GetInstance()->CheckAABBCollision(this, *it))
		{
			if (direction.x != -1)
			{
				direction.x = 1;
				SetMovement(true);
			}
			else
				direction.x = 0;
		}
		else
		{
			std::cout << "Something is blocking right" << std::endl;
			if (direction.y == 1)
			{
				direction.y = 0;
				SetMovement(false);
			}
			break;
		}
	}

	this->SetAABB(tempMax, tempMin);
}

void Player::SetMovement(bool _movement)
{
	m_dMoving = _movement;
}

// Update Jump Upwards
void Player::UpdateJumpUpwards(double dt)
{
	if (m_bJumpUpwards == false)
		return;

	// Update the jump's elapsed time

	// Update position and target y values
	// Use SUVAT equation to update the change in position and target
	// s = u * t + 0.5 * a * t ^ 2


	// Use this equation to calculate final velocity, v
	// SUVAT: v = u + a * t;
	// v is m_dJumpSpeed AFTER updating using SUVAT where u is the initial speed and is equal to m_dJumpSpeed

	// Check if the jump speed is less than zero, then it should be falling
	if (m_dJumpSpeed < 0.0)
	{
	}
}

// Update FreeFall
void Player::UpdateFreeFall(double dt)
{
	if (m_bFallDownwards == false)
		return;

	// Update the jump's elapsed time

	// Update position and target y values
	// Use SUVAT equation to update the change in position and target
	// s = u * t + 0.5 * a * t ^ 2


	// Use this equation to calculate final velocity, v
	// SUVAT: v = u + a * t;
	// v is m_dJumpSpeed AFTER updating using SUVAT where u is the initial speed and is equal to m_dJumpSpeed

	// Check if the jump speed is below terrain, then it should be reset to terrain height
	if (position.y < m_pTerrain->GetTerrainHeight(position))
	{





	}
}

/********************************************************************************
 Hero Update
 ********************************************************************************/
void Player::Update(double dt)
{
	m_dElapsedTime += dt;
	//double mouse_diff_x, mouse_diff_y;
	//MouseController::GetInstance()->GetMouseDelta(mouse_diff_x, mouse_diff_y);

	//double camera_yaw = mouse_diff_x * 0.0174555555555556;		// 3.142 / 180.0
	//double camera_pitch = mouse_diff_y * 0.0174555555555556;	// 3.142 / 180.0

	//double mouse_pos_x, mouse_pos_y;
	//MouseController::GetInstance()->GetMousePosition(mouse_pos_x, mouse_pos_y);
	//view.Set(mouse_pos_x, mouse_pos_y, 0);

	if (attachedCamera == NULL)
		std::cout << "No camera attached! Please make sure to attach one" << std::endl;
	// This code was for first person camera direction
	//direction = attachedCamera->GetCameraTarget() - attachedCamera->GetCameraPos();
	//lock player movement to the ground only
	//direction.y = 0;
	//direction.Normalize();

	// This should be only used for mouse controls. Otherwise, use controller.cpp	

	//// Update the position if the WASD buttons were activated
	//if (KeyboardController::GetInstance()->IsKeyDown('W') ||
	//	KeyboardController::GetInstance()->IsKeyDown('A') ||
	//	KeyboardController::GetInstance()->IsKeyDown('S') ||
	//	KeyboardController::GetInstance()->IsKeyDown('D'))
	//{
	//	// Constrain position
	//	//Constrain();
	//}

	//// If the user presses SPACEBAR, then make him jump
	//if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE) &&
	//	position.y == m_pTerrain->GetTerrainHeight(position))
	//{

	//}

	//// Update the weapons
	//if (KeyboardController::GetInstance()->IsKeyReleased('R'))
	//{
	//}

	// if Mouse Buttons were activated, then act on them
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
	{

	}
	
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::RMB))
	{
		if (!m_bDodge)
		{
			// SUPAH COOL
			setDodge(true);
			m_dSpeed = 50;
			m_dRollTime = m_dElapsedTime + 0.25f;
			std::cout << "roll" << std::endl;
		}
	}

	if (m_dElapsedTime > m_dRollTime && m_bDodge)
	{
		setDodge(false);
		m_dSpeed = 10;
	}

	if (m_dMoving) // TODO: Add dodge roll :v
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
		Reset();
	}
	else
	{


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
	this->SetAABB(this->GetScale() * 0.5f + this->GetPos(), this->GetScale() * -0.5f + this->GetPos());

	primaryWeapon->Update(dt);
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
	primaryWeapon->Discharge(position, view); //position of player, dir to shoot from
	return false;
}

void Player::SetView(Vector3 _view)
{
	this->view = _view;
}
