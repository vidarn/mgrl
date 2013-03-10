#ifndef DUNGEON_CM9N55L0
#define DUNGEON_CM9N55L0
#include <libtcod/libtcod.hpp>
#include "common.h"
#include "tile.h"

class Dungeon{
	public:
		Dungeon(int width, int height);
		void render();
		void generate();
        void drawLine(int y0, int y1, int x0, int x1, int width, char *tiles, void (*tileCallback)(char *));
	private:
        void generateCavern(int miny, int maxy, int minx, int maxx);
        void connectCaverns(int miny, int maxy, int minx, int maxx, char *tiles);
        void cellularAutomata(int height, int width, char **tiles,
                char **tmpTiles, char (*ruleCallback)(int));
		Tile *m_tiles;
		int m_width, m_height;
};

struct
CavernConnectivityPoint
{
    int x, y;
    int tag;
};

#endif /* end of include guard: DUNGEON_CM9N55L0 */
