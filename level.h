#ifndef LEVEL_WF504OFN
#define LEVEL_WF504OFN
#include "dungeon.h"
#include "player.h"

class Level
{
	public:
		Level(int dungeonWidth, int dungeonHeight);
		void render();
		void generate();
		void update();
		void walkTowardsPlayer(int *creatureX, int *creatureY);
		void updatePathFinder(int playerX, int playerY);
		void computeFov(int playerX, int playerY);
		Dungeon *m_dungeon;
		TileFactory m_tileFactory;
		Player *m_player;
		std::vector<Actor*> m_actors;
	private:
		TCODMap *m_fovMap;
		TCODDijkstra *m_pathFinder;
};

#endif /* end of include guard: LEVEL_WF504OFN */
