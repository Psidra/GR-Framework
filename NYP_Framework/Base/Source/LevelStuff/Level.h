#ifndef LEVEL_H
#define LEVEL_H

#include "TileSet.h"
#include "SingletonTemplate.h"

#include <map>
#include <vector>
#include <string>

using std::map;
using std::vector;
using std::string;
using std::pair;

class Level : public Singleton<Level>
{
public:
	struct Point {
		float x;
		float y;
		Point(float _x, float _y) { x = _x; y = _y; };
		Point() {};
	};

	struct Rectangle {

		float x = 0;
		float y = 0;
		float width = 0;
		float height = 0;
		float x2 = 0;
		float y2 = 0;
		bool m_bInRange = false;
		unsigned numOfEnemy = 0;

		Rectangle(float _x, float _y, float _width, float _height)
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

	Level();
	~Level();

	void init(float mapHeight, float mapWidth, float maxRoomHeight, float maxRoomWidth, int maxAttempts);
	void newMap(float mapHeight, float mapWidth, float maxRoomHeight, float maxRoomWidth, int maxAttempts);

	void setMaxRooms(int num);
	int getMaxRooms();
	void setMapWidth(float size);
	float getMapWidth();
	void setMapHeight(float size);
	float getMapHeight();
	void setMaxRoomHeight(float size);
	float getMaxRoomHeight();
	void setMaxRoomWidth(float size);
	float getMaxRoomWidth();


	void testCout();
	

	void generate();
	void createMap(int spawnAttempts);
	void generateCorridors();
	void addCorridor(float x, float y, int dir, int l);
	void generateRooms();
	void addRoom(float roomPosX, float roomPosY, float roomWidth, float roomHeight);
	bool isRoomOverlap(float roomPosX, float roomPosY, float roomWidth, float roomHeight);
	void generateWalls();

	void setUp();
	void spawnExit();
	void spawnTeleporter();
	void spawnEnemies();
	void clearEntitys();
	void loadEntitys();
	void newLevel();
	void updateEnemy();

	bool isTileWall(Tile t);
	bool setTile(int x, int y, Tile::TILE_TYPE);
	Tile getTile(int x, int y);
	vector<Rectangle> getRooms(void);
	
private:
	int maxRooms;
	float mapWidth, mapHeight, maxRoomWidth, maxRoomHeight;
	int roomCount, corrCount;
	int level;
	bool spawnBoss;
	map<pair<int, int>, Tile> levelMap;
	vector<Rectangle> rooms;

};

#endif // !TILEMAP_H
