#include <cstdio>
#include <boost/random/uniform_int_distribution.hpp>
#include "creature.h"
#include "level.h"


Creature::Creature()
	:Actor(),m_hd(1),m_playerSpottedCooldown(0),m_playerSpottedMemory(4),
	m_str(8),m_dex(8),m_con(8),
	m_int(8),m_wis(8),m_cha(8),
	m_bab(0),m_hitDie(6)
{
	addTag(TAG_CREATURE);
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
	std::string msg;
    if(!hasTag(TAG_PLAYER))
        msg = "The ";
	msg += m_name;
	int die = d10(RAND);
	if(die + m_bab + m_strBonus > target->m_ac){
        if(hasTag(TAG_PLAYER))
            msg += " hit the ";
        else
            msg += " hits ";
		if(!target->takeDamage(std::max(0,d3(RAND) + m_strBonus), DAMAGE_PHYSICAL, this)){
			m_level->killActor(target,this);
			showMessage = false;
		}
	}
	else{
        if(hasTag(TAG_PLAYER))
            msg += " miss the ";
        else
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

    m_hp = m_maxHp = m_hitDie;
    boost::random::uniform_int_distribution<> hpDist(0,m_hitDie);
    for(int i=1;i<m_hd;i++){
        m_hp += hpDist(RAND);
    }
    m_hp += m_conBonus*m_hd;
    m_hp = std::max(1,m_hp);
}

void
Creature::handleProperty(std::string &name, TCOD_value_t &val)
{
    if(name == "str") m_str = val.i;
    if(name == "dex") m_dex = val.i;
    if(name == "con") m_con = val.i;
    if(name == "in" ) m_int = val.i;
    if(name == "wis") m_wis = val.i;
    if(name == "cha") m_cha = val.i;
    if(name == "bab") m_bab = val.i;
    if(name == "hp"){
        m_hd = val.dice.nb_rolls;
        m_hitDie = val.dice.nb_faces;
    }
}

void
Creature::handleTag(std::string &tag)
{
}

void
Creature::finish(Level *level)
{
    m_level = level;
	calculateBonuses();
}
