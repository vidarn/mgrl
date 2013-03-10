#include "creature.h"


Creature::Creature(Level *level)
	:Actor(level),m_hd(1),m_playerSpottedCooldown(0),m_playerSpottedMemory(4)
{
}

void
Creature::act()
{
	if(m_playerSpottedCooldown>0){
		walkTowardsPlayer();
		m_playerSpottedCooldown--;
	}
	else{
		walkRandomly();
	}
}

void
Creature::playerSpotted()
{
	m_playerSpottedCooldown = m_playerSpottedMemory;
}
