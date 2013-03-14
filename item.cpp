#include <cstdio>
#include <boost/random/uniform_int_distribution.hpp>
#include "item.h"
#include "level.h"


Item::Item()
	:Actor()
{
	addTag(TAG_ITEM);
}

void
Item::act()
{
}

void
Item::playerSpotted()
{
}

void
Item::handleProperty(std::string &name, TCOD_value_t &val)
{
    if(name == "hp"){
        m_hp = val.i;
    }
}

void
Item::handleTag(std::string &tag)
{
}

void
Item::finish(Level *level)
{
    m_level = level;
}
