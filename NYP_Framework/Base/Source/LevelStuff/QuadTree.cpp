#include "QuadTree.h"

QuadTree::QuadTree() :
	minX(0),
	maxX(0),
	minY(0),
	maxY(0),
	maxObjectToGrow(5),
	nodes(nullptr),
	isLeaf(true)
{
}

QuadTree::QuadTree(float _minX, float _maxX, float _maxY, float _minY, unsigned _maxObjectsToGrow) :
	minX(_minX),
	maxX(_maxX),
	minY(_minY),
	maxY(_maxY),
	maxObjectToGrow(_maxObjectsToGrow),
	nodes(nullptr),
	isLeaf(true)
{
}

//QuadTree::QuadTree(float _x, float _y, float _width, float _height, int _level, int _maxLevel) :
//	x(_x),
//	y(_y),
//	width(_width),
//	height(_height),
//	level (_level),
//	maxLevel(_maxLevel)
//{
//	if (level == maxLevel)
//		return;
//
//	const float halfWidth = width * 0.5f;
//	const float halfHeight = height * 0.5f;
//
//
//	/*NW = new QuadTree(x, y, halfWidth, halfHeight, level + 1, maxLevel);
//	NE = new QuadTree(x + halfWidth, y, halfWidth, halfHeight, level + 1, maxLevel);
//	SW = new QuadTree(x, y + halfHeight, halfWidth, halfHeight, level + 1, maxLevel);
//	SE = new QuadTree(x + halfWidth, y + halfHeight, halfWidth, halfHeight, level + 1, maxLevel);*/
//}

QuadTree::~QuadTree()
{
	//if (level == maxLevel)
	//	return;

	//if (!objects.empty())
	//{
	//	for (list<EntityBase*>::const_iterator it = objects.begin(); it != objects.end(); ++it)
	//		delete *it;

	//	objects.clear();
	//}

	if (!isLeaf)
		delete[] nodes;

	//delete NW;
	//delete NE;
	//delete SW;
	//delete SE;

	//NW = NULL;
	//NE = NULL;
	//SW = NULL;
	//SE = NULL;
}

void QuadTree::addObject(EntityBase * object)
{
	//if (level == maxLevel)
	//	objects.push_back(object);
	//else if (contains(NW, object))
	//	NW->addObject(object);
	//else if (contains(NE, object))
	//	NE->addObject(object);
	//else if (contains(SW, object))
	//	SW->addObject(object);
	//else if (contains(SE, object))
	//	SE->addObject(object);
	//else if (contains(this, object))
	//	objects.push_back(object);

	if (isLeaf)
	{
		objects.push_back(object);
		bool reachedMaxObjects = (objects.size() == maxObjectToGrow);
		if (reachedMaxObjects)
		{
			createLeaves();
			moveObjectsToLeaves();
			isLeaf = false;
		}
		return;
	}

	for (int i = 0; i < NodeCount; ++i)
	{
		if (nodes[i].contains(object)) {
			nodes[i].addObject(object);
			return;
		}
			
	}

	objects.push_back(object);
}

void QuadTree::clear()
{
	//if (level < maxLevel) {
	//	NW->clear();
	//	NE->clear();
	//	SW->clear();
	//	SE->clear();
	//}

	//if (!objects.empty())
	//{
	//	for (list<EntityBase*>::const_iterator it = objects.begin(); it != objects.end(); ++it)
	//		delete *it;

	//	objects.clear();
	//}

	objects.clear();

	if (!isLeaf)
	{
		for (int i = 0; i < NodeCount; ++i)
		{
			nodes[i].clear();
		}
	}

	//nodes.clear();
}

vector<EntityBase*> QuadTree::getObjectsAt(float _x, float _y)
{
	if (isLeaf)
		return objects;

	vector<EntityBase*> returnObjects, childObjects;

	if (!objects.empty())
		returnObjects.insert(returnObjects.end(), objects.begin(), objects.end());

	for (int i = 0; i < NodeCount; ++i)
	{
		if (nodes[i].contains(_x, _y)) {
			childObjects = nodes[i].getObjectsAt(_x, _y);
			returnObjects.insert(returnObjects.end(), childObjects.begin(), childObjects.end());
			break;
		}
	}

	return returnObjects;




	//if (level == maxLevel)
	//	return objects;

	//if (!objects.empty())
	//	returnObjects = objects;

	//const float halfWidth = width * 0.5f;
	//const float halfHeight = height * 0.5f;

	//if (_x > x + halfWidth && _x < x + width)
	//{
	//	if (_y > y + halfHeight && _y < y + height)
	//		childReturnObjects = SE->getObjectsAt(_x, _y);
	//	else if (_y > y && _y <= y + halfHeight)
	//		childReturnObjects = NE->getObjectsAt(_x, _y);
	//}
	//else if (_x > x && _x <= x + halfWidth)
	//{
	//	if (_y > y + halfHeight && _y < y + height)
	//		childReturnObjects = SW->getObjectsAt(_x, _y);
	//	else if (_y > y && _y <= y + halfHeight)
	//		childReturnObjects = NW->getObjectsAt(_x, _y);
	//}

	//if (childReturnObjects.size() > 0)
	//	returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());

	//return returnObjects;
}

//bool QuadTree::contains(QuadTree * child, EntityBase * object)
//{
//	return !(object->GetPosition().x < child->x ||
//		object->GetPosition().y < child->y ||
//		object->GetPosition().x > child->x + child->width ||
//		object->GetPosition().y > child->y + child->height ||
//		dynamic_cast<GenericEntity*>(object)->GetMaxAABB().x < child->x ||
//		dynamic_cast<GenericEntity*>(object)->GetMaxAABB().y < child->y ||
//		dynamic_cast<GenericEntity*>(object)->GetMaxAABB().x > child->x + child->width ||
//		dynamic_cast<GenericEntity*>(object)->GetMaxAABB().y > child->y + child->height);
//		/*object->GetPosition().x + object->GetScale().x * 0.5f < child->x ||
//		object->GetPosition().y + object->GetScale().y * 0.5f < child->y ||
//		object->GetPosition().x + object->GetScale().x * 0.5f > child->x + child->width ||
//		object->GetPosition().y + object->GetScale().y * 0.5f > child->y + child->height);*/
//}

bool QuadTree::contains(EntityBase * object)
{
	GenericEntity* temp = dynamic_cast<GenericEntity*>(object);
	
	//if (temp->GetMinAABB().x > minX &&
	//	temp->GetMinAABB().x < maxX &&
	//	temp->GetMaxAABB().y > maxY &&
	//	temp->GetMinAABB().y < minY)
	//	return true;

	//else if (temp->GetMaxAABB().x > minX &&
	//	temp->GetMaxAABB().x < maxX &&
	//	temp->GetMaxAABB().y > maxY &&
	//	temp->GetMinAABB().y < minY)
	//	return true;

	//else if (temp->GetMinAABB().x > minX &&
	//	temp->GetMinAABB().x < maxX &&
	//	temp->GetMaxAABB().y > maxY &&
	//	temp->GetMaxAABB().y < minY)
	//	return true;

	//else if (temp->GetMaxAABB().x > minX &&
	//	temp->GetMaxAABB().x < maxX &&
	//	temp->GetMaxAABB().y > maxY &&
	//	temp->GetMaxAABB().y < minY)
	//	return true;

	//else
	//	return false;

	return (temp->GetMinAABB().x > minX &&
		temp->GetMaxAABB().x < maxX &&
		temp->GetMinAABB().y > minY &&
		temp->GetMaxAABB().y < maxY);
}
bool QuadTree::contains(float x, float y)
{
	return 	(x >= minX && x <= maxX) &&
		(y >= maxY && y <= minY);
}

void QuadTree::createLeaves()
{
	nodes = new QuadTree[4];
	nodes[NW] = QuadTree(minX, (minX + maxX) * 0.5f, minY, (minY + maxY) * 0.5f, maxObjectToGrow);
	nodes[NE] = QuadTree((minX + maxX) * 0.5f, maxX, minY, (minY + maxY) * 0.5f, maxObjectToGrow);
	nodes[SW] = QuadTree(minX, (minX + maxX) * 0.5f, (minY + maxY) * 0.5f, maxY, maxObjectToGrow);
	nodes[SE] = QuadTree((minX + maxX) * 0.5f, maxX, (minY + maxY) * 0.5f, maxY, maxObjectToGrow);
}

void QuadTree::moveObjectsToLeaves()
{
	for (int n = 0; n < NodeCount; ++n) 
	{
		for (unsigned int m = 0; m < objects.size(); ++m) 
		{
			if (nodes[n].contains(objects[m])) 
			{
				nodes[n].addObject(objects[m]);
				objects.erase(objects.begin() + m);
				--m;
			}
		}
	}
}
