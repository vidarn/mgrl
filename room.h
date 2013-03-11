#ifndef ROOMS_SCM06W3Y
#define ROOMS_SCM06W3Y
#include <vector>
#include "common.h"

#define ROOM_DEF(NAME,...) class NAME: public Room{public: NAME(char *tiles, int w, int h, ##__VA_ARGS__); virtual void renderRoom(char *tiles); virtual void reserveRoom(char *tiles); virtual bool validateRoom(char *tiles);
#define ROOM_CONS(NAME,...) NAME::NAME(char *tiles, int w, int h, ##__VA_ARGS__):Room(tiles,w,h)

enum{
    CON_CAV,
    CON_DUN,
};

struct
CavernConnectivityPoint
{
    int x, y;
    int tag;
    int type;
    int roomID;
};

class Room
{
    public:
        Room(char *tiles, int w, int h);
        void render(char *tiles);
        void reserve(char *tiles);
        bool validate(char *tiles, std::vector<CavernConnectivityPoint> &points, int id);
        bool validateTile(char tile);
        static Room *getRoom(int dungeonLevel, char *tiles, int w, int h);
        int m_x, m_y;
        int m_stride;
        int m_id;
        std::vector<CavernConnectivityPoint> m_doors;
    protected:
        virtual void renderRoom(char *tiles)   = 0;
        virtual void reserveRoom(char *tiles)  = 0;
        virtual bool validateRoom(char *tiles) = 0;
};


ROOM_DEF(SquareRoom, int size)
    int m_w, m_h;
};

#endif /* end of include guard: ROOMS_SCM06W3Y */
