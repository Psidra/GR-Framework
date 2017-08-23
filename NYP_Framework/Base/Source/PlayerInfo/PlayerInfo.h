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
	// Use Blank
	void UseBlank();

	// Set view direction
	void SetView(Vector3 _view);
	// Get view direction
	Vector3 GetView();

	// Collision Response
	void CollisionResponse(GenericEntity* thatEntity);
	// Movement Collision Check
	void CollisionCheck_Movement();

	// Set Max Health
	void SetMaxHealth(float _maxHealth);
	// Get Max Health
	float GetMaxHealth();
	// Edit Max Health
	void EditMaxHealth(float _value);

	// Set Health
	void SetHealth(float _health);
	// Get Health
	float GetHealth();
	// Edit Health
	void EditHealth(float _health);

	// Set Blanks
	void SetBlanks(int _blanks); // blank doesnt even look like a word anymore
	// Get Blanks				 // thanks semanic satiation
	int GetBlanks();
	// Edit Blanks
	void EditBlanks(int _blanks);

	// Set Money
	void SetMoney(int _money);
	// Get Money
	int GetMoney();
	// Edit Money
	void EditMoney(int _money);

	//Get player animation
	GenericEntity** GetPlayerAnimated(); 

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

	float m_fHealth;
	float m_fMaxHealth;

	FPSCamera* attachedCamera;
	CWeaponInfo* primaryWeapon;
	Inventory* playerInventory;

	int weaponIndex;
	int m_iBlank; // projectile remover 101
	int m_iMoney; // doshhhh
	int m_iKeys; // Not sure if we will keep this or not

	double m_dAnimElapsedTime; //bouncetime for animation sequence - used in old anim for now
	GenericEntity** playerAnimated;

	double x, y;			//cursor pos
	int w, h;				//screen width & height	
	bool m_bLookingUp;		//checks if player is looking up or down
};
