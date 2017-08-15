#include "GenericEntity.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "PlayerInfo\PlayerInfo.h"

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
	if ((this->type == PLAYER && ThatEntity->type == WALL) || (this->type == WALL && ThatEntity->type == PLAYER))
	{
		std::cout << "collide" << std::endl;
	}
	if ((this->type == PLAYER_BULLET && ThatEntity->type == WALL) || (this->type == WALL && ThatEntity->type == PLAYER_BULLET))
	{
		std::cout << "bullet collide\n";
		if (this->type == PLAYER_BULLET)
			this->SetIsDone(true);
		if (ThatEntity->type == PLAYER_BULLET)
			ThatEntity->SetIsDone(true);
	}
}

// Set the maxAABB and minAABB
void GenericEntity::SetAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
}

void GenericEntity::SetMesh(Mesh * _modelMesh, OBJECT_TYPE _type)
{
	this->type = _type;
	this->modelMesh = _modelMesh;
}

Mesh * GenericEntity::GetMesh()
{
	return modelMesh;
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
