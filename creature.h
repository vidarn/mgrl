#ifndef CREATURE_EV3PTLWG
#define CREATURE_EV3PTLWG
#include "actor.h"

class Creature: public Actor
{
	public:
		Creature(Level *level);
		int m_hd;
		int m_hitDie;
};

#endif /* end of include guard: CREATURE_EV3PTLWG */
