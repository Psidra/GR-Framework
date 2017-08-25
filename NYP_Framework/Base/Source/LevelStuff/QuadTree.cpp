#include "QuadTree.h"

QuadTree::QuadTree() :
	left(0),
	right(0),
	top(0),
	down(0),
	maxObjectToGrow(5),
	nodes(nullptr),
	isLeaf(true)
{
}

QuadTree::QuadTree(float _left, float _right, float _top, float _down, unsigned _maxObjectsToGrow) :
	left(_left),
	right(_right),
	top(_top),
	down(_down),
	maxObjectToGrow(_maxObjectsToGrow),
	nodes(nullptr),
	isLeaf(true)
{
}

QuadTree::~QuadTree()
{
	if (!isLeaf)
		delete[] nodes;
}

void QuadTree::addObject(EntityBase * object)
{
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
	objects.clear();

	if (!isLeaf)
	{
		for (int i = 0; i < NodeCount; ++i)
		{
			nodes[i].clear();
		}
	}
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
}

vector<EntityBase*> QuadTree::queryRange(float _left, float _right, float _top, float _down)
{
	if (isLeaf)
		return objects;

	vector<EntityBase*> returnObjects, childObjects;

	if (!objects.empty())
		returnObjects.insert(returnObjects.end(), objects.begin(), objects.end());

	for (int i = 0; i < NodeCount; ++i)
	{
		if (nodes[i].contains(_left, _right, _top, _down)) {
			childObjects = nodes[i].queryRange(_left, _right, _top, _down);
			returnObjects.insert(returnObjects.end(), childObjects.begin(), childObjects.end());
			break;
		}
	}

	return returnObjects;
}

bool QuadTree::contains(EntityBase * object)
{
	GenericEntity* temp = dynamic_cast<GenericEntity*>(object);

	float objLeft = temp->GetMinAABB().x;
	float objRight = temp->GetMaxAABB().x;
	float objTop = temp->GetMaxAABB().y;
	float objDown = temp->GetMinAABB().y;

	return (objLeft > left &&
		objRight < right &&
		objTop < top &&
		objDown > down);
}

bool QuadTree::contains(float x, float y)
{
	return 	(x >= left && x <= right) &&
		(y >= down && y <= top);
}

bool QuadTree::contains(float _left, float _right, float _top, float _down)
{
	return 	(_left >= left && _right <= right) &&
		(_down >= down && _top <= top);
} 

void QuadTree::createLeaves()
{
	nodes = new QuadTree[4];
	nodes[NW] = QuadTree(left, (left + right) * 0.5f, top, (top + down) * 0.5f, maxObjectToGrow);
	nodes[NE] = QuadTree((left + right) * 0.5f, right, top, (top + down) * 0.5f, maxObjectToGrow);
	nodes[SW] = QuadTree(left, (left + right) * 0.5f, (top + down) * 0.5f, down, maxObjectToGrow);
	nodes[SE] = QuadTree((left + right) * 0.5f, right, (top + down) * 0.5f, down, maxObjectToGrow);
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
