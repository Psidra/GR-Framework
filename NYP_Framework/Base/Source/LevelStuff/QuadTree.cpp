#include "QuadTree.h"

QuadTree::QuadTree()
{
}

QuadTree::QuadTree(float _x, float _y, float _width, float _height, int _level, int _maxLevel) :
	x(_x),
	y(_y),
	width(_width),
	height(_height),
	level (_level),
	maxLevel(_maxLevel)
{
	if (level == maxLevel)
		return;

	const float halfWidth = width * 0.5f;
	const float halfHeight = height * 0.5f;


	NW = new QuadTree(x, y, halfWidth, halfHeight, level + 1, maxLevel);
	NE = new QuadTree(x + halfWidth, y, halfWidth, halfHeight, level + 1, maxLevel);
	SW = new QuadTree(x, y + halfHeight, halfWidth, halfHeight, level + 1, maxLevel);
	SE = new QuadTree(x + halfWidth, y + halfHeight, halfWidth, halfHeight, level + 1, maxLevel);
}

QuadTree::~QuadTree()
{
	if (level == maxLevel)
		return;

	delete NW;
	delete NE;
	delete SW;
	delete SE;
}

void QuadTree::addObject(GenericEntity * object)
{
	if (level == maxLevel)
		objects.push_back(object);
	else if (contains(NW, object))
		NW->addObject(object);
	else if (contains(NE, object))
		NE->addObject(object);
	else if (contains(SW, object))
		SW->addObject(object);
	else if (contains(SE, object))
		SE->addObject(object);
	else if (contains(this, object))
		objects.push_back(object);
}

list<GenericEntity*> QuadTree::getObjectsAt(float _x, float _y)
{
	list<GenericEntity*> returnObjects, childReturnObjects;

	if (level == maxLevel)
		return objects;

	if (!objects.empty())
		returnObjects = objects;

	const float halfWidth = width * 0.5f;
	const float halfHeight = height * 0.5f;

	if (_x > x + halfWidth && _x < x + width)
	{
		if (_y > y + halfHeight && _y < y + height)
			childReturnObjects = SE->getObjectsAt(_x, _y);
		else if (_y > y && _y <= y + halfHeight)
			childReturnObjects = NE->getObjectsAt(_x, _y);
	}
	else if (_x > x && _x <= x + halfWidth)
	{
		if (_y > y + halfHeight && _y < y + height)
			childReturnObjects = SW->getObjectsAt(_x, _y);
		else if (_y > y && _y <= y + halfHeight)
			childReturnObjects = NW->getObjectsAt(_x, _y);
	}

	if (childReturnObjects.size() > 0)
		returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());

	return returnObjects;
}

bool QuadTree::contains(QuadTree * child, GenericEntity * object)
{
	return !(object->GetPosition().x < child->x ||
		object->GetPosition().y < child->y ||
		object->GetPosition().x > child->x + child->width ||
		object->GetPosition().y > child->y + child->height ||
		object->GetPosition().x + object->GetMinAABB().x  < child->x ||
		object->GetPosition().y + object->GetMinAABB().y < child->y ||
		object->GetPosition().x + object->GetMinAABB().x  > child->x + child->width ||
		object->GetPosition().y + object->GetMinAABB().y > child->y + child->height);
}
