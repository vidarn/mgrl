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
	m_description = "Conjures a holy elixir that replenishes your health";
	m_cost.push_back(ManaCost(3,COLOR_WHITE));
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
        int damage = 5;
        for(int i=0; i<1+invoker->m_hd/2;i++){
            damage += d8(RAND);
        }
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
    for(int y=1;y<DUNGEON_WIN_H-1;y++){
        for(int x=1;x<DUNGEON_WIN_W-1;x++){
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

// Instant Tunnel

AbInstantTunnel::AbInstantTunnel(int id):
	Ability(id)
{
	m_name        = "Instant Tunnel";
	m_description = "Blasts away rock to form a crude tunnel";
	m_cost.push_back(ManaCost(2,COLOR_RED));
    m_color = COLOR_RED;
}

bool
AbInstantTunnel::invoke(Actor *invoker, Level *level, bool &cancelled)
{
    bool blasted = false;
    int w = level->m_dungeon->m_width;
    int h = level->m_dungeon->m_height;
    int x,y;
    bool cont = level->m_player->getDirection(x,y,true);
    if(!cont){
        cancelled = true;
        return false;
    }
    else{
        int tmpX=invoker->m_x;
        int tmpY=invoker->m_y;
        for(int i=0;i<5+invoker->m_hd/3;i++){
            tmpX+=x;
            tmpY+=y;
            if(tmpX > 0 && tmpX < w-1 && tmpY > 0 && tmpY < h-1){
                if(!level->m_dungeon->m_tiles[tmpX+tmpY*w].m_walkable){
                    level->m_dungeon->m_tiles[tmpX+tmpY*w] = level->m_tileFactory.getTile("Stone Floor");
                    blasted = true;
                }
            }
        }
        level->recalculateDungeonFov();
        if(blasted){
            level->m_messages->showMessage("You blast away a portion of the wall",MESSAGE_FLAVOUR);
        }
        else{
            level->m_messages->showMessage("The blast did not hit much. Try aiming at a wall",MESSAGE_FLAVOUR);
        }
        return true;
    }
}

//Blast Trap

AbBlastTrap::AbBlastTrap(int id, int type):
	Ability(id)
{
	m_name        = "Blast Trap";
	m_description = "Conjures a rune that explodes when an enemy steps on it";
	m_cost.push_back(ManaCost(2,COLOR_BLUE));
    m_trap = 0;
    m_color = COLOR_BLUE;
}

bool
AbBlastTrap::invoke(Actor *invoker, Level *level, bool &cancelled)
{
    m_trap = level->m_actorFactory.getActor("Blast Trap",level);
    m_trap->m_x = invoker->m_x;
    m_trap->m_y = invoker->m_y;
    level->m_actors.push_back(m_trap);
    cancelled = false;
    return false;
}

void
AbBlastTrap::deactivate(Actor *invoker, Level *level)
{
    level->removeActor(m_trap);
    if(m_trap != 0){
        delete m_trap;
    }
    level->m_messages->showMessage("The Blast Trap disapears", MESSAGE_NOTIFICATION);
}

//Defensive Stance

AbDefensiveStance::AbDefensiveStance(int id):
	Ability(id)
{
	m_name        = "Defensive Stance";
	m_description = "A stance that reduces the damage taken but hinders your movement";
	m_cost.push_back(ManaCost(5,COLOR_WHITE));
    m_color = COLOR_WHITE;
}

bool
AbDefensiveStance::invoke(Actor *invoker, Level *level, bool &cancelled)
{
    level->m_messages->showMessage("You feel Serra watching over you", MESSAGE_NOTIFICATION);
    invoker->addTag(STATUS_HELD);
    invoker->addTag(STATUS_DEFENDED);
    cancelled = false;
    return false;
}

void
AbDefensiveStance::deactivate(Actor *invoker, Level *level)
{
    invoker->removeTag(STATUS_HELD, false);
    invoker->removeTag(STATUS_DEFENDED, false);
    level->m_messages->showMessage("You feel less protected", MESSAGE_NOTIFICATION);
}



// Mana Stream

AbSmallManaStream::AbSmallManaStream(int id, int color):
	Ability(id)
{
    m_color = color;
    switch(m_color){
        case COLOR_RED:
            m_colorName = "red";
            m_name      = "Red";
            break;
        case COLOR_BLUE:
            m_colorName = "blue";
            m_name      = "Blue";
            break;
        case COLOR_WHITE:
            m_colorName = "white";
            m_name      = "White";
            break;
    }
    m_name += " mana Stream";
    m_cooldown = 0;
    m_description = "Continuously regain ";
    m_description += m_colorName;
    m_description += " mana";
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
            int amount = 1+invoker->m_hd/4;
            static_cast<Creature *>(invoker)->regainMana(amount,m_color);
            std::string msg = "You feel a surge of ";
            msg += m_colorName;
            msg += " Mana";
            level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
        }
        m_cooldown = 20-(invoker->m_hd/2);
    }
    m_cooldown--;
}
