#ifndef CREATURE_EV3PTLWG
#define CREATURE_EV3PTLWG
#include "actor.h"

class Creature: public Actor
{
	public:
		Creature();
		virtual void act();
		virtual void playerSpotted();
		void attack(Actor *target);
		virtual void quaffPotion(Actor *potion);
        virtual void removeFromInventory(Actor *item);
		void calculateBonuses();
		void calculateHp();
		void calculateAc();
        virtual void handleProperty(std::string &name, TCOD_value_t &val);
        virtual void handleTag(std::string &tag);
        virtual void finish(Level *level);
		int m_hd;
		int m_str;int m_dex;int m_con;
		int m_int;int m_wis;int m_cha;
		int m_strBonus;int m_dexBonus;int m_conBonus;
		int m_intBonus;int m_wisBonus;int m_chaBonus;
		int m_bab;
		int m_hitDie;
		int m_playerSpottedCooldown;
		int m_playerSpottedMemory;
        int m_weapon;
        int m_shield;
        int m_helmet;
        int m_bodyArmor;
        int m_gloves;
        int m_boots;
        int m_unarmedDamage;
};

#endif /* end of include guard: CREATURE_EV3PTLWG */
