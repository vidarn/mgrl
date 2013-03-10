#include "player.h"

Player::Player(Dungeon *dungeon):
	Actor()
{
	m_glyph = '@';
	m_dungeon = dungeon;
}

void
Player::move(int dx, int dy)
{
	int tmpX = m_x + dx;
	int tmpY = m_y + dy;
	if(m_dungeon->isWalkable(tmpX,tmpY)){
		m_x = tmpX;
		m_y = tmpY;
	}
}
