#include <boost/random/uniform_01.hpp>
#include "flood.h"

DEC_CONS(FloodDecoration)
{
}

DEC_VAL(FloodDecoration)
{
	return true;
}

DEC_RES(FloodDecoration)
{
	reserveTile(0,0, tiles);
}

DEC_REN(FloodDecoration)
{
	std::string tileName;
	tileName = "Water Stream";
	float frequency = 0.9f;
	recursiveFlood(0,0,0,tileName,frequency,tiles,tileFactory);
}

void
FloodDecoration::recursiveFlood(int recursionDepth, int x, int y, std::string &name, float frequency,  Tile *tiles, TileFactory *tileFactory)
{
    boost::random::uniform_int_distribution<> endDist(2,8);
	boost::random::uniform_01<> randDist;
	int endDepth = endDist(RAND);
	Tile *tile = getTile(x,y,tiles);
	if(tile != 0 && recursionDepth < endDepth && tile->m_walkable && tile->m_glyph != '~'){
		setTile(x,y,name,tiles,tileFactory);
		for(int yy=-1;yy<=1;yy++){
			for(int xx=-1;xx<=1;xx++){
				if(randDist(RAND)<frequency)
					recursiveFlood(recursionDepth+1,x+xx,y+yy,name,frequency,tiles,tileFactory);
			}
		}
	}
}
