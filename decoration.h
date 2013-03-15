#ifndef DECORATION_J7C4JLSU
#define DECORATION_J7C4JLSU
#include "common.h"
#include "tile.h"

#define DEC_DEF(NAME,...) class NAME: public Decoration{public: NAME(int x, int y, int w, int h, ##__VA_ARGS__);private: virtual bool validateDecoration(char *tiles);virtual bool reserveDecoration(char *tiles);virtual void renderDecoration(Tile *tiles, TileFactory *tileFactory, Level *level);
#define DEC_CONS(NAME,...) NAME::NAME(int x, int y, int w, int h, ##__VA_ARGS__):Decoration(x,y,w,h)
#define DEC_VAL(NAME) bool NAME::validateDecoration(char *tiles)
#define DEC_RES(NAME) bool NAME::reserveDecoration(char *tiles)
#define DEC_REN(NAME) void NAME::renderDecoration(Tile *tiles, TileFactory *tileFactory, Level *level)

class Level;

class Decoration
{
    public:
        Decoration(int x, int y, int w, int h);
        bool validate(char *tiles);
        bool validateTile(int x, int y, char *tiles);
        void reserve(char *tiles);
        void reserveTile(int x, int y, char *tiles);
        void render(Tile *tiles, TileFactory *tileFactory, Level *level);
        void setTile(int x, int y, std::string tile, Tile *tiles, TileFactory *tileFactory);
        Tile *getTile(int x, int y, Tile *tiles);
        int m_x, m_y;
        int m_w, m_h;
    protected:
        virtual bool validateDecoration(char *tiles) = 0;
        virtual bool reserveDecoration(char *tiles) = 0;
        virtual void renderDecoration(Tile *tiles, TileFactory *tileFactory, Level *level) = 0;
};


#endif /* end of include guard: DECORATION_J7C4JLSU */
