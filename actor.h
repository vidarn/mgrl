#ifndef ACTOR_U9AMWJYS
#define ACTOR_U9AMWJYS
#include <libtcod/libtcod.hpp>

class Actor
{
	public:
		Actor();
		Actor(int x, int y);
		void render();
		int m_x, m_y;
		char m_glyph;
	private:
};

#endif /* end of include guard: ACTOR_U9AMWJYS */
