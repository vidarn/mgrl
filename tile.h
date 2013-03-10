#ifndef TILE_IWRFWCCC
#define TILE_IWRFWCCC
#include <ncurses.h>

enum TILE_TYPES
{
	T_VOID,
	T_STONE_FLOOR,
	T_ROCK_WALL,
	T_WATER_STREAM,
};

class Tile{
	public:
		Tile();
		void setType(int type);
		int m_type;
		char m_glyph;
		char m_color;
		bool m_walkable;
		bool m_transparent;
		bool m_flyable;
		bool m_discovered;
		bool m_visible;
};

#endif /* end of include guard: TILE_IWRFWCCC */
