#ifndef ACTOR_U9AMWJYS
#define ACTOR_U9AMWJYS
#include <libtcod/libtcod.hpp>
class Level;

class Actor
{
	public:
		Actor(Level *level);
		void walkTowardsPlayer();
		void walkRandomly();
		void render();
		virtual void walk(int dx, int dy);
		virtual void playerSpotted();
		virtual void act();
		int m_x, m_y;
		char m_glyph;
		int m_hp, m_maxHp;
		int m_dx, m_dy;
		const char *m_name;
	protected:
		Level *m_level;
};

#endif /* end of include guard: ACTOR_U9AMWJYS */
