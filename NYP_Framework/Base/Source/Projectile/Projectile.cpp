#include "Projectile.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../Enemy.h"
#include "../CollisionManager.h"
#include "../Particle/ParticleEffect.h"
#include "ProjectileManager.h"

CProjectile::CProjectile(void)
	: modelMesh(NULL)
	, m_bStatus(false)
	, theDirection(0, 0, 0)
	, m_fLifetime(-1.0f)
	, m_fSpeed(10.0f)
{
}

CProjectile::CProjectile(Mesh* _modelMesh)
	: modelMesh(_modelMesh)
	, m_bStatus(false)
	, theDirection(0, 0, 0)
	, m_fLifetime(-1)
	, m_fSpeed(10.0f)
{
}

CProjectile::~CProjectile(void)
{
	modelMesh = NULL;
}

// Activate the projectile. true == active, false == inactive
void CProjectile::SetStatus(const bool m_bStatus)
{
	if (m_bStatus == false)
		m_fLifetime = -1;
	this->m_bStatus = m_bStatus;
}

// get status of the projectile. true == active, false == inactive
bool CProjectile::GetStatus(void) const
{
	return m_bStatus;
}

// Set the position and direction of the projectile
void CProjectile::Set(Vector3 theNewPosition, Vector3 theNewDirection, const float m_fLifetime, const float m_fSpeed)
{
	position = theNewPosition;
	theDirection = theNewDirection;
	this->m_fLifetime = m_fLifetime;
	this->m_fSpeed = m_fSpeed;
}

// Get the direction of the projectile
Vector3 CProjectile::GetDirection(void)
{
	return theDirection;
}

// Get the lifetime of the projectile
float CProjectile::GetLifetime(void) const
{
	return m_fLifetime;
}

// Get the speed of the projectile
float CProjectile::GetSpeed(void) const
{
	return m_fSpeed;
}

// Set the direction of the projectile
void CProjectile::SetDirection(Vector3 theNewDirection)
{
	theDirection = theNewDirection;
}

// Set the lifetime of the projectile
void CProjectile::SetLifetime(const float m_fLifetime)
{
	this->m_fLifetime = m_fLifetime;
}

// Set the speed of the projectile
void CProjectile::SetSpeed(const float m_fSpeed)
{
	this->m_fSpeed = m_fSpeed;
}

// get projectile damage
float CProjectile::getProjectileDamage()
{
	return projectileDamage;
}
//set projectile damage
void CProjectile::setProjectileDamage(const float pDamage)
{
	projectileDamage = pDamage;
}
//get isDots
bool CProjectile::getIsDots()
{
	return isDots;
}
//set isDots
void CProjectile::setIsDots(bool _isDots)
{
	isDots = _isDots;
}
// get m_bProjectileRicochet
bool CProjectile::getIsRicochet()
{
	return m_bProjectileRicochet;
}
//set m_bProjectileRicochet
void CProjectile::setIsRicochet(bool _isRicochet)
{
	m_bProjectileRicochet = _isRicochet;
}

// get m_bProjectileLaserBeam
bool CProjectile::getIsLaserbeam()
{
	return m_bProjectileLaserBeam;
}
//set m_bProjectileLaserBeam
void CProjectile::setIsLaserbeam(bool _isLaserBeam)
{
	m_bProjectileLaserBeam = _isLaserBeam;
}

// Update the status of this projectile
void CProjectile::Update(double dt)
{
	if (m_bStatus == false)
		return;

	// Update TimeLife of projectile. Set to inactive if too long
	m_fLifetime -= (float)dt;
	if (m_fLifetime < 0.0f)
	{
		SetStatus(false);
		SetIsDone(true);	// This method is to inform the EntityManager that it should remove this instance
		return;
	}

	//=====Testing codes for laser=====
	//if(!m_bProjectileLaserBeam)
	//if (m_bProjectileLaserBeam)
	//{
	//	EntityManager* temp = EntityManager::GetInstance();
	//	for (std::list<EntityBase*>::iterator it = temp->getCollisionList().begin();
	//		it != temp->getCollisionList().end(); ++it)
	//	{
	//		if (dynamic_cast<GenericEntity*>((*it))->type != GenericEntity::ENEMY)
	//			continue;

	//		//if ((*it)->GetPosition() >= Player::GetInstance()->GetPos() &&
	//		//	(*it)->GetPosition() <= position)
	//		//	std::cout << "A: " << (*it)->GetPosition() << "\n";

	//		// (A + (0.1 * AB.normalise)) 
	//		Vector3 AB = (position - Player::GetInstance()->GetPos()).Normalize();
	//		Vector3 A = Player::GetInstance()->GetPos();
	//		while(A <= position)
	//		{
	//			if (CollisionManager::GetInstance()->CheckPointToAABBCollision(A, (*it)))
	//				std::cout << "hit\n";
	//				//CollisionResponse(dynamic_cast<GenericEntity*>(*it));
	//			A = A + (0.1f * AB);
	//			std::cout << "A: " << A << std::endl;
	//		}

	//		//std::cout << "pPos: " << Player::GetInstance()->GetPos() << "\n";
	//		//std::cout << "ePos: " << (*it)->GetPosition() << std::endl;
	//		//std::cout << "tPos: " << position << std::endl;
	//	}

	//}
	//==========================

	// Update Position
	position += theDirection * (float)(dt * m_fSpeed);

	//position.Set(	position.x + (float)(theDirection.x * dt * m_fSpeed),
	//				position.y + (float)(theDirection.y * dt * m_fSpeed),
	//				position.z + (float)(theDirection.z * dt * m_fSpeed));

	//set projectile AABB
	if(!m_bProjectileLaserBeam)
		SetAABB(position + scale * 0.5, position - scale * 0.5);
}

// Render this projectile
void CProjectile::Render(void)
{
	if (m_bStatus == false)
		return;

	if (m_fLifetime < 0.0f)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Rotate(Math::RadianToDegree(atan2(theDirection.y, theDirection.x)), 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();

	//display AABB range
	//modelStack.PushMatrix();
	//modelStack.Translate(this->GetMaxAABB().x, this->GetMaxAABB().y, this->GetMaxAABB().z);
	//modelStack.Scale(scale.x * 0.5, scale.y * 0.5, scale.z * 0.5);
	//RenderHelper::RenderMesh(modelMesh);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(this->GetMinAABB().x, this->GetMinAABB().y, this->GetMinAABB().z);
	//modelStack.Scale(scale.x * 0.5, scale.y * 0.5, scale.z * 0.5);
	//RenderHelper::RenderMesh(modelMesh);
	//modelStack.PopMatrix();
}

// Collision Response
void CProjectile::CollisionResponse(GenericEntity * ThatEntity)
{
	switch (ThatEntity->type) {
	case GenericEntity::OBJECT_TYPE::WALL:
	{
		std::cout << "bullet collided with wall\n";
		//if can ricochet
		if (m_bProjectileRicochet)
		{
			Vector3 N = ThatEntity->getNormal();
			Vector3 NP = ThatEntity->getNormal().Cross(Vector3(0, 0, 1));
			Vector3 u = theDirection;
			Vector3 relativePos = position - ThatEntity->GetPosition();

			if (relativePos.Dot(N) < 0)
				N = -N;
			if (relativePos.Dot(NP) > 0)
				NP = -NP;

			Vector3 vel = u - 2 * u.Dot(N) * N;
			theDirection = vel;;

			if (theDirection.Dot(N) < 0
				&& theDirection.Dot(NP) > 0)
			{

				vel = u - 2 * u.Dot(NP) * NP;
				theDirection = vel;
			}
		}
		else if(!m_bProjectileRicochet && !m_bProjectileLaserBeam)
			this->isDone = true;
	}
		break;
	case GenericEntity::OBJECT_TYPE::ENEMY:
	{
		if (this->type != PLAYER_BULLET)
			break;

		this->SetIsDone(true);
		EnemyBase* HitEnemy = dynamic_cast<EnemyBase*>(ThatEntity);
		CProjectile* Proj = dynamic_cast<CProjectile*>(this);

		if (!HitEnemy->getInvuln())
		{
			Create::Particle("blood", this->position, 0, EFFECT_TYPE::ET_BLEED, 0.3, 0.5, true, HitEnemy);
			HitEnemy->editHealth(-Proj->getProjectileDamage());
		}
		else
		{
			std::cout << "enemy is invulnerable!" << std::endl;
		}

		std::cout << "player bullet collide with enemy" << std::endl;
		break;
	}
	default:
		break;
	}
}

//get active
bool CProjectile::getActive()
{
	return m_bActive;
}

//set active
void CProjectile::setIsActive(bool _isActive)
{
	m_bActive = _isActive;
}


// Create a projectile and add it into EntityManager
CProjectile* Create::Projectile(const std::string& _meshName, 
								const Vector3& _position, 
								const Vector3& _direction,
								const Vector3& _scale,
								const float m_fLifetime, 
								const float m_fSpeed)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CProjectile* result = new CProjectile(modelMesh);
	result->Set(_position, _direction, m_fLifetime, m_fSpeed);
	result->SetStatus(true);
	result->SetCollider(true);
	result->SetScale(_scale);
	EntityManager::GetInstance()->AddEntity(result);
	ProjectileManager::GetInstance()->AddProjectile(result);
	return result;
}
