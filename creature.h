#ifndef CREATURE_EV3PTLWG
#define CREATURE_EV3PTLWG
#include "actor.h"

class Creature: public Actor
{
	public:
		Creature(Level *level);
		virtual void act();
		virtual void playerSpotted();
		void attack(Actor *target);
		void calculateBonuses();
		int m_hd;
		int m_str;int m_dex;int m_con;
		int m_int;int m_wis;int m_cha;
		int m_strBonus;int m_dexBonus;int m_conBonus;
		int m_intBonus;int m_wisBonus;int m_chaBonus;
		int m_bab;
		int m_hitDie;
		int m_playerSpottedCooldown;
		int m_playerSpottedMemory;
};

#endif /* end of include guard: CREATURE_EV3PTLWG */
