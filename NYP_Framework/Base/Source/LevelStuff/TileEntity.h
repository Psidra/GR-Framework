#ifndef TILE_ENTITY_H
#define TILE_ENTITY_H

#include "..\GenericEntity.h"
#include <string>
#include "Collider/Collider.h"

class Mesh;

class TileEntity : public GenericEntity
{
public:
	TileEntity();
	TileEntity(Mesh* _modelMesh);
	virtual ~TileEntity();
	virtual void Update(double _dt);
	virtual void Render();

	virtual void CollisionResponse(TileEntity* ThatEntity);

	// Set the maxAABB and minAABB
	void SetAABB(Vector3 maxAABB, Vector3 minAABB);

	// Set mesh
	void SetMesh(Mesh* _modelMesh, OBJECT_TYPE _type);
	// Get mesh
	Mesh* GetMesh();

private:
	Mesh* modelMesh;
};

namespace Create
{
	TileEntity* TEntity(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		bool _collision = false);
};
#endif