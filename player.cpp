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
Player::showInventory()
{
    std::vector<ListDefinition>itemList;
    char k = 'a';
    for(int i=0;i<m_inventory.size();i++){
        itemList.push_back(ListDefinition(LIST_ENTRY, k++ , m_inventory[i]->m_name));
    }
    ListOverlay itemOverlay(30, 30, "Inventory", false, itemList);
    itemOverlay.main(m_level);
}
