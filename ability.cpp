#include "ability.h"
#include "actor.h"
#include "level.h"

Ability::Ability(int id):
	m_name("Nothing"),m_description(""),
	m_flavour(""),m_id(id),m_active(false)
{
}

void
Ability::deactivate(Actor *invoker, Level *level)
{
}

bool
Ability::act(Actor *invoker, Level *level)
{
    return false;
}

//Sacred Nectar

AbSacredNectar::AbSacredNectar(int id):
	Ability(id)
{
	m_name        = "Sacred Nectar";
	m_description = "You gain 5 hp";
	m_cost.push_back(ManaCost(2,COLOR_WHITE));
    m_nectar = 0;
}

bool
AbSacredNectar::invoke(Actor *invoker, Level *level)
{
    m_nectar = level->m_actorFactory.getActor("Sacred Nectar",level);
    invoker->pickUp(m_nectar);
    return false;
}

void
AbSacredNectar::deactivate(Actor *invoker, Level *level)
{
    invoker->removeFromInventory(m_nectar);
    if(m_nectar != 0){
        delete m_nectar;
    }
}

// Lightning Bolt

AbLightningBolt::AbLightningBolt(int id):
	Ability(id)
{
	m_name        = "Lightning Bolt";
	m_description = "";
	m_cost.push_back(ManaCost(1,COLOR_RED));
}

bool
AbLightningBolt::invoke(Actor *invoker, Level *level)
{
    bool done = false;
    Actor *target;
    while(!done){
        target = invoker->getTarget(TARGET_HOSTILE);
        if(target ==0){
            level->m_messages->showMessage("Are you sure that you do not want to target anyone? (y/n)", MESSAGE_WARNING);
            level->render();
            TCODConsole::flush();
            bool selected = false;
            while(!selected){
                TCOD_key_t key = TCODConsole::waitForKeypress(true);
                if(key.pressed){
                    switch(key.c) {
                        case 'y':
                        case 'Y':
                            done = true;selected = true; break;
                        case 'n':
                        case 'N':
                            selected = true; break;
                    }
                }
            }
        }
        else{
            done = true;
        }
    }
    if(target != 0){
        Actor *bolt = level->m_actorFactory.getActor("Lightning Bolt",level);
        int damage = 5 + d8(RAND);
        if(!target->takeDamage(damage, DAMAGE_FIRE, bolt)){
            level->killActor(target,bolt);
        }
        delete bolt;
    }
    return true;
}

// Small Red Mana Stream

AbSmallRedManaStream::AbSmallRedManaStream(int id):
	Ability(id)
{
	m_name        = "Krark's Mana Stream";
	m_description = "";
    m_cooldown = 0;
}

bool
AbSmallRedManaStream::invoke(Actor *invoker, Level *level)
{
    return false;
}

bool
AbSmallRedManaStream::act(Actor *invoker, Level *level)
{
    std::cout << m_cooldown << std::endl;
    if(m_cooldown == 0){
        if(invoker->hasTag(ACTOR_CREATURE) || invoker->hasTag(TAG_PLAYER)){
            static_cast<Creature *>(invoker)->regainMana(1,COLOR_RED);
            level->m_messages->showMessage("You feel a surge of red mana",MESSAGE_NOTIFICATION);
        }
        m_cooldown = 20;
    }
    m_cooldown--;
}
