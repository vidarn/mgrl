#ifndef LIST_OVERLAY_6PB8HFUZ
#define LIST_OVERLAY_6PB8HFUZ
#include "overlay.h"

struct ListDefinition;

class ListOverlay: public Overlay
{
	public:
		ListOverlay(int height, int width, const char *title, bool multiSelection,
                bool cancelable, std::vector<ListDefinition> definition);
		virtual void render();
		virtual bool handleInput(char key);
        std::vector<ListDefinition> m_definition;
	private:
		int m_numLines;
		int m_numEntries;
		int m_scroll;
        bool m_multiSelection;
        bool m_cancelable;
};

enum ListEntryType{
	LIST_ENTRY,
	LIST_CATEGORY,
};

struct ListDefinition
{
    ListDefinition(ListEntryType type, char key, std::string name):
        m_type(type), m_key(key), m_name(name), m_selected(false)
    {}
	ListEntryType m_type;
	char m_key;
    std::string m_name;
    bool m_selected;
};

#endif /* end of include guard: LIST_OVERLAY_6PB8HFUZ */
