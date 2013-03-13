#ifndef LEVEL_WF504OFN
#define LEVEL_WF504OFN
#include "dungeon.h"
#include "player.h"
#include "messages.h"

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
		void killActor(Actor *victim, Actor *killer);
		std::vector<Actor *> getVisibleActors();
		Creature *getCreature(int x, int y);
		Dungeon *m_dungeon;
		TileFactory m_tileFactory;
		ActorFactory m_actorFactory;
		Player *m_player;
		MessageHandler *m_messages;
		std::vector<Actor *> m_actors;
		bool m_playerAlive;
	private:
		TCODMap *m_fovMap, *m_dungeonFovMap;
		TCODDijkstra *m_pathFinder;
};

#endif /* end of include guard: LEVEL_WF504OFN */
