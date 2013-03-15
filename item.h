#ifndef ITEM_OLX31Q8Q
#define ITEM_OLX31Q8Q
#include "actor.h"

class Item: public Actor
{
	public:
		Item();
		~Item();
		virtual void act();
		virtual void playerSpotted();
        virtual void handleProperty(std::string &name, TCOD_value_t &val);
        virtual void handleTag(std::string &tag);
        virtual void finish(Level *level);
        virtual void open(Actor *opener);
        std::vector<std::string> m_inventoryStrings;
};


#endif /* end of include guard: ITEM_OLX31Q8Q */
