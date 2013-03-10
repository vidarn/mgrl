#include <cstdio>
#include "creature.h"
#include "level.h"


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
	if(m_playerSpottedCooldown ==0){
		char buffer[512];
		sprintf(buffer,"A %s comes into view", m_name);
		m_level->m_messages->showMessage(buffer, MESSAGE_NOTIFICATION);
	}
	m_playerSpottedCooldown = m_playerSpottedMemory;
}
