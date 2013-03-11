#include <cstdio>
#include <boost/random/uniform_int_distribution.hpp>
#include "creature.h"
#include "level.h"


Creature::Creature(Level *level)
	:Actor(level),m_hd(1),m_playerSpottedCooldown(0),m_playerSpottedMemory(4),
	m_str(8),m_dex(8),m_con(8),
	m_int(8),m_wis(8),m_cha(8),
	m_bab(0),m_hitDie(6)
{
	addTag(TAG_CREATURE);
	calculateBonuses();
}

void
Creature::act()
{
	if(m_playerSpottedCooldown>0){
		if(abs(m_x - m_level->m_player->m_x) <= 1 && abs(m_y - m_level->m_player->m_y) <= 1){
			attack(m_level->m_player);
		}
		else{
			walkTowardsPlayer();
		}
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

void
Creature::attack(Actor *target)
{
	bool showMessage = true;
	std::string msg = m_name;
	int die = d10(RAND);
	if(die + m_bab + m_strBonus > target->m_ac){
		msg += " hits ";
		if(!target->takeDamage(d6(RAND) + m_strBonus, DAMAGE_PHYSICAL, this)){
			m_level->killActor(target,this);
			showMessage = false;
		}
	}
	else{
		msg += " misses ";
	}
	msg += target->m_name;
	if(showMessage){
		m_level->m_messages->showMessage(msg, MESSAGE_ACTION);
	}
}

void
Creature::calculateBonuses()
{
	m_strBonus = m_str/2 - 5;
	m_dexBonus = m_dex/2 - 5;
	m_conBonus = m_con/2 - 5;
	m_intBonus = m_int/2 - 5;
	m_wisBonus = m_wis/2 - 5;
	m_chaBonus = m_cha/2 - 5;
}
