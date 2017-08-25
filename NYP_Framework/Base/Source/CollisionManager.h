#pragma once
#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <list>

#include "SingletonTemplate.h"
#include "Vector3.h"
#include "GenericEntity.h"
#include "UIElement.h"

class CollisionManager : public Singleton<CollisionManager> {
	friend Singleton<CollisionManager>;
public:
	// Check if this entity's bounding sphere collided with that entity's bounding sphere
	bool CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	// Check if this entity's bounding sphere collided with that entity's bounding sphere 
	bool CheckPointToSphereCollision(Vector3 point, EntityBase *ThatEntity);
	// Check if this entity collided with another entity, but both must have collider
	bool CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	// Check if a point is within an AABB, one must have collider
	bool CheckPointToAABBCollision(Vector3 point, EntityBase *ThatEntity);
	// UI Related Check if Cursor AABB collide with Button AABB
	bool UI_CheckAABBCollision(Vector3 point, UIElement *ThatElement);

	// Check all objects with collision
	void Update(std::list<EntityBase*> collisionList, int totalFrontEntities);

private:
	CollisionManager();
	virtual ~CollisionManager();
};

#endif