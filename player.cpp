#include <libtcod/libtcod.hpp>
#include "player.h"
#include "level.h"
#include "list_overlay.h"

Player::Player():
	Creature()
{
	m_glyph = 3;
	m_running = false;
	m_hitDie = 8;
	m_hp = m_maxHp = m_hitDie;
	m_name = "You";
	m_abilities.push_back(new AbSacredNectar(AB_SACRED_NECTAR));
	m_abilities.push_back(new AbLightningBolt(AB_LIGHTNING_BOLT));
    addTag(TAG_PLAYER);
}

bool
Player::walk(int dx, int dy)
{
	int tmpX = m_x + dx;
	int tmpY = m_y + dy;
	if(m_level->m_dungeon->isWalkable(tmpX,tmpY)){
        std::vector<int> tags;
        tags.push_back(TAG_ATTACKABLE);
        std::vector<Actor *> actors = m_level->getActors(tmpX, tmpY,tags);
		if(actors.size()>0){
			attack(actors[0]);
		}
		else{
			m_x = tmpX;
			m_y = tmpY;
		}
		return true;
	}
	return false;
}

void
Player::startRun(int dx, int dy)
{
    std::vector<int> tags;
    tags.push_back(TAG_CREATURE);
	if(m_level->getVisibleActors(tags).size() == 0){
		m_running = true;
		m_runX = dx;
		m_runY = dy;
		m_glyphLeft  = m_level->m_dungeon->getGlyph(m_x+dy+dx,m_y-dx+dy);
		m_glyphRight = m_level->m_dungeon->getGlyph(m_x-dy+dx,m_y+dx+dy);
	}
	else{
		m_running = false;
		m_level->m_messages->showMessage("There are monsters nearby", MESSAGE_WARNING);
	}
}

void
Player::run()
{
	if(m_glyphLeft == m_level->m_dungeon->getGlyph(     m_x+m_runY+m_runX,m_y-m_runX+m_runY)){
		if(m_glyphRight == m_level->m_dungeon->getGlyph(m_x-m_runY+m_runX,m_y+m_runX+m_runY)){
			m_running = walk(m_runX, m_runY);
			return;
		}
	}
	m_running = false;
}

void
Player::die(Actor *source)
{
	std::string msg = "You were killed by ";
	msg += source->m_name;
	m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
	m_level->m_messages->showMessage("You are dead...",MESSAGE_NOTIFICATION);
	m_level->m_playerAlive = false;
}

void
Player::finish(Level *level)
{
    m_level = level;
	calculateBonuses();
    pickUp(level->m_actorFactory.getActor("Short Sword",level));
    pickUp(level->m_actorFactory.getActor("Leather Cap",level));
    m_weapon = 0;
    m_helmet = 1;
}

Actor *
Player::getTarget(int type)
{
    Actor *target=0;
    std::vector<int> tags;
	m_level->m_messages->showMessage("Choose a target",MESSAGE_ACTION);
    bool done=false;
    int targetX = m_x;
    int targetY = m_y;
    if(type == TARGET_HOSTILE){
        tags.push_back(ACTOR_CREATURE);
        std::vector<Actor *> visibleActors = m_level->getVisibleActors(tags);
        if(visibleActors.size()>0){
            targetX = visibleActors[0]->m_x;
            targetY = visibleActors[0]->m_y;
        }
    }
    while(!done){
        m_level->m_dungeon->clearHilight();
        int tmpX = m_x;
        int tmpY = m_y;
        TCODLine::init (tmpX, tmpY, targetX, targetY);
        do {
            if(m_level->m_dungeon->isWalkable(tmpX,tmpY) && m_level->m_dungeon->isVisible(tmpX,tmpY)){
                m_level->m_dungeon->setHilight(tmpX,tmpY,1);
                std::vector<Actor *> hitActors = m_level->getActors(tmpX,tmpY,tags);
                if(hitActors.size() >0){
                    target = hitActors[0];
                    break;
                }
            }
            else{
                break;
            }
        } while (!TCODLine::step(&tmpX,&tmpY));
        m_level->m_dungeon->setHilight(targetX,targetY,1);
        m_level->render();
        TCODConsole::flush();

        TCOD_key_t key = TCODConsole::waitForKeypress(true);
        if(key.pressed){
            switch(key.c) {
                case 'h' :
                    targetX--; break;
                case 'j' :
                    targetY++; break;
                case 'k' :
                    targetY--; break;
                case 'l' :
                    targetX++; break;
                case 'y' :
                    targetX--;targetY--; break;
                case 'u' :
                    targetX++;targetY--; break;
                case 'b' :
                    targetX--;targetY++; break;
                case 'n' :
                    targetX++;targetY++; break;
                case ENTER :
                    done=true; break;
                case ESC :
                    target=0; done=true; break;
            }
        }
        targetX = std::min(DUNGEON_WIN_W-1, std::max(0, targetX));
        targetY = std::min(DUNGEON_WIN_H-1, std::max(0, targetY));
    }
    m_level->m_dungeon->clearHilight();
    return target;
}

void
Player::doOpen()
{
    std::vector<int> tags;
    tags.push_back(ITEM_OPEN);
    std::vector<Actor *> actors = m_level->getActors(m_x, m_y,tags);
    if(actors.size()>0){
        actors[0]->open(this);
    }
    else{
        m_level->m_messages->showMessage("There is nothing here to open", MESSAGE_NOTIFICATION);
    }
}

void
Player::doQuaff()
{
    std::vector<ListDefinition>itemList;
    bool empty = true;
    for(int i=0;i<m_inventory.size();i++){
        Actor *actor = m_inventory[i];
        if(actor->hasTag(ITEM_QUAFF)){
            std::string st = actor->m_name;
            itemList.push_back(ListDefinition(LIST_ENTRY, actor->m_letter, st));
        }
    }
    ListOverlay itemOverlay(30, 30, "Quaff What?", false, itemList);
    itemOverlay.main(m_level);
    for(int i=0;i<itemOverlay.m_definition.size();i++){
        ListDefinition &def = itemOverlay.m_definition[i];
        if(def.m_selected){
            Actor *item = getFromInventory(def.m_key);
            if(item != 0){
                item->quaff(this);
            }
        }
    }
}

void
Player::pickUp(Actor *item)
{
    m_inventory.push_back(item);
    bool keepLetter = item->m_letter != 0;
    char freeLetter = 'a';
    bool done=false;
    if(keepLetter){
        for(int i=0;i<m_inventory.size();i++){
            Actor *a = m_inventory[i];
            if(a->m_letter == item->m_letter){
                keepLetter = false;
            }
        }
    }
    if(!keepLetter){
        while(!done){
            done = true;
            for(int i=0;i<m_inventory.size();i++){
                Actor *a = m_inventory[i];
                if(a->m_letter == freeLetter){
                    done = false;
                }
            }
            if(!done){
                freeLetter++;
            }
        }
        item->m_letter = freeLetter;
    }
    std::string msg = "";
    msg += item->m_letter;
    msg += " - ";
    msg += item->m_name;
    m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
}

void
Player::showInventory()
{
    std::vector<int>tagList;
    std::vector<ListDefinition>categoryList;
    tagList.push_back(ITEM_WEAPON);categoryList.push_back(ListDefinition(LIST_CATEGORY,')',"Weapons"));
    tagList.push_back(ITEM_ARMOR );categoryList.push_back(ListDefinition(LIST_CATEGORY,'[',"Armor"));
    tagList.push_back(ITEM_POTION);categoryList.push_back(ListDefinition(LIST_CATEGORY,'!',"Potions"));
    std::vector<ListDefinition>itemList;
    for(int category=0;category<categoryList.size();category++){
        int tag = tagList[category];
        itemList.push_back(categoryList[category]);
        bool empty = true;
        for(int i=0;i<m_inventory.size();i++){
            Actor *actor = m_inventory[i];
            if(actor->hasTag(tag)){
                std::string st = actor->m_name;
                if(m_weapon == i){
                    st += " (in hand)";
                }
                if(m_helmet==i||m_shield==i||m_bodyArmor==i||m_gloves==i||m_boots==i){
                    st += " (worn)";
                }
                itemList.push_back(ListDefinition(LIST_ENTRY, actor->m_letter, st));
                empty = false;
            }
        }
        if(empty){
            itemList.pop_back();
        }
    }
    ListOverlay itemOverlay(30, 30, "Inventory", false, itemList);
    itemOverlay.main(m_level);
}

void
Player::invokeAbility(int id)
{
    Ability *ab = m_abilities[id];
    if(!ab->m_active){
        std::string msg = "You invoke ";
        msg += ab->m_name;
        m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
        if(ab->invoke(this,m_level)){
        }
        else{
            ab->m_active = true;
        }
    }
    else{
        ab->deactivate(this,m_level);
        ab->m_active = false;
    }
}

Actor *
Player::getFromInventory(char key){
    for(int i=0;i<m_inventory.size();i++){
        Actor *item = m_inventory[i];
        if(item->m_letter == key){
            return item;
        }
    }
    return 0;
}
