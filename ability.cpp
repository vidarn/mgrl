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
AbLightningBolt::invoke(Actor *invoke, Level *level)
{
    return true;
}
