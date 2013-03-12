#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/discrete_distribution.hpp>
#include "room.h"
#include "rooms/square.h"
#include "rooms/round.h"
#include "rooms/pool.h"

Room::Room(char *tiles, int w, int h):
    m_stride(w)
{
}

void
Room::render(Tile *tiles, TileFactory *tileFactory)
{
    renderRoom(tiles, tileFactory);
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
    double weights[] = {
        0.80f,
        2.20f,
        0.40f,
        0.10f,
        0.45f,
        0.30f,
    };
    /*int weightsSize = sizeof(weights)/sizeof(*weights);
    double totalW = 0;
    for(int i=0;i<weightsSize;i++){
        totalW += weights[i];
    }
    for(int i=0;i<weightsSize;i++){
        weights[i] /= totalW;
    }*/
    boost::random::discrete_distribution<> dist(weights);
    int a = dist(RAND);
    switch(a){
        case 0:
            return new SquareRoom(tiles, w, h, ROOM_SQUARE_SMALL);
        case 1:
            return new SquareRoom(tiles, w, h, ROOM_SQUARE_MEDIUM);
        case 2:
            return new SquareRoom(tiles, w, h, ROOM_SQUARE_LARGE);
        case 3:
            return new SquareRoom(tiles, w, h, ROOM_SQUARE_HUGE);
        case 4:
            return new RoundRoom(tiles, w, h);
        case 5:
            return new PoolRoom(tiles, w, h);
    }
}

