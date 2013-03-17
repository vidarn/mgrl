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
    return true;
}

DEC_REN(FloodDecoration)
{
	std::string tileName, wallName;
	tileName = "Dungeon Floor";
	wallName = "Dungeon Wall";
	float frequency = 0.8f;
    m_visited = new bool[m_w*m_h];
    for(int i=0;i<m_w*m_h;i++){
        m_visited[i] = false;
    }
	recursiveFlood(0,0,0,tileName, wallName,frequency,tiles,tileFactory);
}

void
FloodDecoration::recursiveFlood(int recursionDepth, int x, int y, std::string &name, std::string &wallName, float frequency,  Tile *tiles, TileFactory *tileFactory)
{
    boost::random::uniform_int_distribution<> endDist(3,5);
	boost::random::uniform_01<> randDist;
	int endDepth = endDist(RAND);
	Tile *tile = getTile(x,y,tiles);
	if(tile != 0 && recursionDepth < endDepth && !m_visited[x+m_x+(y+m_y)*m_w]){
        if(tile->m_walkable ){
            setTile(x,y,name,tiles,tileFactory);
            for(int yy=-1;yy<=1;yy++){
                for(int xx=-1;xx<=1;xx++){
                    if(randDist(RAND)<frequency){
                        recursiveFlood(recursionDepth+1,x+xx,y+yy,name,wallName,frequency,tiles,tileFactory);
                    }
                }
            }
        }
        else{
            setTile(x,y,wallName,tiles,tileFactory);
        }
        m_visited[x+m_x+(y+m_y)*m_w] = true;
	}
}

FloodDecoration::~FloodDecoration()
{
    delete[] m_visited;
}
