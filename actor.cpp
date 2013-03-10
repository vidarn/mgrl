#include "actor.h"

Actor::Actor():
	m_x(0),m_y(0),m_glyph('A')
{
}

Actor::Actor(int x, int y):
	m_x(x),m_y(x),m_glyph('A')
{
}

void
Actor::render()
{
	TCODConsole::root->putChar(m_x,m_y,m_glyph);
}
