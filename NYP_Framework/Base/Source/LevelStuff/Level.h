#ifndef LEVEL_H
#define LEVEL_H

#include "TileSet.h"

#include <map>
#include <vector>
#include <string>

using std::map;
using std::vector;
using std::string;
using std::pair;

class Level
{
public:
	struct Point {
		int x;
		int y;
		Point(int _x, int _y) { x = _x; y = _y; };
		Point() {};
	};

	struct Rectangle {

		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
		int x2 = 0;
		int y2 = 0;

		Rectangle(int _x, int _y, int _width, int _height) 
		{ 
			x = _x; 
			y = _y;
			width = _width;
			height = _height;
			update2ndPoints();
		};

		Rectangle() {};

		Point getMidPoint() 
		{ 
			return Point(x + std::round(width * 0.5f), y + std::round(height * 0.5f)); 
		};

		void update2ndPoints() {
			x2 = x + width;
			y2 = y + height;
		};
	};
	
	enum ROOM_TYPE {
		NONE = 0,
		BASIC,
		TREASURE,
		BOSS,
		SHOP,
		PUZZLE,
		CHALLENGE,
		SHRINE
	};

	/*Basic Room - Standard room for enemy and players to fight
	Treasure Room - Enable the player to obtain items
	Boss Room - Defeat the boss in - order to proceed to the next level.
	Shop Room - Allows player to buy and sell items
	Puzzle / Physic Room - Requires player to clear the puzzle to pass.
	Challenge Room - Requires the player to fulfill the require to get the reward
	Shrine Room(Safe Zone) - For player to obtain buffs or heals.*/

	enum DIRECTION {
		NORTH = 0,
		SOUTH,
		EAST,
		WEST
	};
	

	Level();
	~Level();


	void setMaxRooms(int num);
	int getMaxRooms();
	void setMapWidth(int size);
	int getMapWidth();
	void setMapHeight(int size);
	int getMapHeight();
	void setMaxRoomHeight(int size);
	int getMaxRoomHeight();
	void setMaxRoomWidth(int size);
	int getMaxRoomWidth();


	void testCout();
	

	void generate();
	void createMap(int spawnAttempts);
	void generateCorridors();
	void addCorridor(int x, int y, int dir, int l);
	void generateRooms();
	void addRoom(int roomPosX, int roomPosY, int roomWidth, int roomHeight);
	bool isRoomOverlap(int roomPosX, int roomPosY, int roomWidth, int roomHeight);
	void generateWalls();

	bool isTileWall(Tile t);
	bool setTile(int x, int y, Tile::TILE_TYPE);
	Tile getTile(int x, int y);

private:
	int maxRooms, mapWidth, mapHeight, maxRoomWidth, maxRoomHeight;
	int roomCount, corrCount;
	map<pair<int, int>, Tile> levelMap;
	vector<Rectangle> rooms;

};

#endif // !TILEMAP_H
