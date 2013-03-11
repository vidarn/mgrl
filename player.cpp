#include "player.h"
#include "level.h"

Player::Player(Level *level):
	Creature(level)
{
	m_glyph = '@';
	m_running = false;
	m_hitDie = 8;
	m_hp = m_maxHp = m_hitDie;
	m_name = "Player";
	m_abilities.push_back(new AbSacredNectar(AB_SACRED_NECTAR));
	m_abilities.push_back(new AbLightningBolt(AB_LIGHTNING_BOLT));
}

bool
Player::walk(int dx, int dy)
{
	int tmpX = m_x + dx;
	int tmpY = m_y + dy;
	if(m_level->m_dungeon->isWalkable(tmpX,tmpY)){
		Creature *creature = m_level->getCreature(tmpX, tmpY);
		if(creature != 0){
			attack(creature);
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
	if(m_level->getVisibleActors().size() == 0){
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

void Player::run()
{
	if(m_glyphLeft == m_level->m_dungeon->getGlyph(     m_x+m_runY+m_runX,m_y-m_runX+m_runY)){
		if(m_glyphRight == m_level->m_dungeon->getGlyph(m_x-m_runY+m_runX,m_y+m_runX+m_runY)){
			m_running = walk(m_runX, m_runY);
			return;
		}
	}
	m_running = false;
}

void Player::die(Actor *source)
{
	std::string msg = "You were killed by ";
	msg += source->m_name;
	m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
	m_level->m_messages->showMessage("You are dead...",MESSAGE_NOTIFICATION);
	m_level->m_playerAlive = false;
}
