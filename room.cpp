#include "room.h"
#include "rooms/square.h"
#include "rooms/round.h"
#include "rooms/pool.h"
#include "decorations/pillar.h"
#include "decorations/dinner.h"
#include "decorations/spider.h"
#include "decorations/sarcophagus.h"
#include "decorations/flood.h"
#include "level.h"

Room::Room(char *tiles, int w, int h):
    m_stride(w), m_height(h), m_roomWidth(0), m_roomHeight(0)
{
}

void
Room::render(Tile *tiles, TileFactory *tileFactory)
{
    renderRoom(tiles, tileFactory);
}

void
Room::decorate(char *charTiles, Tile *tiles, TileFactory *tileFactory, Level *level)
{
    std::vector<std::string> creatureTags;
    std::vector<DecorationPlacement> decorationPlacements;
    std::vector<Decoration *> decorations;
    decorateRoom(tiles, tileFactory, decorationPlacements);
    int patternId = -1;
    int randomId = -1;
    for(int i=0;i<decorationPlacements.size();i++){
        DecorationPlacement &place = decorationPlacements[i];
        Decoration *dec;
        if(place.m_type == DEC_PATTERN)
            dec = getPatternDecoration(patternId,place, creatureTags);
        else
            dec = getRandomDecoration(randomId, place, creatureTags);
        if(dec != 0){
            if(dec->validate(charTiles)){
                dec->reserve(charTiles);
                decorations.push_back(dec);
            }
            else{
                delete dec;
            }
        }
    }
    for(int i=0;i<decorations.size();i++){
        decorations[i]->render(tiles, tileFactory);
        delete decorations[i];
    }
    if(m_roomWidth > 0 && m_roomHeight > 0){
        boost::random::uniform_int_distribution<> creatureNumDist(3,5);
        boost::random::uniform_int_distribution<> xDist(0,m_roomWidth);
        boost::random::uniform_int_distribution<> yDist(0,m_roomHeight);
        int numCreatures = creatureNumDist(RAND);
        for(int i=0;i<numCreatures;i++){
            int x = xDist(RAND);
            int y = yDist(RAND);
            Tile *tile = getTile(x,y,tiles);
            if(tile != 0 && tile->m_walkable){
                Actor *actor = level->m_actorFactory.getCreature(1,level,creatureTags);
                if(actor != 0){
                    actor->m_x = m_x+x;
                    actor->m_y = m_y+y;
                    level->m_actors.push_back(actor);
                }
            }
        }
    }
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

void
Room::reserveWall(int x, int y, char *tiles)
{
    x+=m_x;y+=m_y;
    if(x>0 && x<m_stride-1 && y>0 && y<m_height-1)
        tiles[x+y*m_stride] = 127;
}

void
Room::reserveFloor(int x, int y, char *tiles)
{
    x+=m_x;y+=m_y;
    if(x>0 && x<m_stride-1 && y>0 && y<m_height-1)
        tiles[x+y*m_stride] = 126;
}

void
Room::addDecoration(int x, int y, int type, std::vector<DecorationPlacement> &places)
{
    x+=m_x;y+=m_y;
    if(x>0 && x<m_stride-1 && y>0 && y<m_height-1){
        DecorationPlacement place(x,y,type);
        places.push_back(place);
    }
}

void
Room::setTile(int x, int y, std::string tile, Tile *tiles, TileFactory *tileFactory)
{
    x+=m_x;y+=m_y;
    if(x>0 && x<m_stride-1 && y>0 && y<m_height-1)
        tiles[x+y*m_stride] = tileFactory->getTile(tile);
}

Tile *
Room::getTile(int x, int y,  Tile *tiles)
{
    x+=m_x;y+=m_y;
    if(x>0 && x<m_stride-1 && y>0 && y<m_height-1)
        return &tiles[x+y*m_stride];
    return 0;
}

Decoration *
Room::getPatternDecoration(int &id, DecorationPlacement &place, std::vector<std::string> &creatureTags)
{
    double weights[] = {
        1.00f,
        0.20f,
    };
    boost::random::discrete_distribution<> dist(weights);
    if(id == -1)
        id = dist(RAND);
    switch(id){
        case 0:
            return new PillarDecoration(place.m_x, place.m_y, m_stride, m_height);
        case 1:
            return new SarcophagusDecoration(place.m_x, place.m_y, m_stride, m_height);
    }
    return 0;
}

Decoration *
Room::getRandomDecoration(int &id, DecorationPlacement &place, std::vector<std::string> &creatureTags)
{
    double weights[] = {
        1.00f,
        0.20f,
        0.00f,
    };
    boost::random::discrete_distribution<> dist(weights);
    int a = dist(RAND);
    if(id != -1)
        a = id;
    switch(a){
        case 0:
            return new DinnerDecoration(place.m_x, place.m_y, m_stride, m_height);
        case 1:
            id = a;
            creatureTags.push_back(std::string("spider"));
            return new SpiderDecoration(place.m_x, place.m_y, m_stride, m_height);
    }
    return 0;
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
        0.20f,
    };
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

