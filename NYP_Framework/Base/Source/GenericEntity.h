#ifndef GENERIC_ENTITY_H
#define GENERIC_ENTITY_H

#include "EntityBase.h"
#include <string>
#include "Collider/Collider.h"

class Mesh;

class GenericEntity : public EntityBase, public Collision
{
public:
	GenericEntity() { modelMesh = nullptr; };
	GenericEntity(Mesh* _modelMesh);
	virtual ~GenericEntity();

	enum OBJECT_TYPE {
		NONE = 0,
		PLAYER,
		WALL,
		ENEMY,
		ENEMY_BULLET,
		PLAYER_BULLET,
	}type;

	virtual void Update(double _dt);
	virtual void Render();

	virtual void CollisionResponse(GenericEntity* ThatEntity);

	// Set the maxAABB and minAABB
	void SetAABB(Vector3 maxAABB, Vector3 minAABB);

	// Set mesh
	void SetMesh(Mesh* _modelMesh, OBJECT_TYPE _type = NONE);
	// Get mesh
	Mesh* GetMesh();
	// Get normal
	Vector3 getNormal();
	//set normal
	void setNormal(Vector3 _normal);
private:
	Mesh* modelMesh;
	Vector3 normal;
};

namespace Create
{
	GenericEntity* Entity(	const std::string& _meshName, 
							const Vector3& _position, 
							const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
							bool _collision = false);
};

#endif // GENERIC_ENTITY_H