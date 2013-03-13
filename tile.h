#ifndef TILE_IWRFWCCC
#define TILE_IWRFWCCC
#include <libtcod/libtcod.hpp>
#include <vector>
#include <map>
#include <string>

enum TILE_TYPES
{
	T_VOID,
	T_STONE_FLOOR,
	T_ROCK_WALL,
	T_WATER_STREAM,
};

class Tile{
	public:
		Tile();
		void setType(int type);
		char m_glyph;
		TCODColor m_color;
		bool m_walkable;
		bool m_transparent;
		bool m_flyable;
		bool m_discovered;
		bool m_visible;
};

class TileFactory
{
	public:
		TileFactory();
		~TileFactory();
		void addTileType(std::string name, Tile tile);
		Tile getTile(std::string name);
        TCODColor getColor(std::string name);
	private:
		std::map<std::string, Tile> m_tiles;
};

class TileConfigListener : public ITCODParserListener {
	public:
		TileConfigListener(TileFactory *factory);
		virtual bool parserNewStruct(TCODParser *parser,const TCODParserStruct *str,const char *name);
		virtual bool parserFlag(TCODParser *parser,const char *name);
		virtual bool parserProperty(TCODParser *parser,const char *name, TCOD_value_type_t type, TCOD_value_t value);
		virtual bool parserEndStruct(TCODParser *parser,const TCODParserStruct *str, const char *name);
		virtual void error(const char *msg);
	private:
		TileFactory *m_factory;
		Tile *m_tile;
		const char *m_tileName;
};


#endif /* end of include guard: TILE_IWRFWCCC */
