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
	QuadTree(float _minX, float _maxX, float _maxY, float _minY, unsigned _maxObjectsToGrow = 4);
	//QuadTree(float _x, float _y, float _width, float _height, int _level, int _maxLevel);
	~QuadTree();

	void addObject(EntityBase *object);
	void clear();
	vector<EntityBase*> getObjectsAt(float _x, float _y);

private:
	/*float x;
	float y;
	float width;
	float height;
	int level;
	int maxLevel;*/

	vector<EntityBase*> objects;

	/*QuadTree* NW;
	QuadTree* NE;
	QuadTree* SW;
	QuadTree* SE;*/

	//bool contains(QuadTree *child, EntityBase *object);

	//new
	float minX, maxX, minY, maxY;
	unsigned maxObjectToGrow;
	QuadTree* nodes;

	bool isLeaf;

	bool contains(EntityBase *object);
	bool contains(float x, float y);

	void createLeaves();
	void moveObjectsToLeaves();

};



#endif // !QUADTREE_H
