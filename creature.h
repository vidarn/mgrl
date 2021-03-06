#ifndef CREATURE_EV3PTLWG
#define CREATURE_EV3PTLWG
#include "actor.h"

class Creature: public Actor
{
	public:
		Creature();
		virtual void act(float time=1.0f);
		virtual void playerSpotted();
		void attack(Actor *target, int type);
        void die(Actor *source);
		virtual void quaffPotion(Actor *potion);
        virtual void removeFromInventory(Actor *item);
		void calculateBonuses();
		void calculateHp();
		void calculateAc();
        virtual void handleProperty(std::string &name, TCOD_value_t &val);
        virtual void handleTag(std::string &tag);
        virtual void finish(Level *level);
        virtual void regainMana(int amount, int color);
		int m_playerSpottedCooldown;
		int m_playerSpottedMemory;
		int m_str;int m_dex;int m_con;
		int m_int;int m_wis;int m_cha;
		int m_strBonus;int m_dexBonus;int m_conBonus;
		int m_intBonus;int m_wisBonus;int m_chaBonus;
		int m_bab;
		int m_hitDie;
        int m_weapon;
        int m_shield;
        int m_bodyArmor;
        int m_gloves;
        int m_boots;
        int m_unarmedDamage;
        int m_helmet;
        int m_quiver;
		int m_expValue;
        int m_regainLifeCooldown;
        int m_mana[3]; int m_maxMana[3]; int m_lockedMana[3];
};

#endif /* end of include guard: CREATURE_EV3PTLWG */
