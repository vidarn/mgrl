#include "tile.h"
#include <iostream>
#include <stdio.h>

TileFactory::TileFactory()
{
	TCODParser parser;
	TCODParserStruct *tileStruct = parser.newStructure("tile");
	tileStruct->addFlag("transparent")
		->addFlag("walkable")
		->addFlag("flyable")
		;
	tileStruct->addProperty("glyph",TCOD_TYPE_CHAR,true)
        ->addProperty("col",TCOD_TYPE_STRING,false)
        ;
	parser.run("data/tiles",new TileConfigListener(this));
}

TileFactory::~TileFactory()
{
}

void
TileFactory::addTileType(std::string name, Tile tile)
{
	m_tiles.insert(std::pair<std::string ,Tile>(name,tile));
}

Tile
TileFactory::getTile(std::string name)
{
	return m_tiles[name];
}

TCODColor
TileFactory::getColor(std::string name){
	if(strcmp(name.c_str(),"blue")==0)
        return TCODColor::blue;
	if(strcmp(name.c_str(),"white")==0)
        return TCODColor::white;
}

TileConfigListener::TileConfigListener(TileFactory *factory):
	m_factory(factory)
{
}

bool
TileConfigListener::parserNewStruct(TCODParser *parser,const TCODParserStruct *str,const char *name)
{
	m_tile = new Tile();
	m_tileName = name;
	return true;
}

bool
TileConfigListener::parserFlag(TCODParser *parser,const char *name)
{
	bool ok = false;
	if(strcmp(name,"transparent")==0){ok = true;
		m_tile  ->m_transparent = true;}
	if(strcmp(name,"walkable")==0){ok = true;
		m_tile  ->m_walkable = true;}
	if(strcmp(name,"flyable")==0){ok = true;
		m_tile  ->m_flyable = true;}
	if(!ok){
		char buffer[512];
		sprintf(buffer,"Unrecognized flag \"%s\"",name);
		parser->error(buffer);
	}
	return ok;
}

bool
TileConfigListener::parserProperty(TCODParser *parser,const char *name, TCOD_value_type_t type, TCOD_value_t value)
{
	bool ok = false;
	if(strcmp(name,"glyph")==0){
		if(type == TCOD_TYPE_CHAR){
			m_tile->m_glyph = value.c;
			ok = true;
		}
	}
	if(strcmp(name,"col")==0){
		if(type == TCOD_TYPE_STRING){
			m_tile->m_color = m_factory->getColor(value.s);
			ok = true;
		}
	}
	if(!ok){
		char buffer[512];
		sprintf(buffer,"Error reading property \"%s\"",name);
		parser->error(buffer);
	}
	return ok;
}

bool
TileConfigListener::parserEndStruct(TCODParser *parser,const TCODParserStruct *str, const char *name)
{
	m_factory->addTileType(m_tileName,*m_tile);
	delete m_tile;
	return true;
}

void
TileConfigListener::error(const char *msg)
{
	std::cerr << msg << std::endl;
}

Tile::Tile()
{
	m_glyph       = 'X';
	m_walkable    = false;
	m_transparent = false;
	m_flyable     = false;
	m_discovered  = false;
	m_visible     = false;
    m_color       = TCODColor::white;
}

void
Tile::setType(int type)
{
	char g = 'X';
	bool w = false;
	switch(type){
		case T_VOID:
			g = ' ';
			break;
		case T_STONE_FLOOR:
			g = '.';
			w = true;
			break;
		case T_ROCK_WALL:
			g = '#';
			break;
		case T_WATER_STREAM:
			g = '~';
			w = true;
			break;
	}
	m_glyph = g;
	m_walkable = w;
}
