#ifndef OVERLAY_X4LZW6PV
#define OVERLAY_X4LZW6PV
#include <libtcod/libtcod.hpp>
#include "common.h"

class Overlay{
	public:
		Overlay(int height, int width, const char *title);
		~Overlay();
		void setPos(int x, int y);
		void setSize(int w, int h);
		virtual void render()=0;
		virtual bool handleInput(char key)=0;
		void drawBorder();
	protected:
		int     m_y      , m_x;
		int     m_height , m_width;
		TCODConsole *m_console;
		const char *m_title;
};

#endif /* end of include guard: OVERLAY_X4LZW6PV */
