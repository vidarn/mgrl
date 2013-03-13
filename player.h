#ifndef PLAYER_R44I7II0
#define PLAYER_R44I7II0
#include "creature.h"
#include "ability.h"

class Player: public Creature
{
	public:
		Player();
		bool walk(int dx, int dy);
		void startRun(int dx, int dy);
		void run();
		virtual void die(Actor *source);
		virtual void finish(Level *level);
		bool m_running;
		std::vector<Ability *> m_abilities;
	private:
		int m_runX, m_runY;
		char m_glyphLeft, m_glyphRight;
};

#endif /* end of include guard: PLAYER_R44I7II0 */
