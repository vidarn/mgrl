#include <cstdio>
#include <boost/random/uniform_int_distribution.hpp>
#include "creature.h"
#include "level.h"


Creature::Creature()
	:Actor(),m_hd(1),m_playerSpottedCooldown(0),m_playerSpottedMemory(4),
	m_str(8),m_dex(8),m_con(8),
	m_int(8),m_wis(8),m_cha(8),
	m_bab(0),m_hitDie(6),
    m_weapon(-1), m_shield(-1),
    m_bodyArmor(-1), m_gloves(-1),
    m_boots(-1), m_unarmedDamage(3),
    m_helmet(-1)
{
	addTag(TAG_CREATURE);
	addTag(TAG_ATTACKABLE);
    for(int i=0;i<3;i++){
        m_mana[i] = m_maxMana[i] = 1;
    }
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
Creature::quaffPotion(Actor *potion)
{
    if(potion->hasTag(ITEM_QUAFF)){
        potion->quaff(this);
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
	int die = d20(RAND);
    bool dealtDamage = true;
    int toHit = die + m_bab + m_strBonus;
	if(toHit > target->m_ac || die == 20){
        if(hasTag(TAG_PLAYER))
            msg += " hit the ";
        else
            msg += " hits ";
        int damageDie = m_unarmedDamage;
        int damageNumDice = 1;
        if(m_weapon > -1){
            Item *weapon  = static_cast<Item *>(m_inventory[m_weapon]);
            damageDie     = weapon->m_damageDie;
            damageNumDice = weapon->m_damageNumDice;
        }
        boost::random::uniform_int_distribution<> damageDist(1,damageDie);
        int damage = 0;
        for(int i=0;i<damageNumDice;i++){
            damage += damageDist(RAND);
        }
        damage += m_strBonus;
        if(damage>0){
            if(!target->takeDamage(damage, DAMAGE_PHYSICAL, this)){
                m_level->killActor(target,this);
                showMessage = false;
            }
        }
        else{
            dealtDamage = false;
        }
	}
	else{
        if(hasTag(TAG_PLAYER))
            msg += " miss the ";
        else
            msg += " misses ";
	}
	msg += target->m_name;
    if(!dealtDamage){
        if(hasTag(TAG_PLAYER))
            msg += " but deal no damage";
        else
            msg += " but deals no damage";
    }
	if(showMessage){
		m_level->m_messages->showMessage(msg, MESSAGE_ACTION);
	}
}

void
Creature::removeFromInventory(Actor *item)
{
    for(int i=0;i<m_inventory.size();i++){
        Actor *invItem = m_inventory[i];
        if(invItem == item){
            m_inventory.erase(m_inventory.begin()+i);
            if(m_weapon == i)m_weapon = -1;
            if(m_weapon > i)m_weapon--;
            if(m_shield == i)m_shield = -1;
            if(m_shield > i)m_shield--;
            if(m_helmet == i)m_helmet = -1;
            if(m_helmet > i)m_helmet--;
            if(m_bodyArmor == i)m_bodyArmor = -1;
            if(m_bodyArmor > i)m_bodyArmor--;
            if(m_gloves == i)m_gloves = -1;
            if(m_gloves > i)m_gloves--;
            if(m_boots == i)m_boots = -1;
            if(m_boots > i)m_boots--;
            i--;
        }
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

void
Creature::calculateHp()
{
    m_hp = m_hitDie;
    boost::random::uniform_int_distribution<> hpDist(0,m_hitDie);
    for(int i=1;i<m_hd;i++){
        m_hp += hpDist(RAND);
    }
    m_hp += m_conBonus*m_hd;
    m_hp = std::max(1,m_hp);
    m_maxHp = m_hp;
}

void
Creature::calculateAc()
{
    m_ac = 10;
    m_ac += m_dexBonus;
    if(m_shield>-1)
        m_ac += ((Item *)m_inventory[m_shield])->m_acBonus;
    if(m_helmet>-1)
        m_ac += ((Item *)m_inventory[m_helmet])->m_acBonus;
    if(m_bodyArmor>-1)
        m_ac += ((Item *)m_inventory[m_bodyArmor])->m_acBonus;
    if(m_gloves>-1)
        m_ac += ((Item *)m_inventory[m_gloves])->m_acBonus;
    if(m_boots>-1)
        m_ac += ((Item *)m_inventory[m_boots])->m_acBonus;
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
    if(name == "weapon"){
        m_weapon = m_inventoryStrings.size();
        m_inventoryStrings.push_back("Short Sword");
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
    for(int i=0;i<m_inventoryStrings.size();i++){
        Actor *actor = m_level->m_actorFactory.getActor(m_inventoryStrings[i],level);
        if(actor != 0){
            m_inventory.push_back(actor);
        }
        std::cout << actor->m_name << std::endl;
    }
    calculateHp();
    calculateAc();
}

void
Creature::regainMana(int amount, int color)
{
    switch(color){
			case COLOR_RED:
                m_mana[0] += amount;
                break;
			case COLOR_BLUE:
                m_mana[1] += amount;
                break;
			case COLOR_WHITE:
                m_mana[2] += amount;
                break;
    }
    for(int i=0;i<3;i++){
        std::cout << m_mana[i] << amount << "Mana\n";
        m_mana[i] = std::min(m_maxMana[i], std::max(0, m_mana[i]));
    }
    std::cout << "Regain\n";
}
