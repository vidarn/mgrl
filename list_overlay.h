#ifndef LIST_OVERLAY_6PB8HFUZ
#define LIST_OVERLAY_6PB8HFUZ
#include "overlay.h"

struct ListDefinition;

class ListOverlay: public Overlay
{
	public:
		ListOverlay(int height, int width, const char *title, ListDefinition *definition);
		virtual void render();
		virtual bool handleInput(char key);
	private:
		ListDefinition *m_definition;
		int m_numLines;
		int m_numEntries;
		int m_scroll;
};

enum ListEntryType{
	LIST_ENTRY,
	LIST_CATEGORY,
	LIST_DONE,
};

struct ListEntryData
{
	const char *m_key;
	const char *m_name;
};

struct ListDefinition
{
	ListEntryType m_type;
	ListEntryData m_data;
};

#endif /* end of include guard: LIST_OVERLAY_6PB8HFUZ */
