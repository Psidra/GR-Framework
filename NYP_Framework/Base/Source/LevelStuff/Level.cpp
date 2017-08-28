#include "../DetectMemoryLeak.h"
#include "Level.h"
#include "MyMath.h"
#include <iostream>
#include "Vector3.h"
#include <algorithm>

#include "TileEntity.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../EntityManager.h"
#include "../Minimap/Minimap.h"

Level::Level() : roomCount(0)
{
}

Level::~Level()
{
}

void Level::init(float mapHeight, float mapWidth, float maxRoomHeight, float maxRoomWidth, int maxAttempts)
{
	this->mapHeight = mapHeight;
	this->mapWidth = mapWidth;
	this->maxRoomHeight = maxRoomHeight;
	this->maxRoomWidth = maxRoomWidth;
	roomCount = 0;
	corrCount = 0;
	generate();
	createMap(maxAttempts);
	//testCout();
}

void Level::setMaxRooms(int num)
{
	maxRooms = num;
}

int Level::getMaxRooms()
{
	return maxRooms;
}

void Level::setMapWidth(float size)
{
	mapWidth = size;
}

float Level::getMapWidth()
{
	return mapWidth;
}

void Level::setMapHeight(float size)
{
	mapHeight = size;
}

float Level::getMapHeight()
{
	return mapHeight;
}

void Level::setMaxRoomHeight(float size)
{
	maxRoomHeight = size;
}

float Level::getMaxRoomHeight()
{
	return maxRoomHeight;
}

void Level::setMaxRoomWidth(float size)
{
	maxRoomWidth = size;
}

float Level::getMaxRoomWidth()
{
	return maxRoomWidth;
}

void Level::generateRooms()
{
	float rw = Math::RandFloatMinMax(maxRoomWidth - 3, maxRoomWidth);
	float rh = Math::RandFloatMinMax(maxRoomHeight - 3, maxRoomHeight);
	float rx = Math::RandFloatMinMax(1, mapWidth - rw - 1);
	float ry = Math::RandFloatMinMax(1, mapHeight - rh - 1);

	if (!isRoomOverlap(rx, ry, rw, rh))
		addRoom(rx, ry, rw, rh);
	
}

void Level::addRoom(float roomPosX, float roomPosY, float roomWidth, float roomHeight)
{
	for (int i = roomPosX; i < (roomWidth + roomPosX) ; ++i)
	{
		for (int j = roomPosY; j < (roomHeight + roomPosY); ++j)
		{
			setTile(i, j, Tile::ROOM);
		}
	}

	rooms.push_back(Rectangle(roomPosX, roomPosY, roomWidth, roomHeight));
	roomCount++;
}

bool Level::isRoomOverlap(float roomPosX, float roomPosY, float roomWidth, float roomHeight)
{
	for (int i = roomPosX; i < (roomWidth + roomPosX); ++i)
	{
		for (int j = roomPosY; j < (roomHeight + roomPosY); ++j)
		{
			if (getTile(i, j).type == Tile::ROOM)
			{
				std::cout << "OverLAP AHHHHHHH" << std::endl;
				return true;
			}
		}
	}

	return false;
}

void Level::generateCorridors()
{
	Rectangle rec1, rec2;
	int p1 = 0, p2 = 0;
	int	x = 0, y = 0, start = 0, end = 0;

	for (int i = 0; i < roomCount - 1; ++i)
	{
		rec1 = rooms[i];
		rec2 = rooms[i + 1];

		p1 = rec1.getMidPoint().x;
		p2 = rec2.getMidPoint().x;

		start = p1;
		end = p2;
		y = rec1.getMidPoint().y;

		if (p1 > p2)
		{
			start = p2;
			end = p1;
			y = rec2.getMidPoint().y;
		}

		addCorridor(start, y, 2, (end - start));

		p1 = rec1.getMidPoint().y;
		p2 = rec2.getMidPoint().y;

		if (rec1.getMidPoint().x > rec2.getMidPoint().x) 
			x = rec1.getMidPoint().x;
		else 
			x = rec2.getMidPoint().x;

		start = p1;
		end = p2;

		if (p1 > p2)
		{
			start = p2;
			end = p1;
		}

		addCorridor(x, start, 3, (end - start));
	}
}

void Level::addCorridor(float x, float y, int dir, int l)
{
	if (dir == 3) {
		for (int i = y; i < y + l + 1; ++i) setTile(x, i, Tile::CORRIDOR);
	}
	else if (dir == 1) {
		for (int i = (y - l) + 1; i < y + 1; ++i) setTile(x, i, Tile::CORRIDOR);
	}
	else if (dir == 2) {
		for (int i = x; i < (x + l - 1) + 1;  ++i) setTile(i, y, Tile::CORRIDOR);
	}
	else if (dir == 4) {
		for (int i = (x - l) + 1; i < x + 1;  ++i) setTile(i, y, Tile::CORRIDOR);
	}
	
	corrCount++;
}

void Level::generateWalls()
{
	Tile *currentTile;
	Tile *nextTile;
	Tile *prevTile;
	Tile *upTile;
	Tile *loTile;

	// Add Walls in 4 direction (Up, Down, Left, Right)
	for (int x = 0; x < mapWidth; ++x) {
		for (int y = 0; y < mapHeight; ++y) {

			currentTile = &levelMap[{x, y}];
			nextTile = &levelMap[{x + 1, y}];
			prevTile = &levelMap[{x - 1, y}];
			upTile = &levelMap[{x, y + 1}];
			loTile = &levelMap[{x, y - 1}];

			if (prevTile->type == Tile::EMPTY && (currentTile->type == Tile::ROOM || currentTile->type == Tile::CORRIDOR))
				prevTile->type = Tile::WALL;

			if (currentTile->type == Tile::EMPTY) {
				if (nextTile->type == Tile::ROOM || nextTile->type == Tile::CORRIDOR)
					currentTile->type = Tile::WALL;
				else if (prevTile->type == Tile::ROOM || prevTile->type == Tile::CORRIDOR)
					currentTile->type = Tile::WALL;
				else if (upTile->type == Tile::ROOM || upTile->type == Tile::CORRIDOR)
					currentTile->type = Tile::WALL;
				else if (loTile->type == Tile::ROOM || loTile->type == Tile::CORRIDOR)
					currentTile->type = Tile::WALL;
			}
		}
	}

	//Add Walls in 4 coners
	/*for (int x = 0; x < mapWidth; ++x) {
		for (int y = 0; y < mapHeight; ++y) {

			currentTile = &levelMap[{x, y}];
			nextTile = &levelMap[{x + 1, y}];
			prevTile = &levelMap[{x - 1, y}];
			upTile = &levelMap[{x, y + 1}];
			loTile = &levelMap[{x, y - 1}];

			if (currentTile->type == Tile::EMPTY) {
				if (isTileWall(*nextTile) && isTileWall(*loTile))
					currentTile->type = Tile::WALL;
				else if (isTileWall(*nextTile) && isTileWall(*upTile))
					currentTile->type = Tile::WALL;
				else if (isTileWall(*prevTile) && isTileWall(*loTile))
					currentTile->type = Tile::WALL;
				else if (isTileWall(*prevTile) && isTileWall(*upTile))
					currentTile->type = Tile::WALL;
			}
		}
	}*/
}

void Level::setUp()
{
	spawnExit();
	spawnTeleporter();
	loadEntitys();
	CMinimap::GetInstance()->Init();


	//Locate a suitable location to spawn the player.
	bool spawnable = false;
	while (!spawnable)
	{
		int rndX = Math::RandIntMinMax(0, mapWidth);
		int rndY = Math::RandIntMinMax(0, mapHeight);

		if (getTile(rndX, rndY).type == Tile::ROOM)
		{
			spawnable = true;
			Player::GetInstance()->SetPos(Vector3(rndX, rndY, 1));
			break;
		}
	}
}

void Level::spawnExit()
{
	int roomSelected = Math::RandIntMinMax(0, roomCount - 1);
	int randX = Math::RandIntMinMax(rooms[roomSelected].x, rooms[roomSelected].x2);
	int randY = Math::RandIntMinMax(rooms[roomSelected].y, rooms[roomSelected].y2);
	
	setTile(randX, randY, Tile::EXIT);
	//setTile(rooms[roomSelected].getMidPoint().x, rooms[roomSelected].getMidPoint().y, Tile::TELEPORTER);
}

void Level::spawnTeleporter()
{
	int roomSelected = Math::RandIntMinMax(0, roomCount - 1);

	setTile(rooms[roomSelected].getMidPoint().x, rooms[roomSelected].getMidPoint().y, Tile::TELEPORTER);
}

void Level::clearEntitys()
{
	levelMap.clear();
	rooms.clear();
	EntityManager::GetInstance()->getEntityList().clear();

	std::list<EntityBase*>::iterator it, end;
	it = EntityManager::GetInstance()->getCollisionList().begin();
	while (it != EntityManager::GetInstance()->getCollisionList().end()) {
		(*it)->SetIsDone(true);
		++it;
	}
	EntityManager::GetInstance()->getCollisionList().clear();
}

void Level::loadEntitys()
{
	for (size_t i = 0; i < mapWidth; ++i)
	{
		for (size_t j = 0; j < mapHeight; ++j)
		{
			TileEntity* temp = NULL;

			switch (getTile(i, j).type)
			{
			case Tile::WALL:
				temp = Create::TEntity("tile_floor", Vector3(i, j, 0), Vector3(1, 1, 1), true);
				temp->type = GenericEntity::OBJECT_TYPE::WALL;
				temp->setNormal(Vector3(1, 0, 0));
				break;
			case Tile::EXIT:
				temp = Create::TEntity("player", Vector3(i, j, 0), Vector3(1, 1, 1), true);
				temp->type = GenericEntity::OBJECT_TYPE::EXIT;
				temp->setNormal(Vector3(1, 0, 0));
				break;
			case Tile::TELEPORTER:
				temp = Create::TEntity("greenCube", Vector3(i, j, 0.0f), Vector3(1, 1, 1), true);
				temp->type = GenericEntity::OBJECT_TYPE::TELEPORTER;
				break;
			default:
				break;
			}

			if (!temp)
				continue;

			temp->SetAABB(temp->GetScale() * 0.5f + temp->GetPosition(), temp->GetScale() * -0.5f + temp->GetPosition());
		}
	}
}

void Level::newLevel()
{
	clearEntitys();
	GetInstance()->init(32.f, 32.f, 12.f, 12.f, 30);
	setUp();
}

void Level::testCout()
{
	for (size_t i = 0; i < mapWidth; ++i)
	{
		for (size_t j = 0; j < mapHeight; ++j)
		{
			if (getTile(i,j).type == Tile::EMPTY)
				std::cout << "E";
			else if (getTile(i, j).type == Tile::ROOM)
				std::cout << "0";
			else if (getTile(i, j).type == Tile::CORRIDOR)
				std::cout << "X";
			else if (getTile(i, j).type == Tile::WALL)
				std::cout << "W";

		}

		std::cout << std::endl;
	}
}

void Level::generate()
{
	for (int x = 0; x < mapWidth; ++x)
	{
		for (int y = 0; y < mapHeight; ++y)
		{
			//levelMap[x].push_back(vector<Tile>);
			levelMap[std::make_pair(x,y)] = Tile(Tile::EMPTY);
		}
	}
}

void Level::createMap(int spawnAttempts)
{
	for (int i = 0; i < spawnAttempts; ++i)
	{
		generateRooms();
	}

	std::cout << "Number of Rooms" << roomCount << std::endl;

	generateCorridors();
	generateWalls();
}

bool Level::isTileWall(Tile t)
{
	return t.type == Tile::WALL;
}

bool Level::setTile(int x, int y, Tile::TILE_TYPE type)
{
	if (&getTile(x, y) != NULL)
	{
		levelMap[{x, y}].type = type;
		return true;
	}

	return false;
}

Tile Level::getTile(int x, int y)
{
	return levelMap[{x, y}];
}

vector<Level::Rectangle> Level::getRooms(void)
{
	return rooms;
}


