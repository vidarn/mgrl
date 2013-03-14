#include <boost/random/uniform_int_distribution.hpp>
#include <cstdio>
#include <sstream>
#include "actor.h"
#include "level.h"

Actor::Actor():
	m_x(0),m_y(0),m_glyph('A'),m_hp(1),m_maxHp(1), m_level(0), m_dx(0),
	m_dy(0), m_name("something"), m_ac(0), m_color(TCODColor::white)
{
}
void
Actor::finish(Level *level)
{
    m_level = level;
}

void
Actor::walkTowardsPlayer()
{
	m_level->walkTowardsPlayer(&m_x,&m_y);
}

void
Actor::walkRandomly()
{
	boost::random::uniform_int_distribution<> turnDist(0,3);
	boost::random::uniform_int_distribution<> dist(-1,1);
	int turn = turnDist(RAND);
	if(turn == 0){
		m_dx = dist(RAND);
		m_dy = dist(RAND);
	}
	int a =0;
	while(!m_level->m_dungeon->isWalkable(m_x+m_dx,m_y+m_dy) && a < 5){
		m_dx = dist(RAND);
		m_dy = dist(RAND);
		a++;
	}
	walk(m_dx,m_dy);
}

bool
Actor::walk(int dx,int dy)
{
	if(m_level->m_dungeon->isWalkable(m_x+dx,m_y+dy)){
		m_x += dx;
		m_y += dy;
		return true;
	}
	return false;
}

void
Actor::render()
{
	TCODConsole::root->setDefaultForeground(m_color);
	TCODConsole::root->putChar(m_x,m_y,m_glyph);
}

void
Actor::addTag(int tag)
{
	bool add = true;
	for(int i=0;i<m_tags.size();i++){
		if(m_tags[i] == tag)
			add = false;
	}
	if(add){
		m_tags.push_back(tag);
	}
}

bool
Actor::hasTag(int tag)
{
	for(int i=0;i<m_tags.size();i++){
		if(m_tags[i] == tag)
			return true;
	}
	return false;
}

void
Actor::playerSpotted()
{
}

void
Actor::act()
{
}

bool
Actor::takeDamage(int amount, int type, Actor *source)
{
	m_hp -= amount;
    std::stringstream ss;
    ss << m_name << " takes " << amount << " damage and has " << m_hp << " hp left";
    m_level->m_messages->showMessage(ss.str(), MESSAGE_ACTION);
	return m_hp > 0;
}

void
Actor::die(Actor *source)
{
	std::string msg;
    if(!source->hasTag(TAG_PLAYER))
        msg = "The ";
	msg += source->m_name;
	msg += " destroy the ";
	msg += m_name;
	m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
}

bool
Actor::fortSave(int dc)
{
	return false;
}

bool
Actor::willSave(int dc)
{
	return false;
}

bool
Actor::reflSave(int dc)
{
	return false;
}
void
Actor::handleProperty(std::string &name, TCOD_value_t &val)
{
}

void
Actor::handleTag(std::string &tag)
{
}

ActorFactory::ActorFactory()
{
	TCODParser parser;
	TCODParserStruct *creatureStruct = parser.newStructure("creature");
	creatureStruct->addFlag("spider")
		;
	creatureStruct->addProperty("glyph",TCOD_TYPE_CHAR,true)
        ->addProperty("col",TCOD_TYPE_STRING,false)
        ->addProperty("name",TCOD_TYPE_STRING,false)
        ->addProperty("genWeight",TCOD_TYPE_FLOAT,false)
        ;
	TCODParserStruct *itemStruct = parser.newStructure("item");
	itemStruct->addProperty("glyph",TCOD_TYPE_CHAR,true)
        ->addProperty("col",TCOD_TYPE_STRING,false)
        ->addProperty("name",TCOD_TYPE_STRING,false)
        ->addProperty("genWeight",TCOD_TYPE_FLOAT,false)
        ;
	parser.run("data/actors",new ActorConfigListener(this));
}

ActorFactory::~ActorFactory()
{
}

void
ActorFactory::addActorType(std::string name, ActorDefinition actorDef)
{
	m_actorDefinitions.insert(std::pair<std::string ,ActorDefinition>(name,actorDef));
}

Actor *
ActorFactory::getActor(std::string name, Level *level)
{
    ActorDefinition &def = m_actorDefinitions[name];
    Actor *actor;
    if(def.m_type == ACTOR_CREATURE)
        actor = new Creature();
    else if(def.m_type == ACTOR_ITEM)
        actor = new Item();
    else
        actor = new Actor();
    actor->m_name = name.c_str();
    for(int i=0;i<def.m_propertyNames.size();i++){
        bool handled=false;
        std::string  &name = def.m_propertyNames[i];
        TCOD_value_t &val  = def.m_propertyData[i];
        if(name == "glyph"){handled=true;
            actor->m_glyph = val.c;
        }
        if(name == "col"){handled=true;
            actor->m_color = getColor(val.s);
        }
        if(name == "name"){handled=true;
            actor->m_name = val.s;
        }
        if(!handled){
            actor->handleProperty(name,val);
        }
    }
    for(int i=0;i<def.m_flags.size();i++){
        actor->handleTag(def.m_flags[i]);
    }
    actor->finish(level);
    return actor;
}

Actor *
ActorFactory::getCreature(int hd, Level *level, std::vector<std::string> tags)
{
    std::vector<std::string> matchingDefinitions;
    std::vector<float> definitionWeights;
    std::map<std::string, ActorDefinition>::iterator iter;
    for(iter=m_actorDefinitions.begin();iter!=m_actorDefinitions.end();++iter){
        std::string defName = iter->first;
        ActorDefinition &def = iter->second;
        if(def.m_type == ACTOR_CREATURE){
            int creatureHd = 1;
            for(int i=0;i<def.m_propertyNames.size();i++){
                std::string  &name = def.m_propertyNames[i];
                TCOD_value_t &val  = def.m_propertyData[i];
                if(name == "hp"){
                    creatureHd = val.dice.nb_rolls;
                }
            }
            std::vector<bool> tagMatches;
            for(int i=0;i<tags.size();i++){
                tagMatches.push_back(false);
            }
            for(int i=0;i<def.m_flags.size();i++){
                std::string &flag = def.m_flags[i];
                for(int ii=0;ii<tags.size();ii++){
                    if(tags[ii] == flag){
                        tagMatches[ii] = true;
                    }
                }
            }
            bool allTagsMatch = true;
            for(int i=0;i<tags.size();i++){
                allTagsMatch = allTagsMatch && tagMatches[i];
            }
            if(creatureHd == hd && allTagsMatch){
                definitionWeights.push_back(def.m_genWeight);
                matchingDefinitions.push_back(defName);
            }
        }
    }
    if(matchingDefinitions.size() > 0){
        boost::random::discrete_distribution<> creatureDist(definitionWeights);
        int a = creatureDist(RAND);
        std::string name = matchingDefinitions[a];
        return getActor(name,level);
    }
    else{
        return 0;
    }
}

TCODColor
ActorFactory::getColor(std::string name){
	if(name == "blue")
        return TCODColor::blue;
	if(name == "green")
        return TCODColor::green;
	if(name == "dark green")
        return TCODColor::darkGreen;
	if(name == "white")
        return TCODColor::white;
}

ActorConfigListener::ActorConfigListener(ActorFactory *factory):
	m_factory(factory)
{
}

bool
ActorConfigListener::parserNewStruct(TCODParser *parser,const TCODParserStruct *str,const char *name)
{
	bool ok = false;
	if(strcmp(str->getName(),"creature")==0){ok=true;
        m_actorDef  = ActorDefinition(ACTOR_CREATURE);
    }
	if(strcmp(str->getName(),"item")==0){ok=true;
        m_actorDef  = ActorDefinition(ACTOR_ITEM);
    }
    m_defName = name;
	return ok;
}

bool
ActorConfigListener::parserFlag(TCODParser *parser,const char *name)
{
    m_actorDef.m_flags.push_back(name);
    return true;
}

bool
ActorConfigListener::parserProperty(TCODParser *parser,const char *name, TCOD_value_type_t type, TCOD_value_t value)
{
	if(strcmp(name,"genWeight")==0){
        m_actorDef.m_genWeight = value.f;
    }
    else{
        m_actorDef.m_propertyNames.push_back(name);
        m_actorDef.m_propertyData.push_back(value);
    }
}

bool
ActorConfigListener::parserEndStruct(TCODParser *parser,const TCODParserStruct *str, const char *name)
{
	m_factory->addActorType(m_defName,m_actorDef);
	return true;
}

void
ActorConfigListener::error(const char *msg)
{
	std::cerr << msg << std::endl;
}
