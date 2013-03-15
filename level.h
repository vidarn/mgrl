#ifndef LEVEL_WF504OFN
#define LEVEL_WF504OFN
#include "callback_overlay.h"
#include "dungeon.h"
#include "player.h"
#include "item.h"
#include "messages.h"

class Level
{
	public:
		Level(int dungeonWidth, int dungeonHeight, int statusWinWidth, int statusWinHeight,
                int messagesWinWidth, int messagesWinHeight);
		void render();
		void generate();
		void update();
		void walkTowardsPlayer(int *creatureX, int *creatureY);
		void updatePathFinder(int playerX, int playerY);
		void computeFov(int playerX, int playerY);
		void killActor(Actor *victim, Actor *killer);
		std::vector<Actor *> getVisibleActors(std::vector<int> tags);
        std::vector<Actor *> getActors(int x, int y, std::vector<int> tags);
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
        Overlay *m_statusWin;
        Overlay *m_messagesWin;
};

#endif /* end of include guard: LEVEL_WF504OFN */
