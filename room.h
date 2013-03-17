#ifndef ROOMS_SCM06W3Y
#define ROOMS_SCM06W3Y
#include <vector>
#include "common.h"
#include "tile.h"
#include "decoration.h"

#define ROOM_DEF(NAME,...) class NAME: public Room{public: NAME(char *tiles, int w, int h, ##__VA_ARGS__); virtual void renderRoom(Tile *tiles, TileFactory *tileFactory); virtual void decorateRoom(Tile *tiles, TileFactory *tileFactory, std::vector<DecorationPlacement> &places); virtual void reserveRoom(char *tiles); virtual bool validateRoom(char *tiles);
#define ROOM_CONS(NAME,...) NAME::NAME(char *tiles, int w, int h, ##__VA_ARGS__):Room(tiles,w,h)

enum{
    CON_CAV,
    CON_DUN,
};

enum{
    DEC_PATTERN,
    DEC_RANDOM,
    DEC_DOOR,
    DEC_DOOR_CLOSED,
    DEC_STAIRS_UP,
    DEC_STAIRS_DOWN,
    DEC_SHARD,
};

struct
CavernConnectivityPoint
{
    int x, y;
    int tag;
    int type;
    int roomID;
};

struct
DecorationPlacement
{
    DecorationPlacement(int x, int y, int type):
        m_x(x),m_y(y),m_type(type)
    {}
    int m_x, m_y;
    int m_type;
};

class Level;

class Room
{
    public:
        Room(char *tiles, int w, int h);
        virtual ~Room();
        void render(Tile *tiles, TileFactory *tileFactory);
        void decorate(char *charTiles, Tile *tiles, TileFactory *tileFactory, Level *level);
        void reserve (char *tiles);
        bool validate(char *tiles, std::vector<CavernConnectivityPoint> &points, int id);
        bool validateTile(char tile);
        void reserveWall (int x, int y, char *tiles);
        void reserveFloor(int x, int y, char *tiles);
        void addDecoration(int x, int y, int type, std::vector<DecorationPlacement> &places);
        void setTile(int x, int y, std::string tile, Tile *tiles, TileFactory *tileFactory);
        Tile * getTile(int x, int y,  Tile *tiles);
        Decoration * getPatternDecoration(int &id, DecorationPlacement &place, std::vector<std::string> &creatureTags);
        Decoration * getRandomDecoration (int &id, DecorationPlacement &place, std::vector<std::string> &creatureTags);
        Decoration * getDoorDecoration   (int &id, DecorationPlacement &place, std::vector<std::string> &creatureTags, int closed);
        Decoration * getStairsDecoration (int &id, DecorationPlacement &place, std::vector<std::string> &creatureTags, bool down);
        Decoration * getShardDecoration (int &id, DecorationPlacement &place, std::vector<std::string> &creatureTags);
        static Room *getRoom(int dungeonLevel, char *tiles, int w, int h);
        int m_x, m_y;
        int m_stride, m_height;
        int m_roomWidth, m_roomHeight;
        int m_id;
        std::vector<CavernConnectivityPoint> m_doors;
    protected:
        virtual void renderRoom(Tile *tiles, TileFactory *tileFactory)   = 0;
        virtual void decorateRoom(Tile *tiles, TileFactory *tileFactory, std::vector<DecorationPlacement> &places) = 0;
        virtual void reserveRoom(char *tiles)  = 0;
        virtual bool validateRoom(char *tiles) = 0;
};

#endif /* end of include guard: ROOMS_SCM06W3Y */
