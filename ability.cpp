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

//Sacred Nectar

AbSacredNectar::AbSacredNectar(int id):
	Ability(id)
{
	m_name        = "Sacred Nectar";
	m_description = "You gain 5 hp";
	m_cost.push_back(ManaCost(1,COLOR_WHITE));
	m_cost.push_back(ManaCost(1,COLOR_LESS));
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
            level->m_messages->showMessage("Are you sure you do not want to target anyone? (y/n)", MESSAGE_WARNING);
            level->render();
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
