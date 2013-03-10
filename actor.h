#ifndef ACTOR_U9AMWJYS
#define ACTOR_U9AMWJYS
#include <libtcod/libtcod.hpp>
class Level;

class Actor
{
	public:
		Actor(Level *level);
		void walkTowardsPlayer();
		void render();
		int m_x, m_y;
		char m_glyph;
		int m_hp, m_maxHp;
	protected:
		Level *m_level;
};

#endif /* end of include guard: ACTOR_U9AMWJYS */
