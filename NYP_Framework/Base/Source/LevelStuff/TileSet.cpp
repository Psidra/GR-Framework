#include "../DetectMemoryLeak.h"
#include "TileSet.h"

Tile::Tile() : type(TILE_TYPE::EMPTY)
{
}

Tile::~Tile()
{
}

Tile::Tile(TILE_TYPE t) : type(t)
{
}
