#include "callback_overlay.h"

CallbackOverlay::CallbackOverlay(int height, int width, const char *title,
	void *data, void (*drawCallback)(TCODConsole *, void *, int, int),
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
	m_drawCallback(m_console, m_data, m_width, m_height);
	TCODConsole::blit(m_console,0,0,0,0,TCODConsole::root,m_x,m_y);
}

bool
CallbackOverlay::handleInput(char key)
{
	return m_inputCallback(key, m_data);
}
