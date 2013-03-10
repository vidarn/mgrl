#include <boost/random/uniform_int_distribution.hpp>
#include "actor.h"
#include "level.h"

Actor::Actor(Level *level):
	m_x(0),m_y(0),m_glyph('A'),m_hp(1),m_maxHp(1), m_level(level), m_dx(0), m_dy(0)
{
}

void
Actor::walkTowardsPlayer()
{
	m_level->walkTowardsPlayer(&m_x,&m_y);
}

void
Actor::walkRandomly()
{
	boost::random::uniform_int_distribution<> turnDist(0,3);
	boost::random::uniform_int_distribution<> dist(-1,1);
	int turn = turnDist(RAND);
	if(turn == 0){
		m_dx = dist(RAND);
		m_dy = dist(RAND);
	}
	int a =0;
	while(!m_level->m_dungeon->isWalkable(m_x+m_dx,m_y+m_dy) && a < 5){
		m_dx = dist(RAND);
		m_dy = dist(RAND);
		a++;
	}
	walk(m_dx,m_dy);
}

void
Actor::walk(int dx,int dy)
{
	if(m_level->m_dungeon->isWalkable(m_x+dx,m_y+dy)){
		m_x += dx;
		m_y += dy;
	}
}

void
Actor::render()
{
	TCODConsole::root->putChar(m_x,m_y,m_glyph);
}

void
Actor::playerSpotted()
{
}

void
Actor::act()
{
}
