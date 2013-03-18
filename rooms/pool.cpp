#include "pool.h"
#include "boost/random/uniform_real_distribution.hpp"

ROOM_CONS(PoolRoom)
{
    boost::random::uniform_int_distribution<> radiusDist(3,6);
	m_radius = radiusDist(RAND);
    boost::random::uniform_int_distribution<> xDist(2+m_radius,w-2-m_radius);
    boost::random::uniform_int_distribution<> yDist(2+m_radius,h-2-m_radius);
    m_x = xDist(RAND);
    m_y = yDist(RAND);
}

void
PoolRoom::renderRoom(Tile *tiles, TileFactory *tileFactory)
{
}

void
PoolRoom::decorateRoom(Tile *tiles, TileFactory *tileFactory, std::vector<DecorationPlacement> &places)
{
	//TODO make sure it doesn't go outside the dungeon (or at least handle that case)
    boost::random::uniform_real_distribution<> noiseDist(-3.f,4.f);
	for(int y=-m_radius;y<=m_radius;y++){
		for(int x=-m_radius;x<=m_radius;x++){
			if(sqrtf(float(x*x)+float(y*y)) < float(m_radius)-0.6f+noiseDist(RAND)){
                setTile(x,y,"Water Stream", tiles, tileFactory);
			}
		}
	}
}

void
PoolRoom::reserveRoom(char *tiles)
{
}

bool
PoolRoom::validateRoom(char *tiles)
{
    return true;
}
