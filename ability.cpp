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
	m_description = "Conjures a holy elixir that replenishes a small amount of health";
	m_cost.push_back(ManaCost(2,COLOR_WHITE));
    m_nectar = 0;
    m_color = COLOR_WHITE;
}

bool
AbSacredNectar::invoke(Actor *invoker, Level *level, bool &cancelled)
{
    m_nectar = level->m_actorFactory.getActor("Sacred Nectar",level);
    m_nectar->addTag(ITEM_DONT_DROP);
    invoker->pickUp(m_nectar);
    cancelled = false;
    return false;
}

void
AbSacredNectar::deactivate(Actor *invoker, Level *level)
{
    invoker->removeFromInventory(m_nectar);
    if(m_nectar != 0){
        delete m_nectar;
    }
    level->m_messages->showMessage("The Sacred Nectar disapears from your inventory", MESSAGE_NOTIFICATION);
}

// Lightning Bolt

AbLightningBolt::AbLightningBolt(int id):
	Ability(id)
{
	m_name        = "Lightning Bolt";
	m_description = "Conjures a bolt of lightning that strikes a single target";
	m_cost.push_back(ManaCost(1,COLOR_RED));
    m_color = COLOR_RED;
}

bool
AbLightningBolt::invoke(Actor *invoker, Level *level, bool &cancelled)
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
    if(target == 0){
        cancelled = true;
        return false;
    }
    else{
        std::string msg = "A spark of lightning shoots from your fingers and hits the ";
        msg += target->m_name;
        level->m_messages->showMessage(msg, MESSAGE_WARNING);
        int damage = 5 + d8(RAND);
        if(!target->takeDamage(damage, DAMAGE_FIRE, invoker)){
            level->killActor(target,invoker);
        }
    }
    cancelled = false;
    return true;
}

// Blink

AbBlink::AbBlink(int id):
	Ability(id)
{
	m_name        = "Risky Displacement";
	m_description = "Teleports the caster to a random space within sight";
	m_cost.push_back(ManaCost(1,COLOR_BLUE));
    m_color = COLOR_BLUE;
}

bool
AbBlink::invoke(Actor *invoker, Level *level, bool &cancelled)
{
    std::vector<int> tags;
    std::vector<int> xList;
    std::vector<int> yList;
    for(int y=0;y<DUNGEON_WIN_H;y++){
        for(int x=0;x<DUNGEON_WIN_W;x++){
            if(level->m_dungeon->isVisible(x,y) && level->isWalkable(x,y)){
                xList.push_back(x);
                yList.push_back(y);
            }
        }
    }
    if(xList.size() == 0){
        level->m_messages->showMessage("No free spaces to blink to",MESSAGE_NOTIFICATION);
        return false;
    }
    else{
        boost::random::uniform_int_distribution<> tileDist(0,xList.size());
        int tile = tileDist(RAND);
        if(invoker->m_x == xList[tile] && invoker->m_y == yList[tile])
            level->m_messages->showMessage("Your surroundings seem exactly the same",MESSAGE_FLAVOUR);
        else
            level->m_messages->showMessage("Your surroundings seem different",MESSAGE_FLAVOUR);
        invoker->m_x = xList[tile];
        invoker->m_y = yList[tile];
        return true;
    }
}

// Mana Stream

AbSmallManaStream::AbSmallManaStream(int id, int color):
	Ability(id)
{
    m_color = color;
    m_name = "Small Mana Stream";
    switch(m_color){
        case COLOR_RED:
            m_colorName   = "red";
            break;
        case COLOR_BLUE:
            m_colorName   = "blue";
            break;
        case COLOR_WHITE:
            m_colorName   = "white";
            break;
    }
    m_cooldown = 0;
    m_description = "Continously regain a small amount of ";
    m_description += m_colorName;
    m_description += " Mana";
}

bool
AbSmallManaStream::invoke(Actor *invoker, Level *level, bool &cancelled)
{
    cancelled = false;
    return false;
}

bool
AbSmallManaStream::act(Actor *invoker, Level *level)
{
    if(m_cooldown == 0){
        if(invoker->hasTag(ACTOR_CREATURE) || invoker->hasTag(TAG_PLAYER)){
            static_cast<Creature *>(invoker)->regainMana(1,m_color);
            std::string msg = "You feel a small wave of ";
            msg += m_colorName;
            msg += " Mana";
            level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
        }
        m_cooldown = 20;
    }
    m_cooldown--;
}
