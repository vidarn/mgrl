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
		Dungeon(int width, int height, TileFactory *tileFactory, std::vector<Actor*> *actors);
		void render();
		void generate(Level *level);
        void drawLine(int y0, int y1, int x0, int x1, int width, char *tiles, void (*tileCallback)(char *));
        void drawPath(int y0, int y1, int x0, int x1, int width, char *tiles, void (*tileCallback)(char *));
		bool isWalkable(int x, int y);
		bool isTransparent(int x, int y);
		char getGlyph(int x, int y);
		void setVisible(int x, int y, bool state);
		void setDiscovered(int x, int y, bool state);
		int m_width, m_height;
	private:
        void generateCavern(int miny, int maxy, int minx, int maxx);
        void connectCaverns(int miny, int maxy, int minx, int maxx,
                char *tiles, std::vector<CavernConnectivityPoint> &points);
        void cellularAutomata(int height, int width, char **tiles,
                char **tmpTiles, char (*ruleCallback)(int));
        void roomsReserve(char *tiles);
        void roomsRender(char *tiles);
		Tile *m_tiles;
		TileFactory *m_tileFactory;
		std::vector<Actor*> *m_actors;
        std::vector<Room *> m_rooms;
};
#endif /* end of include guard: DUNGEON_CM9N55L0 */
