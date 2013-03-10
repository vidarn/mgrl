#ifndef CREATURE_EV3PTLWG
#define CREATURE_EV3PTLWG
#include "actor.h"

class Creature: public Actor
{
	public:
		Creature(Level *level);
		virtual void act();
		virtual void playerSpotted();
		int m_hd;
		int m_hitDie;
		int m_playerSpottedCooldown;
		int m_playerSpottedMemory;
};

#endif /* end of include guard: CREATURE_EV3PTLWG */
