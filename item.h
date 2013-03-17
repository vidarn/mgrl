#ifndef ITEM_OLX31Q8Q
#define ITEM_OLX31Q8Q
#include "actor.h"

class Item: public Actor
{
	public:
		Item();
		~Item();
		virtual void act(float time=1.0f);
		virtual void playerSpotted();
        virtual void handleProperty(std::string &name, TCOD_value_t &val);
        virtual void handleTag(std::string &tag);
        virtual void finish(Level *level);
        virtual void open(Actor *opener);
        virtual void quaff(Actor *drinker);
        int m_acBonus;
        int m_armorCheckPenalty;
        int m_enchantment;
        int m_damageDie;
        int m_damageNumDice;
};


#endif /* end of include guard: ITEM_OLX31Q8Q */
