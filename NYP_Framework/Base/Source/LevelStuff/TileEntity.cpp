#include "TileEntity.h"
#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../PlayerInfo\PlayerInfo.h"

TileEntity::TileEntity()
{
	modelMesh = nullptr;
}

TileEntity::TileEntity(Mesh * _modelMesh)
	: modelMesh(_modelMesh)
{
}

TileEntity::~TileEntity()
{
}

void TileEntity::Update(double _dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D
}

void TileEntity::Render()
{
	/*if (this->position.x < Player::GetInstance()->GetPos().x + 6  && this->position.y < Player::GetInstance()->GetPos().y + 3 &&
		this->position.x > Player::GetInstance()->GetPos().x - 6 && this->position.y > Player::GetInstance()->GetPos().y - 3)
	{*/
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(position.x, position.y, position.z);
		modelStack.Scale(scale.x, scale.y, scale.z);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();
	/*}*/
}

void TileEntity::CollisionResponse(TileEntity * ThatEntity)
{
	if ((this->type == PLAYER && ThatEntity->type == WALL) || (this->type == WALL && ThatEntity->type == PLAYER))
	{
		std::cout << "collide" << std::endl;
	}
}

// Set the maxAABB and minAABB
void TileEntity::SetAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
}

void TileEntity::SetMesh(Mesh * _modelMesh, OBJECT_TYPE _type)
{
	this->type = _type;
	this->modelMesh = _modelMesh;
}

Mesh * TileEntity::GetMesh()
{
	return modelMesh;
}

TileEntity* Create::TEntity(const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale,
	bool _collision)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	TileEntity* result = new TileEntity(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(_collision);
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}