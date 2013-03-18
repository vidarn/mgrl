#include "stairs.h"
#include "boost/random/uniform_real_distribution.hpp"

ROOM_CONS(StairsRoom)
{
	m_radius = 4;
    boost::random::uniform_int_distribution<> xDist(2+m_radius,w-2-m_radius);
    boost::random::uniform_int_distribution<> yDist(2+m_radius,h-2-m_radius);
    m_x = xDist(RAND);
    m_y = yDist(RAND);
    boost::random::uniform_int_distribution<> doorDist(0,3);
    for(int y=-1;y<=1;y++){
        for(int x=-1;x<=1;x++){
            if(abs(x) != abs(y)){
                CavernConnectivityPoint point;
                point.x = m_radius*x + m_x;
                point.y = m_radius*y + m_y;
                point.tag = -1;
                point.type = CON_DUN;
                m_doors.push_back(point);
            }
        }
    }
}

void
StairsRoom::renderRoom(Tile *tiles, TileFactory *tileFactory)
{
	for(int y=-m_radius;y<=m_radius;y++){
		for(int x=-m_radius;x<=m_radius;x++){
			if(sqrtf(float(x*x)+float(y*y)) < float(m_radius)+0.6f){
                Tile *tile = getTile(x,y,tiles);
                if(tile!=0){
                    bool ok = true;
                    for(unsigned int i=0;i<m_doors.size();i++){
                        if(m_doors[i].x - m_x == x && m_doors[i].y - m_y == y){
                            ok = false;
                        }
                    }
                    if(ok){
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
StairsRoom::decorateRoom(Tile *tiles, TileFactory *tileFactory, std::vector<DecorationPlacement> &places)
{
    for(unsigned int i=0;i<m_doors.size();i++){
        CavernConnectivityPoint &point = m_doors[i];
        addDecoration(point.x-m_x,point.y-m_y,DEC_DOOR_CLOSED,places);
    }
    addDecoration(-1,0,DEC_STAIRS_DOWN,places);
    addDecoration( 1,0,DEC_STAIRS_UP,places);
}

void
StairsRoom::reserveRoom(char *tiles)
{
	for(int y=-m_radius-1;y<m_radius+2;y++){
		for(int x=-m_radius-1;x<m_radius+2;x++){
            tiles[m_x+x+(m_y+y)*m_stride] = 126;
		}
	}
}

bool
StairsRoom::validateRoom(char *tiles)
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
