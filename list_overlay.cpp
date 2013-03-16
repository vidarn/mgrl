#include "overlay.h"
#include "list_overlay.h"
#include <cstdio>
#include <algorithm>

ListOverlay::ListOverlay(int height, int width, const char *title, bool multiSelection,
        bool cancelable, std::vector<ListDefinition> definition):
	Overlay(height,width, title)
{
	m_definition = definition;
	m_numEntries=m_definition.size();
	m_numLines = m_numEntries;
	if(m_numLines > m_height){
		m_numLines = m_height-2;
		m_scroll = 0;
	}
	else{
		m_scroll = -1;
	}
	m_multiSelection = multiSelection;
	m_cancelable = cancelable;
}

void
ListOverlay::render()
{
	drawBorder();
	int numEntries = 0;
	char buffer[256];
	int y=0;
    for(int i=0;i<m_numEntries;i++){
        ListDefinition *d = &(m_definition[i]);
		numEntries++;
		if(numEntries>=m_scroll){
			if(y < m_numLines){
				if(d->m_type == LIST_ENTRY){
                    char separator = '-';
                    if(d->m_selected)
                        separator = '+';
					sprintf(buffer," %c %c %s", d->m_key, separator, d->m_name.c_str());
				}
				if(d->m_type == LIST_CATEGORY){
					sprintf(buffer,"%s", d->m_name.c_str(), d->m_key);
				}
				int ii=0;
				while(buffer[ii] != 0){
					m_console->putChar(ii,y,buffer[ii]);
					ii++;
					if(ii >= m_width){
						for(int iii=i-1;iii>0 && ii-iii <= 3;iii--){
							m_console->putChar(iii,y,'.');
						}
						break;
					}
				}
			}
			y++;
		}
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
    bool cont = true;
	switch(key){
		case '<':
			if(m_scroll != -1)
				m_scroll = m_scroll > 0 ? m_scroll -1 : 0;
			break;
		case '>':
			if(m_scroll != -1)
				m_scroll = m_scroll < m_numEntries - m_numLines +1 ? m_scroll +1 : m_numEntries - m_numLines +1;
			break;
		case ESC:
            if(m_cancelable){
                for(int i=0;i<m_numEntries;i++){
                    ListDefinition &d = m_definition[i];
                    d.m_selected = false;
                }
                cont =  false;
            }
            break;
		case ENTER:
            if(m_multiSelection)
                cont =  false;
            break;
	}
    for(int i=0;i<m_numEntries;i++){
        ListDefinition &d = m_definition[i];
        if(key == d.m_key){
            d.m_selected = !d.m_selected;
            if(!m_multiSelection){
                cont = false;
            }
        }
    }
	return cont;
}
