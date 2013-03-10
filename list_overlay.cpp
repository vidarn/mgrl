#include "list_overlay.h"
#include <cstdio>

ListOverlay::ListOverlay(int height, int width, const char *title, ListDefinition *definition):
	Overlay(height,width, title)
{
	m_definition = definition;
	ListDefinition *d = m_definition;
	m_numEntries=0;
	while(d->m_type != LIST_DONE){
		m_numEntries++;
		d++;
	}
	m_numLines = m_numEntries;
	if(m_numLines > m_height){
		m_numLines = m_height-2;
		m_scroll = 0;
	}
	else{
		m_scroll = -1;
	}
}

void
ListOverlay::render()
{
	drawBorder();
	int numEntries = 0;
	ListDefinition *d = m_definition;
	char buffer[256];
	int y=0;
	while(d->m_type != LIST_DONE){
		numEntries++;
		if(numEntries>=m_scroll){
			if(y < m_numLines){
				if(d->m_type == LIST_ENTRY){
					sprintf(buffer," [%s] %s", d->m_data.m_key, d->m_data.m_name);
				}
				if(d->m_type == LIST_CATEGORY){
					sprintf(buffer,"%s (%s):", d->m_data.m_name, d->m_data.m_key);
				}
				int i=0;
				while(buffer[i] != 0){
					m_console->putChar(i,y,buffer[i]);
					i++;
					if(i >= m_width){
						for(int ii=i-1;ii>0 && i-ii <= 3;ii--){
							m_console->putChar(ii,y,'.');
						}
						break;
					}
				}
			}
			y++;
		}
		d++;
	}
	if(m_scroll != -1){
		m_console->print(0,m_height-2,"--");
		m_console->print(0,m_height-1,"<,> to scroll");
	}
	TCODConsole::blit(m_console,0,0,0,0,TCODConsole::root,m_x,m_y);
}

bool
ListOverlay::handleInput(char key)
{
	switch(key){
		case '<':
			if(m_scroll != -1)
				m_scroll = m_scroll > 0 ? m_scroll -1 : 0;
			break;
		case '>':
			if(m_scroll != -1)
				m_scroll = m_scroll < m_numEntries - m_numLines +1 ? m_scroll +1 : m_numEntries - m_numLines +1;
			break;
		case 'q':
			return false;
	}
	return true;
}
