#pragma once
#include "Vector3.h"
#include "../FPSCamera.h"
#include "../GroundEntity.h"
#include "SingletonTemplate.h"
#include "../GenericEntity.h"
#include "../Animation.h"

class CWeaponInfo;

class Player : public Singleton<Player>, public GenericEntity, public CAnimation
{
	friend Singleton<Player>;
public:
	Player();
	~Player();

	// Initialise this class instance
	void Init(void);
	// Returns true if the player is on ground
	bool isOnGround(void);
	// Returns true if the player is jumping upwards
	bool isJumpUpwards(void);
	// Returns true if the player is on freefall
	bool isFreeFall(void);
	// Set the player's status to free fall mode
	void SetOnFreeFall(bool isOnFreeFall);
	// Set the player to jumping upwards
	void SetToJumpUpwards(bool isOnJumpUpwards);
	// Stop the player's vertical movement
	void StopVerticalMovement(void);
	// Reset this player instance to default
	void Reset(void);

	// Set position
	void SetPos(const Vector3& pos);
	// Set m_dJumpSpeed of the player
	void SetJumpSpeed(const double m_dJumpSpeed);
	// Set m_dJumpAcceleration of the player
	void SetJumpAcceleration(const double m_dJumpAcceleration);
	// Set Fall Speed of the player
	void SetFallSpeed(const double m_dFallSpeed);
	// Set Fall Acceleration of the player
	void SetFallAcceleration(const double m_dFallAcceleration);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);

	// Get position
	Vector3 GetPos(void) const;
	// Get Jump Speed of the player
	double GetJumpSpeed(void) const;
	// Get Jump Acceleration of the player
	double GetJumpAcceleration(void) const;
	// Get Fall Speed of the player
	double GetFallSpeed(void) const;
	// Get Fall Acceleration of the player
	double GetFallAcceleration(void) const;

	// Move Up
	void MoveUp();
	// Move Down
	void MoveDown();
	// Move Left
	void MoveLeft();
	// Move Right
	void MoveRight();
	// Set Movement
	void SetMovement(bool _movement);

	// Update Jump Upwards
	void UpdateJumpUpwards(double dt = 0.0333f);
	// Update FreeFall
	void UpdateFreeFall(double dt = 0.0333f);
	// Update
	void Update(double dt = 0.0333f);

	// Constrain the position within the borders
	void Constrain(void);

	// Handling Camera
	FPSCamera* getCamera();
	void AttachCamera(FPSCamera* _cameraPtr);
	void DetachCamera();

	// return true if dodge rolling
	bool isDodging(void);
	// toggle dodge
	void setDodge(bool _dodge);

	// Shoot Weapon
	bool Shoot(const float dt);


	//void animate(double dt);
	GenericEntity** playerAnimated;
	int GetAnimIndex();
	// Set view direction
	void SetView(Vector3 _view);

private:
	Vector3 defaultPosition;
	Vector3 position, direction, view; // direction is walking dir, view is where player aims (cursor)
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;

	double m_dSpeed;
	double m_dAcceleration;

	bool m_bJumpUpwards;
	double m_dJumpSpeed;
	double m_dJumpAcceleration;

	double m_dElapsedTime;

	double m_dFallSpeed;
	bool m_bFallDownwards;
	double m_dFallAcceleration;

	bool m_bMoving;
	bool m_bDodge;
	double m_dRollTime; // Bouncetime for rolling

	int m_iAnimIndex;

	FPSCamera* attachedCamera;
	CWeaponInfo* primaryWeapon;

	//double anim_ElapsedTime;
};
