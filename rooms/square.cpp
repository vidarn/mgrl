#include "square.h"

ROOM_CONS(SquareRoom, int size)
{
    boost::random::uniform_int_distribution<> sizeDist(7,7);
    switch(size){
        case ROOM_SQUARE_SMALL:
            sizeDist = boost::random::uniform_int_distribution<>(4,7);break;
        case ROOM_SQUARE_MEDIUM:
            sizeDist = boost::random::uniform_int_distribution<>(6,9);break;
        case ROOM_SQUARE_LARGE:
            sizeDist = boost::random::uniform_int_distribution<>(10,13);break;
        case ROOM_SQUARE_HUGE:
            sizeDist = boost::random::uniform_int_distribution<>(15,20);break;
    }
    m_w = sizeDist(RAND);
    m_h = sizeDist(RAND);
    boost::random::uniform_int_distribution<> xDist(2,w-2-m_w);
    boost::random::uniform_int_distribution<> yDist(2,h-2-m_h);
    m_x = xDist(RAND);
    m_y = yDist(RAND);

    boost::random::uniform_int_distribution<> doorWallDist(0,3);
    boost::random::uniform_int_distribution<> numDoorsDist(1,3);
    switch(size){
        case ROOM_SQUARE_SMALL:
            numDoorsDist = boost::random::uniform_int_distribution<>(1,2);break;
        case ROOM_SQUARE_MEDIUM:
            numDoorsDist = boost::random::uniform_int_distribution<>(1,2);break;
        case ROOM_SQUARE_LARGE:
            numDoorsDist = boost::random::uniform_int_distribution<>(1,3);break;
        case ROOM_SQUARE_HUGE:
            numDoorsDist = boost::random::uniform_int_distribution<>(2,5);break;
    }
    boost::random::uniform_int_distribution<> doorXDist(2,m_w-2);
    boost::random::uniform_int_distribution<> doorYDist(2,m_h-2);
    int numDoors = numDoorsDist(RAND);
    for(int i=0;i<numDoors;i++){
        CavernConnectivityPoint point;
        int doorWall = doorWallDist(RAND);
        point.x = doorXDist(RAND) + m_x;
        point.y = doorYDist(RAND) + m_y;
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
SquareRoom::renderRoom(Tile *tiles, TileFactory *tileFactory)
{
    for(int y=m_y+1;y<m_y+m_h-1;y++){
        for(int x=m_x+1;x<m_x+m_w-1;x++){
            tiles[x+y*m_stride] = tileFactory->getTile("Stone Floor");
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
