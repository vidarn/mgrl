#include "round.h"
#include <boost/random/uniform_real_distribution.hpp>

ROOM_CONS(RoundRoom)
{
    boost::random::uniform_int_distribution<> radiusDist(2,5);
	m_radius = radiusDist(RAND);
    boost::random::uniform_int_distribution<> xDist(2+m_radius,w-2-m_radius);
    boost::random::uniform_int_distribution<> yDist(2+m_radius,h-2-m_radius);
    m_x = xDist(RAND);
    m_y = yDist(RAND);
    boost::random::uniform_real_distribution<> angleDist(0.f,3.14f*2.f);
	CavernConnectivityPoint point;
	float doorAngle = angleDist(RAND);
	point.x = m_x+cos(doorAngle)*float(m_radius-1.0f);
	point.y = m_y+sin(doorAngle)*float(m_radius-1.0f);
	point.tag = -1;
	point.type = CON_DUN;
	m_doors.push_back(point);
}

void
RoundRoom::renderRoom(Tile *tiles, TileFactory *tileFactory)
{
	for(int y=-m_radius;y<=m_radius;y++){
		for(int x=-m_radius;x<=m_radius;x++){
			if(sqrtf(float(x*x)+float(y*y)) < float(m_radius)-0.6f){
                setTile(x,y,"Stone Floor", tiles, tileFactory);
			}
		}
	}
}

void
RoundRoom::decorateRoom(Tile *tiles, TileFactory *tileFactory, std::vector<DecorationPlacement> &places)
{
    boost::random::uniform_int_distribution<> patternDist(-1,0);
    int pattern = patternDist(RAND);
    if(pattern >=0){
        addDecoration(0,0,DEC_PATTERN,places);
    }
}

void
RoundRoom::reserveRoom(char *tiles)
{
	for(int y=-m_radius;y<m_radius+1;y++){
		for(int x=-m_radius;x<m_radius+1;x++){
			if(sqrtf(float(x*x)+float(y*y)) < float(m_radius)-0.6f+1){
				tiles[m_x+x+(m_y+y)*m_stride] = 127;
			}
		}
	}
}

bool
RoundRoom::validateRoom(char *tiles)
{
	for(int y=-m_radius;y<m_radius+1;y++){
		for(int x=-m_radius;x<m_radius+1;x++){
			if(sqrtf(float(x*x)+float(y*y)) < float(m_radius)-0.6f+1){
				if(!validateTile(tiles[m_x+x+(m_y+y)*m_stride])){
					return false;
				}
			}
		}
	}
    return true;
}
