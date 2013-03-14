#ifndef ITEM_OLX31Q8Q
#define ITEM_OLX31Q8Q
#include "actor.h"

enum{
    ITEM_PICKABLE,
};

class Item: public Actor
{
	public:
		Item();
		virtual void act();
		virtual void playerSpotted();
        virtual void handleProperty(std::string &name, TCOD_value_t &val);
        virtual void handleTag(std::string &tag);
        virtual void finish(Level *level);
};


#endif /* end of include guard: ITEM_OLX31Q8Q */
