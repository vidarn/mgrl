#include "overlay.h"
#include <cstring>

Overlay::Overlay(int height, int width, const char *title)
{
	m_x = DUNGEON_WIN_W/2 - width/2;
	m_y = DUNGEON_WIN_H/2 - height/2;
	m_y = m_y >= 1 ? m_y : 1;
	m_x = m_x >= 1 ? m_x : 1;
	height = height <= DUNGEON_WIN_H-2 ? height : DUNGEON_WIN_H-2;
	m_height=height;
	m_width=width;
	m_title = title;
	m_window = newwin(m_height,m_width,m_y,m_x);
	m_console = new TCODConsole(m_width,m_height);
}

void
Overlay::drawBorder()
{
	m_console->clear();
    for(int i = 0; i< m_height; i++){
		TCODConsole::root->putChar(m_x-1,i+m_y,'|');
		TCODConsole::root->putChar(m_x+m_width,i+m_y,'|');
    }
    for(int i = 0; i< m_width; i++){
		TCODConsole::root->putChar(i+m_x,m_y-1,'-');
		TCODConsole::root->putChar(i+m_x,m_y+m_height,'-');
    }
	TCODConsole::root->putChar(m_x-1,m_y-1,'/');
	TCODConsole::root->putChar(m_x-1,m_y+m_height,'\\');
	TCODConsole::root->putChar(m_x+m_width,m_y-1,'\\');
	TCODConsole::root->putChar(m_x+m_width,m_y+m_height,'/');
	int titleLen = strlen(m_title);
	int titleX = m_x-1 + (m_width)/2 - (titleLen+0.5f)/2.0f;
	TCODConsole::root->print(titleX,m_y-1,"|%s|",m_title);
	mvprintw(m_y-1,titleX,"|%s|",m_title);
	refresh();
}
