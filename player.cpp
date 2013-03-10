#include "player.h"

Player::Player(Dungeon *dungeon):
	Actor()
{
	m_glyph = '@';
	m_dungeon = dungeon;
	m_running = false;
}

bool
Player::move(int dx, int dy)
{
	int tmpX = m_x + dx;
	int tmpY = m_y + dy;
	if(m_dungeon->isWalkable(tmpX,tmpY)){
		m_x = tmpX;
		m_y = tmpY;
		return true;
	}
	return false;
}

void
Player::startRun(int dx, int dy)
{
	m_running = true;
	m_runX = dx;
	m_runY = dy;
	m_glyphLeft  = m_dungeon->getGlyph(m_x+dy+dx,m_y-dx+dy);
	m_glyphRight = m_dungeon->getGlyph(m_x-dy+dx,m_y+dx+dy);
}

void Player::run()
{
	if(m_glyphLeft == m_dungeon->getGlyph(     m_x+m_runY+m_runX,m_y-m_runX+m_runY)){
		if(m_glyphRight == m_dungeon->getGlyph(m_x-m_runY+m_runX,m_y+m_runX+m_runY)){
			m_running = move(m_runX, m_runY);
			return;
		}
	}
	m_running = false;
}
