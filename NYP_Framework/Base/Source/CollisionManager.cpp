#include "CollisionManager.h"
#include "PlayerInfo\PlayerInfo.h"
#include "LevelStuff\QuadTree.h"
#include "LevelStuff\Level.h"

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

void CollisionManager::Update(std::list<EntityBase*> collisionList, int totalFrontEntities)
{
	std::list<EntityBase*>::iterator it, end;
	int index = 0;
	end = collisionList.end();

	QuadTree quadTree(0, Level::GetInstance()->getMapWidth(), Level::GetInstance()->getMapHeight(), 0);
	//QuadTree quadTree(0, 800, 600, 0, 3);
	vector<EntityBase*> getNearestObj;
	
	quadTree.clear();
	for (it = collisionList.begin(); it != end; ++it)
		quadTree.addObject(*it);
	
	for (it = collisionList.begin(); it != end; ++it) 
	{
		if (!(*it)->IsActive())
			continue;
		if (index > totalFrontEntities)
			break;

		//list<EntityBase*> retunedObj;
		getNearestObj = quadTree.queryRange(dynamic_cast<GenericEntity*>(*it)->GetMinAABB().x,
			dynamic_cast<GenericEntity*>(*it)->GetMaxAABB().x, 
			dynamic_cast<GenericEntity*>(*it)->GetMaxAABB().y, 
			dynamic_cast<GenericEntity*>(*it)->GetMinAABB().y);
		//getNearestObj = quadTree.getObjectsAt((*it)->GetPosition().x, (*it)->GetPosition().y);
		//std::copy(getNearestObj.begin(), getNearestObj.end(), std::back_inserter(retunedObj));
		//for (it2 = std::next(it, 1); it2 != end; ++it2)
		
		for (std::vector<EntityBase*>::iterator it2 = getNearestObj.begin(); it2 != getNearestObj.end(); ++it2)
		//for (it2 = std::next(it, 1); it2 != end; ++it2)
		{

			if (!(*it2)->IsActive())
				continue;

			// do your checks here
			if (CheckAABBCollision(*it, *it2))
			{
				GenericEntity* thisEntity = dynamic_cast<GenericEntity*>(*it);
				GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it2);

				//create collison response code to settle what to do
				thisEntity->CollisionResponse(thatEntity);

			}
		}

		++index;
	}

	getNearestObj.clear();
	getNearestObj = quadTree.queryRange(Player::GetInstance()->GetMinAABB().x, Player::GetInstance()->GetMaxAABB().x, Player::GetInstance()->GetMaxAABB().y, Player::GetInstance()->GetMinAABB().y);

	for (std::vector<EntityBase*>::iterator it3 = getNearestObj.begin(); it3 != getNearestObj.end(); ++it3)
	{
		if (CheckAABBCollision(Player::GetInstance(), *it3))
		{
			GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it3);
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
