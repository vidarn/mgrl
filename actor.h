#ifndef ACTOR_U9AMWJYS
#define ACTOR_U9AMWJYS
#include <libtcod/libtcod.hpp>
#include <vector>
#include <map>
#include <string>
class Level;

enum{
	DAMAGE_PHYSICAL,
	DAMAGE_FIRE,
};

enum{
	TAG_CREATURE,
	TAG_PLAYER,
	TAG_ITEM,
	TAG_ATTACKABLE,
	TAG_REMEMBER,
    TAG_MINOR,
    TAG_MEDIUM,
    TAG_MAJOR,
    TAG_WALKABLE,
    TAG_TRANSPARENT,
    TAG_OPEN,
    ITEM_OPEN,
    ITEM_BLEND_IN,
    ITEM_DOOR,
    ITEM_PICK_UP,
    ITEM_APPLY,
    ITEM_READ,
    ITEM_QUAFF,
    ITEM_EAT,
    ITEM_EVOKE,
    ITEM_WEAPON,
        ITEM_MELEE,
        ITEM_RANGED,
    ITEM_ARMOR,
        ITEM_SHIELD,
        ITEM_HELMET,
        ITEM_BODY_ARMOR,
        ITEM_GLOVES,
        ITEM_BOOTS,
    ITEM_SCROLL,
    ITEM_FOOD,
    ITEM_CONTAINER,
    ITEM_POTION,
        POTION_HEAL,
        POTION_MANA,
        POTION_NOT_NAME,
    ITEM_STAIRS_DOWN,
    ITEM_STAIRS_UP,
    ITEM_SHARD_OF_KNOWLEDGE,
};

enum{
	ACTOR_CREATURE,
	ACTOR_ITEM,
};

enum{
	TARGET_HOSTILE,
	TARGET_FRIENDLY,
};

class Actor
{
	public:
		Actor();
		virtual void finish(Level *level);
		void walkTowardsPlayer();
		void walkRandomly();
		void render(bool hilight, bool visible);
		void addTag(int tag);
		bool hasTag(int tag);
		virtual bool walk(int dx, int dy);
		virtual void playerSpotted();
		virtual void act();
		virtual bool takeDamage(int amount, int type, Actor *source);
		virtual void healDamage(int amount, Actor *source);
		virtual void die(Actor *source);
		virtual bool fortSave(int dc);
		virtual bool willSave(int dc);
		virtual bool reflSave(int dc);
        virtual void handleProperty(std::string &name, TCOD_value_t &val);
        virtual void handleTag(std::string &tag);
        virtual void open(Actor *opener);
        virtual void pickUp(Actor *item);
        virtual void removeFromInventory(Actor *item);
        virtual void quaff(Actor *drinker);
        virtual Actor *getTarget(int type);
		int m_x, m_y;
		char m_glyph;
		char m_letter;
		TCODColor m_color;
		int m_hp, m_maxHp;
		int m_dx, m_dy;
		int m_ac;
        bool m_discovered;
        std::string m_name;
		std::vector<int> m_tags;
        std::vector<Actor *> m_inventory;
        std::vector<std::string> m_inventoryStrings;
	protected:
		Level *m_level;
};

struct ActorDefinition
{
    ActorDefinition():
        m_genWeight(1.0f)
    {}
    ActorDefinition(int type):
        m_type(type),m_genWeight(1.0f)
    {}
    int   m_type;
    float m_genWeight;
    std::vector<std::string> m_flags;
    std::vector<std::string> m_propertyNames;
    std::vector<TCOD_value_t> m_propertyData;
};

class ActorFactory
{
	public:
		ActorFactory();
		~ActorFactory();
		void addActorType(std::string name, ActorDefinition actorDef);
		Actor *getActor(std::string name, Level *level);
		Actor *getCreature(int hd, Level *level, std::vector<std::string> tags);
        TCODColor getColor(std::string name);
	private:
		std::map<std::string, ActorDefinition> m_actorDefinitions;
};

class ActorConfigListener : public ITCODParserListener {
	public:
		ActorConfigListener(ActorFactory *factory);
		virtual bool parserNewStruct(TCODParser *parser,const TCODParserStruct *str,const char *name);
		virtual bool parserFlag(TCODParser *parser,const char *name);
		virtual bool parserProperty(TCODParser *parser,const char *name, TCOD_value_type_t type, TCOD_value_t value);
		virtual bool parserEndStruct(TCODParser *parser,const TCODParserStruct *str, const char *name);
		virtual void error(const char *msg);
	private:
		ActorFactory *m_factory;
		ActorDefinition m_actorDef;
		const char *m_defName;
};


#endif /* end of include guard: ACTOR_U9AMWJYS */
