#ifndef DUNGEON_CM9N55L0
#define DUNGEON_CM9N55L0
#include <libtcod/libtcod.hpp>
#include "common.h"
#include "tile.h"

class Dungeon{
	public:
		Dungeon(int width, int height, TileFactory *tileFactory);
		void render();
		void generate();
		void computeFov(int playerX, int playerY);
        void drawLine(int y0, int y1, int x0, int x1, int width, char *tiles, void (*tileCallback)(char *));
		bool isWalkable(int x, int y);
	private:
        void generateCavern(int miny, int maxy, int minx, int maxx);
        void connectCaverns(int miny, int maxy, int minx, int maxx, char *tiles);
        void cellularAutomata(int height, int width, char **tiles,
                char **tmpTiles, char (*ruleCallback)(int));
		Tile *m_tiles;
		TileFactory *m_tileFactory;
		int m_width, m_height;
		TCODMap *m_fovMap;
};

struct
CavernConnectivityPoint
{
    int x, y;
    int tag;
};

#endif /* end of include guard: DUNGEON_CM9N55L0 */
