#ifndef CALLBACK_OVERLAY_IPGE0GFC
#define CALLBACK_OVERLAY_IPGE0GFC
#include "overlay.h"

class CallbackOverlay: public Overlay
{
	public:
		CallbackOverlay(int height, int width, const char *title,
				void *data, void (*drawCallback)(TCODConsole *, void *, int, int),
				bool (*inputCallback)(char , void*));
		virtual void render();
		virtual bool handleInput(char key);
	private:
		void (*m_drawCallback)(TCODConsole *, void *, int, int);
		bool (*m_inputCallback)(char , void*);
		void *m_data;
};

#endif /* end of include guard: CALLBACK_OVERLAY_IPGE0GFC */
