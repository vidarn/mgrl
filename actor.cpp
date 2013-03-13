#include <boost/random/uniform_int_distribution.hpp>
#include "actor.h"
#include "level.h"

Actor::Actor(Level *level):
	m_x(0),m_y(0),m_glyph('A'),m_hp(1),m_maxHp(1), m_level(level), m_dx(0),
	m_dy(0), m_name("something"), m_ac(0), m_color(TCODColor::white)
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

bool
Actor::walk(int dx,int dy)
{
	if(m_level->m_dungeon->isWalkable(m_x+dx,m_y+dy)){
		m_x += dx;
		m_y += dy;
		return true;
	}
	return false;
}

void
Actor::render()
{
	TCODConsole::root->setDefaultForeground(m_color);
	TCODConsole::root->putChar(m_x,m_y,m_glyph);
}

void
Actor::addTag(int tag)
{
	bool add = true;
	for(int i=0;i<m_tags.size();i++){
		if(m_tags[i] == tag)
			add = false;
	}
	if(add){
		m_tags.push_back(tag);
	}
}

bool
Actor::hasTag(int tag)
{
	for(int i=0;i<m_tags.size();i++){
		if(m_tags[i] == tag)
			return true;
	}
	return false;
}

void
Actor::playerSpotted()
{
}

void
Actor::act()
{
}

bool
Actor::takeDamage(int amount, int type, Actor *source)
{
	m_hp -= amount;
	return m_hp > 0;
}

void
Actor::die(Actor *source)
{
	std::string msg = m_name;
	msg += " was destroyed by ";
	msg += source->m_name;
	m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
}

bool
Actor::fortSave(int dc)
{
	return false;
}

bool
Actor::willSave(int dc)
{
	return false;
}

bool
Actor::reflSave(int dc)
{
	return false;
}
