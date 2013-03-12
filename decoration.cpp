#include "decoration.h"


Decoration::Decoration(int x, int y, int w, int h):
    m_w(w),m_h(h), m_x(x), m_y(y)
{
}

bool
Decoration::validate(char *tiles)
{
    if(!validateDecoration(tiles)){
        return false;
    }
    return true;
}

bool
Decoration::validateTile(int x, int y, char *tiles)
{
    x+=m_x;y+=m_y;
    if(x>0 && x<m_w-1 && y>0 && y<m_h-1)
        return tiles[x+y*m_w] == 0;
    else
        return false;
}

void
Decoration::reserve(char *tiles)
{
    reserveDecoration(tiles);
}

void
Decoration::reserveTile(int x, int y, char *tiles)
{
    x+=m_x;y+=m_y;
    if(x>0 && x<m_w-1 && y>0 && y<m_h-1)
        tiles[x+y*m_w] = 125;
}

void
Decoration::render(Tile *tiles, TileFactory *tileFactory)
{
    renderDecoration(tiles, tileFactory);
}

void
Decoration::setTile(int x, int y, std::string tile, Tile *tiles, TileFactory *tileFactory)
{
    x+=m_x;y+=m_y;
    if(x>0 && x<m_w-1 && y>0 && y<m_h-1)
        tiles[x+y*m_w] = tileFactory->getTile(tile);
}

Tile *
Decoration::getTile(int x, int y, Tile *tiles)
{
    x+=m_x;y+=m_y;
    if(x>0 && x<m_w-1 && y>0 && y<m_h-1)
        return &tiles[x+y*m_w];
    return 0;
}

