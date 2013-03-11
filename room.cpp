#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/discrete_distribution.hpp>
#include "room.h"

Room::Room(char *tiles, int w, int h):
    m_stride(w)
{
}

void
Room::render(char *tiles)
{
    renderRoom(tiles);
}

void
Room::reserve(char *tiles)
{
    reserveRoom(tiles);
    for(int i=0;i<m_doors.size();i++){
        CavernConnectivityPoint &point = m_doors[i];
        tiles[point.x+point.y*m_stride] = 126;
    }
}

bool
Room::validate(char *tiles, std::vector<CavernConnectivityPoint> &points, int id)
{
    if(!validateRoom(tiles)){
        return false;
    }
    for(int i=0;i<m_doors.size();i++){
        m_doors[i].roomID = id;
        points.push_back(m_doors[i]);
    }
    m_id = id;
    return true;
}

bool
Room::validateTile(char tile)
{
    return tile < 126;
}

Room *
Room::getRoom(int dungeonLevel, char *tiles, int w, int h)
{
    return new SquareRoom(tiles, w, h, 0);
}

ROOM_CONS(SquareRoom, int size)
{
    boost::random::uniform_int_distribution<> sizeDist(4,7);
    m_w = sizeDist(RAND);
    m_h = sizeDist(RAND);
    boost::random::uniform_int_distribution<> xDist(2,w-2-m_w);
    boost::random::uniform_int_distribution<> yDist(2,h-2-m_h);
    m_x = xDist(RAND);
    m_y = yDist(RAND);

    boost::random::uniform_int_distribution<> doorWallDist(0,3);
    boost::random::uniform_int_distribution<> numDoorsDist(1,3);
    boost::random::uniform_int_distribution<> doorXDist(1,m_w-1);
    boost::random::uniform_int_distribution<> doorYDist(1,m_h-1);
    int numDoors = numDoorsDist(RAND);
    for(int i=0;i<numDoors;i++){
        CavernConnectivityPoint point;
        int doorWall = doorWallDist(RAND);
        point.x = doorXDist(RAND) + m_x;
        point.y = doorYDist(RAND) + m_y;
        point.x = m_x + m_w/2;
        point.y = m_y + m_h/2;
        switch(doorWall){
            case 0: point.x = m_x;break;
            case 1: point.x = m_x + m_w-1;break;
            case 2: point.y = m_y;break;
            case 3: point.y = m_y + m_h-1;break;
        }
        point.tag = -1;
        point.type = CON_DUN;
        m_doors.push_back(point);
    }
}

void
SquareRoom::renderRoom(char *tiles)
{
    for(int y=m_y+1;y<m_y+m_h-1;y++){
        for(int x=m_x+1;x<m_x+m_w-1;x++){
            tiles[x+y*m_stride] = 0;
        }
    }
}

void
SquareRoom::reserveRoom(char *tiles)
{
    for(int y=m_y;y<m_y+m_h;y++){
        for(int x=m_x;x<m_x+m_w;x++){
            tiles[x+y*m_stride] = 127;
        }
    }
}

bool
SquareRoom::validateRoom(char *tiles)
{
    for(int y=m_y;y<m_y+m_h;y++){
        for(int x=m_x;x<m_x+m_w;x++){
            if(!validateTile(tiles[x+y*m_stride])){
                return false;
            }
        }
    }
    return true;
}
