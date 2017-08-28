#ifndef TILESET_H
#define TILESET_H

class Tile
{
public:
	enum TILE_TYPE
	{
		EMPTY = 0,
		TOP_WALL,
		BOT_WALL,
		RIGHT_WALL,
		LEFT_WALL,
		FLOOR,
		WALL,
		ROOM,
		CORRIDOR,
		TELEPORTER,
		ENEMY,
		EXIT,
		BOSS,
		COUNT,
	};

	TILE_TYPE type;         // The type of tile this is.
	
	Tile();
	~Tile();
	Tile(TILE_TYPE t);
private:

};
#endif // !TILESET_H

