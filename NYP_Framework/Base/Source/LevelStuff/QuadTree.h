#ifndef QUADTREE_H
#define QUADTREE_H


#include "../GenericEntity.h"

#include <list>;
#include <vector>;

using std::list;
using std::vector;

class QuadTree {

enum Node {
		NW = 0,
		NE,
		SW,
		SE,
		NodeCount
};

public:
	QuadTree();
	QuadTree(float _left, float _right, float _top, float _down, unsigned _maxObjectsToGrow = 4);
	~QuadTree();

	void addObject(EntityBase *object);
	void clear();
	vector<EntityBase*> getObjectsAt(float _x, float _y);
	vector<EntityBase*> queryRange(float _left, float _right, float _top, float _down);

private:
	vector<EntityBase*> objects;

	float left, right, top, down;
	unsigned maxObjectToGrow;
	QuadTree* nodes;

	bool isLeaf;

	bool contains(EntityBase *object);
	bool contains(float x, float y);
	bool contains(float _left, float _right, float _top, float _down);

	void createLeaves();
	void moveObjectsToLeaves();
};



#endif // !QUADTREE_H
