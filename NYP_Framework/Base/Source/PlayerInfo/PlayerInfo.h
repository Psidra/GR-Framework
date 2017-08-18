#pragma once
#include "Vector3.h"
#include "../FPSCamera.h"
#include "../GroundEntity.h"
#include "SingletonTemplate.h"
#include "../GenericEntity.h"
#include "../Animation.h"
#include "../Inventory.h"
#include "../AudioEngine.h"

class CWeaponInfo;

class Player : public Singleton<Player>, public GenericEntity, public CAnimation
{
	friend Singleton<Player>;
public:
	Player();
	~Player();

	// Initialise this class instance
	void Init(void);
	// Reset this player instance to default
	void Reset(void);

	// Set position
	void SetPos(const Vector3& pos);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);

	// Get position
	Vector3 GetPos(void) const;

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
	// Reload Weapon
	bool Reload(const float dt);
	// Change Weapon
	bool ChangeWeapon(const float dt);

	void animate(double dt); //run animation for player
	// Set view direction
	void SetView(Vector3 _view);

	// Collision Response
	void CollisionResponse(GenericEntity* thatEntity);
	// Movement Collision Check
	void CollisionCheck_Movement();

	// Set Health
	void SetHealth(float _health);
	// Get Health
	float GetHealth();
	// Edit Health
	void EditHealth(float _health);

	bool usingOldAnim; //toggle old animations
	GenericEntity** GetPlayerAnimated(); //Get player animation

private:
	Vector3 defaultPosition;
	Vector3 position, direction, view; // direction is walking dir, view is where player aims (cursor)
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;

	double m_dSpeed;
	double m_dAcceleration;

	double m_dElapsedTime;

	bool m_bMoving;
	bool m_bDodge;
	double m_dRollTime; // Bouncetime for rolling

	int m_iAnimIndex;
	float m_fHealth;

	FPSCamera* attachedCamera;
	CWeaponInfo* primaryWeapon;
	Inventory* playerInventory;

	int weaponIndex;

	double m_dAnimElapsedTime; //bouncetime for animation sequence - used in old anim for now
	GenericEntity** playerAnimated;

	double x, y;			//cursor pos
	int w, h;				//screen width & height	
	bool m_bLookingUp;		//checks if player is looking up or down
};
