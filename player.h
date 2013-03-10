#ifndef PLAYER_R44I7II0
#define PLAYER_R44I7II0
#include "actor.h"
#include "dungeon.h"

class Player: public Actor
{
	public:
		Player(Dungeon *dungeon);
		bool move(int dx, int dy);
		void startRun(int dx, int dy);
		void run();
		bool m_running;
	private:
		Dungeon *m_dungeon;
		int m_runX, m_runY;
		char m_glyphLeft, m_glyphRight;
};

#endif /* end of include guard: PLAYER_R44I7II0 */
