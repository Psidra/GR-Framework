#include "GenericEntity.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "PlayerInfo\PlayerInfo.h"
#include "Projectile\Projectile.h"

GenericEntity::GenericEntity(Mesh* _modelMesh)
	: modelMesh(_modelMesh)
{
}

GenericEntity::~GenericEntity()
{
}

void GenericEntity::Update(double _dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D

}

void GenericEntity::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

void GenericEntity::CollisionResponse(GenericEntity * ThatEntity)
{
	GenericEntity* FirstEntity = nullptr;
	GenericEntity* SecondEntity = nullptr;

	if (this->type == PLAYER_BULLET)
	{
		FirstEntity = this;
		SecondEntity = ThatEntity;
	}
	else if (ThatEntity->type == PLAYER_BULLET)
	{
		FirstEntity = ThatEntity;
		SecondEntity = this;
	}

	if (FirstEntity != nullptr)
	{
		switch (SecondEntity->type) {
		case NONE:
			return;
		case WALL:
		{
			std::cout << "bullet collided with wall\n";
			//FirstEntity->SetIsDone(true);
			//Vector3 temp = dynamic_cast<CProjectile*>(FirstEntity)->GetDirection();
			Vector3 N = SecondEntity->normal;
			Vector3 NP = SecondEntity->normal.Cross(Vector3(0, 0, 1));
			Vector3 u = dynamic_cast<CProjectile*>(FirstEntity)->GetDirection();
			Vector3 relativePos = FirstEntity->position - SecondEntity->position;

			if (relativePos.Dot(N) < 0)
				N = -N;
			if (relativePos.Dot(NP) > 0)
				NP = -NP;

			Vector3 vel = u - 2 * u.Dot(N) * N;

			dynamic_cast<CProjectile*>(FirstEntity)->SetDirection(vel);
		
			if (dynamic_cast<CProjectile*>(FirstEntity)->GetDirection().Dot(N) < 0
				&& dynamic_cast<CProjectile*>(FirstEntity)->GetDirection().Dot(NP) > 0)
			{
	
				vel = u - 2 * u.Dot(NP) * NP;
				dynamic_cast<CProjectile*>(FirstEntity)->SetDirection(vel);


			}

		
			

			//vel = u - 2 * u.Dot(NP) * NP; 
	

		}
			return;
		//case ENEMY:
		//	std::cout << "bullet collided with enemy\n";
		//	FirstEntity->SetIsDone(true);
		//	return;

		default:
			// Bullet collides with other bullets xd
			return;
		}
	}

	/*if ((this->type == PLAYER_BULLET && ThatEntity->type == WALL) || (this->type == WALL && ThatEntity->type == PLAYER_BULLET))
	{
		std::cout << "bullet collided with wall\n";
		if (this->type == PLAYER_BULLET)
			this->SetIsDone(true);
		if (ThatEntity->type == PLAYER_BULLET)
			ThatEntity->SetIsDone(true);
	}
	if ((this->type == PLAYER_BULLET && ThatEntity->type == ENEMY) || (this->type == ENEMY && ThatEntity->type == PLAYER_BULLET))
	{
		std::cout << "bullet collided with enemy\n";
	}*/
}

// Set the maxAABB and minAABB
void GenericEntity::SetAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
}

void GenericEntity::SetMesh(Mesh * _modelMesh, OBJECT_TYPE _type)
{
	this->modelMesh = _modelMesh;
	this->type = _type;
}

Mesh * GenericEntity::GetMesh()
{
	return modelMesh;
}

Vector3 GenericEntity::getNormal()
{
	return this->normal;
}

void GenericEntity::setNormal(Vector3 _normal)
{
	this->normal = _normal;
}

GenericEntity* Create::Entity(	const std::string& _meshName, 
								const Vector3& _position,
								const Vector3& _scale,
								bool _collision)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	GenericEntity* result = new GenericEntity(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(_collision);
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}
