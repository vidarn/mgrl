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
    boost::random::uniform_int_distribution<> numDoorsDist(1,3);
    boost::random::uniform_int_distribution<> doorDist(0,3);
    int numDoors = numDoorsDist(RAND);
    for(int i=0;i<numDoors;i++){
        CavernConnectivityPoint point;
        point.x = m_radius;
        point.y = m_radius;
        switch(doorDist(RAND)){
            case 0:
                point.x *= 0;
                point.y *= -1;
                break;
            case 1:
                point.x *= 0;
                point.y *= 1;
                break;
            case 2:
                point.x *= 1;
                point.y *= 0;
                break;
            case 3:
                point.x *= -1;
                point.y *= 0;
                break;
        }
        point.x += m_x;
        point.y += m_y;
        point.tag = -1;
        point.type = CON_DUN;
        m_doors.push_back(point);
    }
}

void
RoundRoom::renderRoom(Tile *tiles, TileFactory *tileFactory)
{
	for(int y=-m_radius;y<=m_radius;y++){
		for(int x=-m_radius;x<=m_radius;x++){
			if(sqrtf(float(x*x)+float(y*y)) < float(m_radius)+0.6f){
                Tile *tile = getTile(x,y,tiles);
                if(tile!=0){
                    if(tile->m_walkable!=1){
                        setTile(x,y,"Dungeon Wall",tiles,tileFactory);
                    }
                    else{
                        setTile(x,y,"Dungeon Floor", tiles, tileFactory);
                    }
                }
			}
			if(sqrtf(float(x*x)+float(y*y)) < float(m_radius)-0.6f){
                setTile(x,y,"Dungeon Floor", tiles, tileFactory);
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
    for(int i=0;i<m_doors.size();i++){
        CavernConnectivityPoint &point = m_doors[i];
        addDecoration(point.x-m_x,point.y-m_y,DEC_DOOR,places);
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
