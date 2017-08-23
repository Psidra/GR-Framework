#ifndef QUADTREE_H
#define QUADTREE_H


#include "../GenericEntity.h"

#include <list>;

using std::list;


class QuadTree {

public:
	QuadTree();
	QuadTree(float _x, float _y, float _width, float _height, int _level, int _maxLevel);
	~QuadTree();

	void addObject(EntityBase *object);
	list<EntityBase*> getObjectsAt(float _x, float _y);

private:
	float x;
	float y;
	float width;
	float height;
	int level;
	int maxLevel;
	list<EntityBase*> objects;

	QuadTree* NW;
	QuadTree* NE;
	QuadTree* SW;
	QuadTree* SE;

	bool contains(QuadTree *child, EntityBase *object);
};



#endif // !QUADTREE_H
