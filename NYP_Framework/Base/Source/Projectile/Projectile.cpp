#include "Projectile.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

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

	// Update Position
	position += theDirection * (float)(dt * m_fSpeed);
	//position.Set(	position.x + (float)(theDirection.x * dt * m_fSpeed),
	//				position.y + (float)(theDirection.y * dt * m_fSpeed),
	//				position.z + (float)(theDirection.z * dt * m_fSpeed));

	//set projectile AABB
	SetAABB(position + scale, position - scale);
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
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

// Collision Response
void CProjectile::CollisionResponse(GenericEntity * ThatEntity)
{
	//switch (ThatEntity->type) {
	//case GenericEntity::OBJECT_TYPE::WALL:
	//{
	//	std::cout << "bullet collided with wall\n";
	//	//FirstEntity->SetIsDone(true);
	//	//Vector3 temp = dynamic_cast<CProjectile*>(FirstEntity)->GetDirection();
	//	Vector3 N = ThatEntity->getNormal();
	//	Vector3 NP = ThatEntity->getNormal().Cross(Vector3(0, 0, 1));
	//	Vector3 u = theDirection;
	//	Vector3 relativePos = position - ThatEntity->GetPosition();

	//	if (relativePos.Dot(N) < 0)
	//		N = -N;
	//	if (relativePos.Dot(NP) > 0)
	//		NP = -NP;

	//	Vector3 vel = u - 2 * u.Dot(N) * N;
	//	theDirection = vel;;

	//	if (theDirection.Dot(N) < 0
	//		&& theDirection.Dot(NP) > 0)
	//	{

	//		vel = u - 2 * u.Dot(NP) * NP;
	//		theDirection = vel;
	//	}
	//}
	//	break;
	//default:
	//	break;
	//}
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

	return result;
}
