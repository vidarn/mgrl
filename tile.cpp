#include "tile.h"

Tile::Tile()
{
	setType(T_VOID);
}

void
Tile::setType(int type)
{
	char g = 'X';
	char c = COLOR_WHITE;
	bool w = false;
	switch(type){
		case T_VOID:
			g = ' ';
			break;
		case T_STONE_FLOOR:
			g = '.';
			w = true;
			break;
		case T_ROCK_WALL:
			g = '#';
			break;
		case T_WATER_STREAM:
			g = '~';
			c = COLOR_BLUE;
			w = true;
			break;
	}
	m_glyph = g;
	m_color = c;
	m_walkable = w;
}
