#ifndef DUNGEON_CM9N55L0
#define DUNGEON_CM9N55L0
#include <libtcod/libtcod.hpp>
#include "common.h"
#include "tile.h"
#include "creature.h"
#include "room.h"

class Level;

class Dungeon{
	public:
		Dungeon(int width, int height, TileFactory *tileFactory);
		void render();
		void generate(Level *level);
        void drawLine(int y0, int y1, int x0, int x1, int width, char *tiles, void (*tileCallback)(char *));
        void drawPath(int y0, int y1, int x0, int x1, int width, char *tiles, void (*tileCallback)(char *));
		bool isWalkable(int x, int y);
		bool isTransparent(int x, int y);
		char getGlyph(int x, int y);
		void setVisible(int x, int y, bool state);
		bool isVisible(int x, int y);
		void setDiscovered(int x, int y, bool state);
		void clearHilight();
		void setHilight(int x, int y, char val);
		bool isHilighted(int x, int y, char val);
		int m_width, m_height;
		Tile *m_tiles;
	private:
        void generateCavern(int miny, int maxy, int minx, int maxx, Level *level);
        void connectCaverns(int miny, int maxy, int minx, int maxx,
                char *tiles, std::vector<CavernConnectivityPoint> &points);
        void cellularAutomata(int height, int width, char **tiles,
                char **tmpTiles, char (*ruleCallback)(int));
        void roomsReserve(char *tiles);
        void roomsRender(Tile *tiles, TileFactory *tileFactory);
        void roomsDecorate(char *charTiles, Tile *tiles, TileFactory *tileFactory, Level *level);
		TileFactory *m_tileFactory;
        std::vector<Room *> m_rooms;
		char *m_hilight;
};
#endif /* end of include guard: DUNGEON_CM9N55L0 */
