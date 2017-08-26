#include "PlayerInfo.h"
#include <iostream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../WeaponInfo/Pistol.h"
#include "../WeaponInfo/Shotgun.h"
#include "../WeaponInfo/Rifle.h"
#include "../WeaponInfo/Bow.h"
#include "../WeaponInfo/LaserBeam.h"
#include "../WeaponManager.h"
#include "../CollisionManager.h"
#include "../EntityManager.h"
#include "MeshList.h"
#include "../Application.h"
#include "../Minimap/Minimap.h"
#include "../Particle/ParticleEffect.h"
#include "../WeaponInfo/CircularWeapon.h"

#include "../LevelStuff/QuadTree.h"
#include "../LevelStuff/Level.h"

// Allocating and initializing Player's static data member.  
// The pointer is allocated but not the object's constructor.

Player::Player(void)
	: m_dSpeed(10.0)
	, m_dAcceleration(10.0)
	, m_dElapsedTime(0.0)
	, hurtElapsedTime(0.0)
	, attachedCamera(NULL)
	, m_pTerrain(NULL)
	, primaryWeapon(NULL)
	, m_bDodge(false)
	, m_dRollTime(0.0)
	, m_bMoving(false)
	, weaponIndex(0)
	, m_fHealth(100.f)
	, m_bLookingUp(false)
	, m_iMoney(0) // me_irl
	, m_iBlank(2)
	, isHurt(false)
	, m_bFire(false)
	, m_bSlow(false)
	, m_bPoison(false)
	, weaponMesh(NULL)
{
	playerInventory = new Inventory;
	playerInventory->addWeaponToInventory(new Pistol(GenericEntity::PLAYER_BULLET));
	playerInventory->addWeaponToInventory(new Rifle(GenericEntity::PLAYER_BULLET));
	playerInventory->addWeaponToInventory(new Bow(GenericEntity::PLAYER_BULLET));
	playerInventory->addWeaponToInventory(new Shotgun(GenericEntity::PLAYER_BULLET));
	playerInventory->addWeaponToInventory(new LaserBeam(GenericEntity::PLAYER_BULLET));
	//this weapon for boss and enemies
	//playerInventory->addWeaponToInventory(new CircularWeapon(GenericEntity::PLAYER_BULLET));
}

Player::~Player(void)
{
	m_pTerrain = NULL;
	for (size_t i = 0;i < playerInventory->getWeaponList().size(); ++i)
	{
		playerInventory->removeWeaponFromInventory(playerInventory->getWeaponList()[i]);
	}
}

// Initialise this class instance
void Player::Init(void)
{
	this->SetIsActive(true);

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
	this->type == GenericEntity::OBJECT_TYPE::PLAYER;

	//set weaponIndex
	weaponIndex = 0;
	//init weapon
	//primaryWeapon = new Pistol(GenericEntity::PLAYER_BULLET);
	//primaryWeapon->Init();

	this->SetMaxHealth(100.f);

	this->SetCollider(true);

	this->FireIntensity = 0;

	// Audio Related adding sound
	AudioEngine::GetInstance()->Init();
	AudioEngine::GetInstance()->AddSound("testjump", "Audio/Mario-jump-sound.mp3");

	playerAnimated = new GenericEntity*[10];
	for (size_t i = 0; i < 10; i++)
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
	playerAnimated[8]->SetMesh(MeshList::GetInstance()->GetMesh("Player_fHurt"));
	playerAnimated[9]->SetMesh(MeshList::GetInstance()->GetMesh("Player_bHurt"));
	this->SetIndices_fStand(0, 1);
	this->SetIndices_bStand(2, 3);
	this->SetIndices_fWalk(4, 5);
	this->SetIndices_bWalk(6, 7);
	this->SetIndices_fHurt(8, 8);
	this->SetIndices_bHurt(9, 9);
	playerInventory->setWeaponIndex(weaponIndex);
	playerInventory->getPrimaryWeapon()->setIsActive(true);
	//playerInventory->getWeaponList()[weaponIndex]->setIsActive(true);
	weaponMesh = playerInventory->getPrimaryWeapon()->GetMesh();
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

/********************************************************************************
 Hero Update
 ********************************************************************************/

void Player::CollisionResponse(GenericEntity* thatEntity)
{
	switch (thatEntity->type) {
	case WALL:
		std::cout << "collide" << std::endl;
		break;
	case ENEMY:
		//std::cout << "enemy collide" << std::endl;
		break;
	case ENEMY_BULLET:
	{
		if (this->m_bDodge)
			break;

		//std::cout << "player hit by enemy bullet" << std::endl;
		Create::Particle("blood", this->position, 0, EFFECT_TYPE::ET_BLEED, 0.3, 0.5, true, this);

		thatEntity->SetIsDone(true);
		CProjectile* Proj = dynamic_cast<CProjectile*>(thatEntity);

		if (this->m_fHealth > 0)
			EditHealth(-Proj->getProjectileDamage());
		isHurt = true;
		break;
	}
	case PIT:
		// TODO
		break;
	case SPIKE:
		// TODO
		break;
	case FIRE:
		if (!this->m_bFire)
		{
			this->FireIntensity = 1;
			m_dFireTickUp = m_dElapsedTime + 2.f;
		}

		this->m_bFire = true;
		// oh shit nigga
		break;
	case SLOW:
		this->m_bSlow = true;
		break;
	case POISON:
		this->m_bPoison = true;
		this->m_dPoisonDuration = m_dElapsedTime + 3.f;
		break;

	default:
		break;
	}
}
void Player::CollisionCheck_Movement()
{
	Vector3 tempMax = this->GetMaxAABB();
	Vector3 tempMin = this->GetMinAABB();
	std::list<EntityBase*> cpy = EntityManager::GetInstance()->getCollisionList();

	QuadTree quadTree(0, Level::GetInstance()->getMapWidth(), Level::GetInstance()->getMapHeight(), 0);
	//QuadTree quadTree(0, 800, 600, 0, 3);
	vector<EntityBase*> getNearestObj;

	quadTree.clear();
	for (std::list<EntityBase*>::iterator it = cpy.begin(); it != cpy.end(); ++it)
		quadTree.addObject(*it);

	

	float checkby = 0;
	
	if (!isDodging())
		checkby = 0.2f;
	else
		checkby = 0.5f;

	if (direction.y != 0)
	{
		this->SetAABB(tempMax + Vector3(0.f, checkby, 0.f) * direction.y, tempMin + Vector3(0.f, checkby, 0.f) * direction.y);
		//getNearestObj = quadTree.getObjectsAt(this->position.x, this->position.y);
		getNearestObj = quadTree.queryRange(this->minAABB.x, this->maxAABB.x, this->maxAABB.y, this->minAABB.y);
		//getNearestObj = quadTree.queryRange(position.x - 5, position.x + 5, position.y + 5, position.x - 5);
		std::vector<EntityBase*>::iterator it, end;
		end = getNearestObj.end();
		std::cout << "Intial :: " << cpy.size() << "||" << "Checking :: " << getNearestObj.size() << std::endl;

		for (it = getNearestObj.begin(); it != end; ++it)
		{
			if (!(*it)->IsActive())
				continue;

			if (CollisionManager::GetInstance()->CheckAABBCollision(this, *it))
			{
				GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it);
				if (thatEntity->type == WALL || thatEntity->type == ENEMY)
				{
					//std::cout << "Something is blocking up" << std::endl;
					direction.y = 0;
					break;
				}
			}
		}
		
		this->SetAABB(tempMax, tempMin);
	}
	if (direction.x != 0)
	{
		this->SetAABB(tempMax + Vector3(checkby, 0.f, 0.f) * direction.x, tempMin + Vector3(checkby, 0.f, 0.f) * direction.x);
	
		getNearestObj = quadTree.queryRange(this->minAABB.x, this->maxAABB.x, this->maxAABB.y, this->minAABB.y);
		//getNearestObj = quadTree.queryRange(position.x - 5, position.x + 5, position.y + 5, position.x - 5);
		std::vector<EntityBase*>::iterator it, end;
		end = getNearestObj.end();
		std::cout << "Intial :: " << cpy.size() << "||" << "Checking :: " << getNearestObj.size() << std::endl;

		for (it = getNearestObj.begin(); it != end; ++it)
		{
			if (!(*it)->IsActive())
				continue;

			if (CollisionManager::GetInstance()->CheckAABBCollision(this, *it))
			{
				GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it);
				if (thatEntity->type == WALL || thatEntity->type == ENEMY)
				{
					//std::cout << "Something is blocking right" << std::endl;
					direction.x = 0;
					break;
				}
			}
		}

		this->SetAABB(tempMax, tempMin);
	}

	SetMovement(!(direction.x == 0 && direction.y == 0));
}

void Player::Update(double dt)
{
	m_dElapsedTime += dt;
	hurtElapsedTime += dt;

	if (attachedCamera == NULL)
		std::cout << "No camera attached! Please make sure to attach one" << std::endl;

	// STATUS EFFECTS
	if (FireIntensity == 0)
		this->m_bFire = false;

	if (this->m_bFire && m_dElapsedTime > m_dFireTickUp)
	{
		m_dFireTickUp = m_dElapsedTime + 2.f;

		if (FireIntensity < 5)
			++FireIntensity;

		std::cout << "ROLL YOU STUPID SHIT" << std::endl;
		std::cout << FireIntensity << std::endl;
	}

	if (this->m_bSlow)
	{
		this->m_dSpeed = 5.0;
		std::cout << "You are slowed!" << std::endl;
	}
	else
		this->m_dSpeed = 10.0;

	if (this->m_bPoison && m_dElapsedTime > m_dPoisonDuration)
	{
		this->m_bPoison = false;
	}

	if (this->m_bPoison)
		std::cout << "You are Poisoned for: " << m_dPoisonDuration - m_dElapsedTime << std::endl;

	if ((this->m_bFire || this->m_bPoison) && m_dElapsedTime > m_dDmgOverTimeTick)
	{
		m_dDmgOverTimeTick = m_dElapsedTime + 1.f;
		this->EditHealth(-10.f); // lol rekt
	}

	MouseController::GetInstance()->GetMousePosition(x, y);
	w = Application::GetInstance().GetWindowWidth();
	h = Application::GetInstance().GetWindowHeight();
	x = x - (w * 0.5f);
	y = y + (h * 0.5f);

	//(y <= h) //W.I.P - to compare cursor pos.y with mid of screen size
	SetAnimationStatus((y <= h) ? true : false, m_bMoving, isHurt, dt);

	SetMovement(!(direction.x == 0 && direction.y == 0));

	if (isHurt)
		hurtElapsedTime += dt;

	if (hurtElapsedTime > 1.5)
	{
		hurtElapsedTime = 0.0;
		isHurt = false;
	}
	
	// if Mouse Buttons were activated, then act on them
	if (MouseController::GetInstance()->IsButtonDown(MouseController::LMB))
	{
		if (!Player::GetInstance()->isDodging())
			Player::GetInstance()->Shoot(dt);
	}
	
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::RMB))
	{
		if (m_dElapsedTime > (m_dRollTime + 0.35f) && !m_bDodge && m_bMoving) // cooldown on spamming roll really quickly
		{
			// SUPAH COOL
			setDodge(true);
			m_dSpeed = 30;
			m_dRollTime = m_dElapsedTime + 0.15f;
			std::cout << "ROLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL" << std::endl;

			if (this->m_bFire)
			{
				m_dFireTickUp = m_dElapsedTime + 2.f;
				--FireIntensity;

				std::cout << "Fire Intensity:" << FireIntensity << std::endl;
			}

			//AudioEngine::GetInstance()->editVolume(-10); // just a random button to test if edit volume is working (spoiler: it is)
		}
	}

	CollisionCheck_Movement();

	//update minimap only when player moves
	//if (m_bMoving)
	//{
		for (std::list<EntityBase*>::iterator it = CMinimap::GetInstance()->getMinimapList().begin()
			;it != CMinimap::GetInstance()->getMinimapList().end();++it)
		{

			Vector3 temp = CMinimap::GetInstance()->GetScale();

			if (((*it)->GetPosition() - position).LengthSquared() < (temp.x * 0.1) * (temp.x * 0.1))
			{
				//std::cout << "in range\n";
				switch (dynamic_cast<GenericEntity*>((*it))->type)
				{
				case GenericEntity::TELEPORTER:
					CMinimap::GetInstance()->setObjectPos("telepos", (*it)->GetPosition() - position);
					CMinimap::GetInstance()->setObjectScale("telescale", (*it)->GetScale());
					CMinimap::GetInstance()->addTeleporterPos((*it)->GetPosition());
					break;
				default:
					break;
				}
			}
		}
	//}

	for (size_t i = 0; i < Level::GetInstance()->getRooms().size(); ++i)
	{
		Vector3 tPos(Level::GetInstance()->getRooms()[i].getMidPoint().x - position.x, Level::GetInstance()->getRooms()[i].getMidPoint().y - position.y, 0);
		Vector3 tScale(Level::GetInstance()->getRooms()[i].width, Level::GetInstance()->getRooms()[i].height, 1);
		CMinimap::GetInstance()->setObjectPos("wallpos", tPos);
		CMinimap::GetInstance()->setObjectScale("wallscale", tScale);
	}

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

	if (this->m_bSlow)
		this->m_bSlow = false;

	// If the user presses R key, then reset the view to default values
	//if (KeyboardController::GetInstance()->IsKeyDown('P'))
	//{
	//	Reset(); // I dont think we need this but w/e maybe we might use it :thinking:
	//}

	// If a camera is attached to this playerInfo class, then update it
	if (attachedCamera)
	{
		double x, y;
		MouseController::GetInstance()->GetMousePosition(x, y);
		float halfWindowWidth = Application::GetInstance().GetWindowWidth() * 0.5f;
		float halfWindowHeight = Application::GetInstance().GetWindowHeight() * 0.5f;
		float posX = (static_cast<float>(x) - halfWindowWidth);
		float posY = (halfWindowHeight - static_cast<float>(y));

		Vector3 cameraView = attachedCamera->GetCameraTarget() - attachedCamera->GetCameraPos();
		attachedCamera->SetCameraPos(position + Vector3(Math::Clamp(posX * 0.005f, -1.0f, 1.0f), Math::Clamp(posY * 0.005f, -1.0f, 1.0f), 15)); // Vector3(0,0,15)
		attachedCamera->SetCameraTarget(position + Vector3(Math::Clamp(posX * 0.005f, -1.0f, 1.0f), Math::Clamp(posY * 0.005f, -1.0f, 1.0f), 0));
		attachedCamera->Update(dt);
	}

	this->SetPosition(position);
	this->SetAABB(Vector3(this->GetScale().x * 0.3f, this->GetScale().y * 0.4f, this->GetScale().z * 0.5f) + GetPos(),
		Vector3(this->GetScale().x * -0.3f, this->GetScale().y * -0.4f, this->GetScale().z * -0.5f) + GetPos());

	//set weapon pos & dir
	playerInventory->getPrimaryWeapon()->setGunPos(position);
	playerInventory->getPrimaryWeapon()->setGunDir(view);
	//playerInventory->getWeaponList()[weaponIndex]->setGunPos(position);
	//playerInventory->getWeaponList()[weaponIndex]->setGunDir(view);
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
	double x, y;
	MouseController::GetInstance()->GetMousePosition(x, y);
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() * 0.5f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() * 0.5f;
	float posX = (static_cast<float>(x) - halfWindowWidth);
	float posY = (halfWindowHeight - static_cast<float>(y));

	Vector3 changedpos = position;
	changedpos.x += Math::Clamp(posX * 0.005f, -1.0f, 1.0f);
	changedpos.y += Math::Clamp(posY * 0.005f, -1.0f, 1.0f);

	//playerInventory->getWeaponList()[weaponIndex]->Discharge(changedpos, view);
	playerInventory->getPrimaryWeapon()->Discharge(position, view.Normalize()); //position of player, dir to shoot from
	AudioEngine::GetInstance()->PlayASound("testjump", false);
	return false;
}

// Reload Weapon
bool Player::Reload(const float dt)
{
	//playerInventory->getWeaponList()[weaponIndex]->Reload(); //reload gun
	playerInventory->getPrimaryWeapon()->Reload(); //position of player, dir to shoot from
	std::cout << "TotalAmmo: " << playerInventory->getPrimaryWeapon()->GetTotalRound() << std::endl;
	return false;
}

// Change Weapon
bool Player::ChangeWeapon(const float dt)
{	
	playerInventory->getPrimaryWeapon()->setIsActive(false);
	++weaponIndex;
	weaponIndex = Math::Wrap(weaponIndex, 0, (int)playerInventory->getWeaponList().size() - 1);
	playerInventory->setWeaponIndex(weaponIndex);
	playerInventory->getPrimaryWeapon()->setIsActive(true);
	std::cout << "weaponIndex: " << weaponIndex << std::endl;
	weaponMesh = playerInventory->getPrimaryWeapon()->GetMesh();

	//playerInventory->getWeaponList()[weaponIndex]->setIsActive(true);
	//weaponMesh = playerInventory->getWeaponList()[weaponIndex]->GetMesh();
	return false;
}

void Player::UseBlank()
{
	std::list<EntityBase*> cpy = EntityManager::GetInstance()->getCollisionList();
	std::list<EntityBase*>::iterator it, end;
	end = cpy.end();

	for (it = cpy.begin(); it != end; ++it)
	{
		GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it);

		if (thatEntity->type == ENEMY_BULLET)
		{
			thatEntity->SetIsDone(true);
		}
	}

	--this->m_iBlank;
}