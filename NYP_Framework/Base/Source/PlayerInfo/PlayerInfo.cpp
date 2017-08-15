#include "PlayerInfo.h"
#include <iostream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../WeaponInfo/Pistol.h"
#include "../WeaponInfo/Shotgun.h"
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
	defaultPosition.Set(0,0,10);

	// Set the current values
	position.Set(0, 0, 10);

	// Set Boundary
	maxBoundary.Set(1,1,1);
	minBoundary.Set(-1, -1, -1);

	//init weapon
	primaryWeapon = new Shotgun();
	primaryWeapon->Init();
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

void Player::MoveUp(double dt)
{
	// If doing move front/back, direction.normalized()
	Vector3 up(0, 1, 0);
	position += up * (float)m_dSpeed * (float)dt;
}

void Player::MoveDown(double dt)
{
	Vector3 up(0, 1, 0);
	position -= up * (float)m_dSpeed * (float)dt;
}

void Player::MoveLeft(double dt)
{
	Vector3 up(0, 1, 0);
	Vector3 rightUV;

	rightUV = (direction.Normalized()).Cross(up);
	rightUV.y = 0;
	rightUV.Normalize();
	position -= rightUV * (float)m_dSpeed * (float)dt;
}

void Player::MoveRight(double dt)
{
	Vector3 up(0, 1, 0);
	Vector3 rightUV;

	rightUV = (direction.Normalized()).Cross(up);
	rightUV.y = 0;
	rightUV.Normalize();
	position += rightUV * (float)m_dSpeed * (float)dt;
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
	double mouse_diff_x, mouse_diff_y;
	MouseController::GetInstance()->GetMouseDelta(mouse_diff_x, mouse_diff_y);

	double camera_yaw = mouse_diff_x * 0.0174555555555556;		// 3.142 / 180.0
	double camera_pitch = mouse_diff_y * 0.0174555555555556;	// 3.142 / 180.0

	if (attachedCamera == NULL)
		std::cout << "No camera attached! Please make sure to attach one" << std::endl;
	direction = attachedCamera->GetCameraTarget() - attachedCamera->GetCameraPos();
	//lock player movement to the ground only
	//direction.y = 0;
	direction.Normalize();

	// Update the position if the WASD buttons were activated
	if (KeyboardController::GetInstance()->IsKeyDown('W') ||
		KeyboardController::GetInstance()->IsKeyDown('A') ||
		KeyboardController::GetInstance()->IsKeyDown('S') ||
		KeyboardController::GetInstance()->IsKeyDown('D'))
	{
		// Constrain position
		//Constrain();
	}

	// If the user presses SPACEBAR, then make him jump
	if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE) &&
		position.y == m_pTerrain->GetTerrainHeight(position))
	{

	}

	// Update the weapons
	if (KeyboardController::GetInstance()->IsKeyReleased('R'))
	{
	}

	// if Mouse Buttons were activated, then act on them
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
	{
	}
	else if (MouseController::GetInstance()->IsButtonPressed(MouseController::RMB))
	{

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
		attachedCamera->SetCameraPos(position + Vector3(0, 0, 10));
		attachedCamera->SetCameraTarget(position);
		attachedCamera->Update(dt);
	}
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

// Shoot Weapon
bool Player::Shoot(const float dt)
{	
	primaryWeapon->Discharge(position, Vector3(1,0,0)); //position of player, dir to shoot from
	return false;
}

