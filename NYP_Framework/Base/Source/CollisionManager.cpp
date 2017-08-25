#include "CollisionManager.h"
#include "PlayerInfo\PlayerInfo.h"

bool CollisionManager::CheckPointToSphereCollision(Vector3 point, EntityBase * ThatEntity)
{
	if (!ThatEntity->HasCollider()) {
		std::cout << "Entity does not have Collider" << std::endl;
		return false;
	}

	GenericEntity* thatSphere = dynamic_cast<GenericEntity*>(ThatEntity);
	
	if (!thatSphere->HasSphere()) {
		std::cout << "Entity does not have Bounding Sphere" << std::endl;
		return false;
	}

	if (DistanceSquaredBetween(point, thatSphere->GetPosition() < thatSphere->GetRadius() * thatSphere->GetRadius()))
		return true;
	return false;
}

bool CollisionManager::CheckSphereCollision(EntityBase * ThisEntity, EntityBase * ThatEntity)
{
	if (!ThisEntity->HasCollider() || !ThatEntity->HasCollider()) {
		std::cout << "1 or more Entities does not have Collider" << std::endl;
		return false;
	}

	GenericEntity* thisSphere = dynamic_cast<GenericEntity*>(ThisEntity);
	GenericEntity* thatSphere = dynamic_cast<GenericEntity*>(ThatEntity);
	
	if (!thatSphere->HasSphere() || !thisSphere->HasSphere()) {
		std::cout << "1 or more Entities does not have Bounding Sphere" << std::endl;
		return false;
	}

	float totalRadius = thatSphere->GetRadius() + thisSphere->GetRadius();
	if (DistanceSquaredBetween(thatSphere->GetPosition(), thisSphere->GetPosition()) < totalRadius * totalRadius) {
		return true;
	}
	return false;
}

bool CollisionManager::CheckAABBCollision(EntityBase * ThisEntity, EntityBase * ThatEntity)
{
	if (!ThatEntity->HasCollider() || !ThisEntity->HasCollider()) {
		//std::cout << "1 or more Entities does not have Collider" << std::endl;
		return false;
	}

	GenericEntity* thisHitbox = dynamic_cast<GenericEntity*>(ThisEntity);
	GenericEntity* thatHitbox = dynamic_cast<GenericEntity*>(ThatEntity);

	if (!thisHitbox->HasAABB() || !thatHitbox->HasAABB()){
		//std::cout << "1 or more Entities does not have AABB" << std::endl;
		return false;
	}

	return (thisHitbox->GetMinAABB().x <= thatHitbox->GetMaxAABB().x && thisHitbox->GetMaxAABB().x >= thatHitbox->GetMinAABB().x) &&
		(thisHitbox->GetMinAABB().y <= thatHitbox->GetMaxAABB().y && thisHitbox->GetMaxAABB().y >= thatHitbox->GetMinAABB().y);
}

bool CollisionManager::CheckPointToAABBCollision(Vector3 point, EntityBase * ThatEntity)
{
	if (!ThatEntity->HasCollider()) {
		std::cout << "Entity does not have Collider" << std::endl;
		return false;
	}

	GenericEntity* thatHitbox = dynamic_cast<GenericEntity*>(ThatEntity);

	if (!thatHitbox->HasAABB()) {
		std::cout << "Entity does not have AABB" << std::endl;
		return false;
	}

	return (point.x <= thatHitbox->GetMaxAABB().x && point.x >= thatHitbox->GetMinAABB().x) &&
		(point.y <= thatHitbox->GetMaxAABB().y && point.y >= thatHitbox->GetMinAABB().y);
}

bool CollisionManager::UI_CheckAABBCollision(Vector3 point, UIElement * ThatElement)
{
	if (!ThatElement->HasCollider()) {
		std::cout << "Entity does not have Collider" << std::endl;
		return false;
	}

	UIElement* thatHitbox = dynamic_cast<UIElement*>(ThatElement);

	if (!thatHitbox->HasAABB()) {
		std::cout << "Entity does not have AABB" << std::endl;
		return false;
	}

	return (point.x <= thatHitbox->GetMaxAABB().x && point.x >= thatHitbox->GetMinAABB().x) &&
		(point.y <= thatHitbox->GetMaxAABB().y && point.y >= thatHitbox->GetMinAABB().y); // Would check Z but 2d game so w/e (removed for all)
}

void CollisionManager::Update(std::list<EntityBase*> collisionList)
{
	std::list<EntityBase*>::iterator it, it2, end;
	end = collisionList.end();
	
	for (it = collisionList.begin(); it != end; ++it) 
	{
		if (!(*it)->IsActive())
			continue;

		for (it2 = std::next(it, 1); it2 != end; ++it2)
		{
			if (!(*it2)->IsActive())
				continue;

			// do your checks here
			if (CheckAABBCollision(*it, *it2))
			{
				GenericEntity* thisEntity = dynamic_cast<GenericEntity*>(*it);
				GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it2);

				// I HAVE NO IDEA IF I NEED THIS OR NOT
				GenericEntity* tempEntity = nullptr;
				if (thatEntity->type == GenericEntity::OBJECT_TYPE::ENEMY
					|| thatEntity->type == GenericEntity::OBJECT_TYPE::PLAYER_BULLET
					|| thatEntity->type == GenericEntity::OBJECT_TYPE::ENEMY_BULLET)
				{
					tempEntity = thisEntity;
					thisEntity = thatEntity;
					thatEntity = tempEntity;
				}

				//create collison response code to settle what to do
				thisEntity->CollisionResponse(thatEntity);

			}
		}

		if (CheckAABBCollision(Player::GetInstance(), *it))
		{
			GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it);
			Player::GetInstance()->CollisionResponse(thatEntity);
		}
	}
}

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}
