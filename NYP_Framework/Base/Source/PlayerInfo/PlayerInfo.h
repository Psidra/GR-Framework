#pragma once
#include "Vector3.h"
#include "../FPSCamera.h"
#include "../GroundEntity.h"
#include "SingletonTemplate.h"
#include "../GenericEntity.h"
#include "../Animation.h"
#include "../Inventory.h"

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

	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);

	// Update
	void Update(double dt = 0.0333f);

	// Constrain the position within the borders
	void Constrain(void);

	// Handling Camera
	FPSCamera* getCamera();
	void AttachCamera(FPSCamera* _cameraPtr);
	void DetachCamera();

	// Shoot Weapon
	bool Shoot(const float dt);
	// Reload Weapon
	bool Reload(const float dt);
	// Change Weapon
	bool ChangeWeapon(const float dt);
	// Use Blank
	void UseBlank();

	// Collision Response
	void CollisionResponse(GenericEntity* thatEntity);
	// Movement Collision Check
	void CollisionCheck_Movement();

	// Move Up
	inline void MoveUp() { direction.y = ((direction.y != -1) ? 1 : 0); m_bMoving = true; };
	// Move Down
	inline void MoveDown() { direction.y = ((direction.y != 1) ? -1 : 0); m_bMoving = true; };
	// Move Left
	inline void MoveLeft() { direction.x = ((direction.x != 1) ? -1 : 0); m_bMoving = true; };
	// Move Right
	inline void MoveRight() { direction.x = ((direction.x != -1) ? 1 : 0); m_bMoving = true; };

	// Set Movement
	inline void SetMovement(bool _movement) { this->m_bMoving = _movement; };
	// Get Movement
	inline bool GetMovement() const { return this->m_bMoving; };

	// Set position
	inline void SetPos(const Vector3& pos) { this->position = pos; };
	// Get position
	inline Vector3 GetPos(void) const { return this->position; };

	// return true if dodge rolling
	inline bool isDodging(void) const { return this->m_bDodge; };
	// toggle dodge
	inline void setDodge(bool _dodge) { this->m_bDodge = _dodge; };

	// Set view direction
	inline void SetView(Vector3 _view) { this->view = _view; };
	// Get view direction
	inline Vector3 GetView() const { return this->view; };

	// Set Max Health
	inline void SetMaxHealth(float _maxHealth) { this->m_fMaxHealth = _maxHealth; };
	// Get Max Health
	inline float GetMaxHealth() const { return this->m_fMaxHealth; };
	// Edit Max Health
	inline void EditMaxHealth(float _value) { this->m_fMaxHealth += _value; };

	// Set Health
	inline void SetHealth(float _health) { this->m_fHealth = _health; };
	// Get Health
	inline float GetHealth() const { return this->m_fHealth; };
	// Edit Health
	inline void EditHealth(float _health) { if (this->m_fHealth > 0) this->m_fHealth += _health; };

	// Set Blanks
	inline void SetBlanks(int _blanks) { this->m_iBlank = _blanks; };	// blank doesnt even look like a word anymore
	// Get Blanks														// thanks semanic satiation
	inline int GetBlanks() const { return this->m_iBlank; };
	// Edit Blanks
	inline void EditBlanks(int _blanks) { this->m_iBlank += _blanks; };

	// Set Money
	inline void SetMoney(int _money) { this->m_iMoney = _money; };
	// Get Money
	inline int GetMoney() const { return this->m_iMoney; };
	// Edit Money
	inline void EditMoney(int _money) { this->m_iMoney += _money; };

	//Get player animation
	inline GenericEntity** GetPlayerAnimated() const { return this->playerAnimated; };

	// Set Fire (to the rain)
	inline void setFire(bool _lit) { this->m_bFire = _lit; }; // fam
	// Set Slow
	inline void setSlow(bool _slow) { this->m_bSlow = _slow; };
	// Set Poison
	inline void setPoison(bool _poison) { this->m_bPoison = _poison; };

	//Get player inventory
	inline Inventory* getInvetory() const { return this->playerInventory; };
	//get weapon index
	inline int getWeaponIndex() const { return this->weaponIndex; };
	//get weaponMesh
	inline Mesh* getWeaponMesh() const { return this->weaponMesh; };

	//reset player's ammo/hp
	void playerRest();

	// Boss fight effects on player
	bool m_bProjectileCircle;
	bool m_bPullEffect;
	bool m_bHunted; // Target on player

	//bool falg to new level
	static bool m_bNewLevel;
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

	int FireIntensity; // 0-5, 0 being not on fire, 5 being need to roll 5 times to put it out.

	bool m_bFire;	// STOP DROP AND ROLL (literally)
	bool m_bSlow;
	bool m_bPoison;

	double m_dFireTickUp;
	double m_dDmgOverTimeTick;
	double m_dPoisonDuration;
	double m_dPullEndKB;

	float m_fHealth;
	float m_fMaxHealth;

	FPSCamera* attachedCamera;
	CWeaponInfo* primaryWeapon;
	Inventory* playerInventory;

	int weaponIndex;
	int m_iBlank; // projectile remover 101
	int m_iMoney; // doshhhh
	int m_iKeys; // Not sure if we will keep this or not

	double hurtElapsedTime;		//bouncetime for hurt animation
	GenericEntity** playerAnimated;

	double x, y;			//cursor pos
	int w, h;				//screen width & height	
	bool m_bLookingUp;		//checks if player is looking up or down
	bool isHurt;
	Mesh* weaponMesh;
};
