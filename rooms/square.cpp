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
    m_roomWidth  = m_w;
    m_roomHeight = m_h;
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
    for(int y=0;y<m_h;y++){
        for(int x=0;x<m_w;x++){
            if(x==0 || y==0 || x==m_w-1 || y==m_h-1){
                Tile *tile = getTile(x,y,tiles);
                if(tile!=0){
                    if(tile->m_walkable!=1){
                        setTile(x,y,"Dungeon Wall",tiles,tileFactory);
                    }
                    else{
                        setTile(x,y,"Dungeon Floor",tiles,tileFactory);
                    }
                }
            }
            else{
                setTile(x,y,"Dungeon Floor",tiles,tileFactory);
            }
        }
    }
}

void
SquareRoom::decorateRoom(Tile *tiles, TileFactory *tileFactory, std::vector<DecorationPlacement> &places)
{
    boost::random::uniform_int_distribution<> xDist(1,m_w-2);
    boost::random::uniform_int_distribution<> yDist(1,m_h-2);
    double weights[] = {
		0.20f,
        1.00f,
        1.00f,
        1.00f,
        1.50f,
        0.30f,
    };
    boost::random::discrete_distribution<> patternDist(weights);
    int pattern = patternDist(RAND) -1;
    if(pattern >=0){
        if(m_w < 4 || m_h < 4){
            addDecoration(xDist(RAND),yDist(RAND),DEC_RANDOM,places);
        }
        else{
            switch(pattern){
                case 0: // corners
                    addDecoration(  1  ,  1  ,DEC_PATTERN,places);
                    addDecoration(  1  ,m_h-2,DEC_PATTERN,places);
                    addDecoration(m_w-2,  1  ,DEC_PATTERN,places);
                    addDecoration(m_w-2,m_h-2,DEC_PATTERN,places);
                    break;
                case 1: // corners, one in from walls
                    addDecoration(  2  ,  2  ,DEC_PATTERN,places);
                    addDecoration(  2  ,m_h-3,DEC_PATTERN,places);
                    addDecoration(m_w-3,  2  ,DEC_PATTERN,places);
                    addDecoration(m_w-3,m_h-3,DEC_PATTERN,places);
                    break;
                case 2: // one random
                    addDecoration(xDist(RAND),yDist(RAND),DEC_RANDOM,places);
                    break;
                case 3: // few random
                    {
                        boost::random::uniform_int_distribution<> numDist(1,3);
                        int numRand = numDist(RAND);
                        for(int i=0;i<m_w;i++){
                            addDecoration(xDist(RAND),yDist(RAND),DEC_RANDOM,places);
                        }
                    }
                    break;
                case 4: // many random
                    {
                        boost::random::uniform_int_distribution<> numDist(4,7);
                        int numRand = numDist(RAND);
                        for(int i=0;i<m_w;i++){
                            addDecoration(xDist(RAND),yDist(RAND),DEC_RANDOM,places);
                        }
                    }
                    break;
            }
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
