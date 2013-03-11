#include "ability.h"

Ability::Ability(int id):
	m_name("Nothing"),m_description(""),
	m_flavour(""),m_id(id)
{
}

AbSacredNectar::AbSacredNectar(int id):
	Ability(id)
{
	m_name        = "Sacred Nectar";
	m_description = "You gain 5 hp";
	m_cost.push_back(ManaCost(1,COLOR_WHITE));
	m_cost.push_back(ManaCost(1,COLOR_LESS));
}

AbLightningBolt::AbLightningBolt(int id):
	Ability(id)
{
	m_name        = "Lightning Bolt";
	m_description = "";
	m_cost.push_back(ManaCost(1,COLOR_RED));
}
