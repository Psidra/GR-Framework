#include "EntityManager.h"
#include "EntityBase.h"
#include "CollisionManager.h"
#include "PlayerInfo\PlayerInfo.h"

#include <iostream>
using namespace std;

// Update all entities
void EntityManager::Update(double _dt)
{
	// Update all entities
	std::list<EntityBase*>::iterator it, end;
	int index = 0;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		if (!(*it)->IsActive())
			continue;
		if (index > totalFrontEntities)
			break;

		(*it)->Update(_dt);

		++index;
	}

	CollisionManager::GetInstance()->Update(collisionList, totalFrontEntities);

	// Erase objects that are done from collisionList
	it = collisionList.begin();
	while (it != collisionList.end()) {
		if ((*it)->IsDone()) {
			it = collisionList.erase(it);
		}
		else
			++it;
	}

	// Clean up entities that are done
	it = entityList.begin();
	index = 0;
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			if (index < totalFrontEntities)
				--totalFrontEntities;

			// Delete if done
			delete *it;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
			++index;
		}
	}
}

void EntityManager::Render()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();

	float it_posX, it_posY;
	float player_posX = Player::GetInstance()->GetPos().x;
	float player_posY = Player::GetInstance()->GetPos().y;

	for (it = entityList.begin(); it != end; ++it)
	{
		if (!(*it)->IsActive())
			continue;

		it_posX = (*it)->GetPosition().x;
		it_posY = (*it)->GetPosition().y;
		if (it_posX < player_posX + 10.5f && it_posY < player_posY + 7.5f &&
			it_posX > player_posX - 10.5f && it_posY > player_posY - 7.5f)
		{
			(*it)->Render();
		}
	}
}

// Render the UI entities
void EntityManager::RenderUI()
{
	// Render all entities UI
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}
}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity, bool _isFront)
{
	if (!_isFront)
	{
		entityList.push_back(_newEntity);

		if (_newEntity->HasCollider())
			collisionList.push_back(_newEntity);
	}
	else
	{
		++totalFrontEntities;

		entityList.push_front(_newEntity);

		if (_newEntity->HasCollider())
			collisionList.push_front(_newEntity);
	}
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity, bool isDelete)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);
	
	// find if the entity has a collider and remove from list
	if (_existingEntity->HasCollider()) {
		std::list<EntityBase*>::iterator findCollider = std::find(collisionList.begin(), collisionList.end(), _existingEntity);
		if (findCollider != collisionList.end())
			findCollider = collisionList.erase(findCollider);
	}

	// Delete the entity if found
	if(isDelete)
	{
		if (findIter != entityList.end())
		{
			delete *findIter;
			findIter = entityList.erase(findIter);
			return true;
		}
	}
	else //remove from list
	{
		if (findIter != entityList.end())
		{
			findIter = entityList.erase(findIter);
			return true;
		}
	}

	// Return false if not found
	return false;
}

std::list<EntityBase*>& EntityManager::getCollisionList()
{
	return collisionList;
}

void EntityManager::setCollisionList(std::list<EntityBase*>& newList)
{
	collisionList = newList;
}

void EntityManager::setEntityList(std::list<EntityBase*>& newList)
{
	entityList = newList;
}

std::list<EntityBase*>& EntityManager::getEntityList()
{
	return entityList;
}

// Constructor
EntityManager::EntityManager() : totalFrontEntities(0)
{
}

// Destructor
EntityManager::~EntityManager()
{
}
