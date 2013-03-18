#include "shard.h"
#include "boost/random/uniform_real_distribution.hpp"

ROOM_CONS(ShardRoom, bool orb)
{
	m_radius = 4;
    boost::random::uniform_int_distribution<> xDist(2+m_radius,w-2-m_radius);
    boost::random::uniform_int_distribution<> yDist(2+m_radius,h-2-m_radius);
    m_x = xDist(RAND);
    m_y = yDist(RAND);
    m_orb = orb;
}

void
ShardRoom::renderRoom(Tile *tiles, TileFactory *tileFactory)
{
}

void
ShardRoom::decorateRoom(Tile *tiles, TileFactory *tileFactory, std::vector<DecorationPlacement> &places)
{
    boost::random::uniform_real_distribution<> noiseDist(-3.f,4.f);
	for(int y=-m_radius;y<=m_radius;y++){
		for(int x=-m_radius;x<=m_radius;x++){
			if(sqrtf(float(x*x)+float(y*y)) < float(m_radius)-0.6f+noiseDist(RAND)){
                setTile(x,y,"Crystal Floor", tiles, tileFactory);
			}
		}
	}
    if(m_orb){
        setTile(0,0,"Orb of Knowledge", tiles, tileFactory);
        setTile( 1, 1,"Shard of Knowledge", tiles, tileFactory);
        setTile( 1,-1,"Shard of Knowledge", tiles, tileFactory);
        setTile(-1, 1,"Shard of Knowledge", tiles, tileFactory);
        setTile(-1,-1,"Shard of Knowledge", tiles, tileFactory);
    }
    else{
        setTile(0,0,"Shard of Knowledge", tiles, tileFactory);
    }
}

void
ShardRoom::reserveRoom(char *tiles)
{
}

bool
ShardRoom::validateRoom(char *tiles)
{
    return true;
}
