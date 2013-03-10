#include "callback_overlay.h"

CallbackOverlay::CallbackOverlay(int height, int width, const char *title,
	void *data, void (*drawCallback)(WINDOW *, void *, int, int),
	bool (*inputCallback)(char , void*)):
	Overlay(height,width,title)
{
	m_drawCallback  = drawCallback;
	m_inputCallback = inputCallback;
	m_data          = data;
}

void
CallbackOverlay::render()
{
	drawBorder();
	wmove(m_window,0,0);
    for(int i = 0; i< m_width*m_height; i++){
        waddch(m_window,' ');
    }
	wmove(m_window,0,0);
	m_drawCallback(m_window, m_data, m_width, m_height);
    wrefresh(m_window);
	TCODConsole::blit(m_console,0,0,0,0,TCODConsole::root,m_x,m_y);
}

bool
CallbackOverlay::handleInput(char key)
{
	return m_inputCallback(key, m_data);
}
