#include "actor.h"
#include "level.h"

Actor::Actor(Level *level):
	m_x(0),m_y(0),m_glyph('A'),m_hp(1),m_maxHp(1), m_level(level)
{
}

void
Actor::walkTowardsPlayer()
{
	m_level->walkTowardsPlayer(&m_x,&m_y);
}

void
Actor::render()
{
	TCODConsole::root->putChar(m_x,m_y,m_glyph);
}
