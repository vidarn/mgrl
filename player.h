#ifndef PLAYER_R44I7II0
#define PLAYER_R44I7II0
#include "actor.h"
#include "dungeon.h"

class Player: public Actor
{
	public:
		Player(Dungeon *dungeon);
		void move(int dx, int dy);
	private:
		Dungeon *m_dungeon;
};

#endif /* end of include guard: PLAYER_R44I7II0 */
