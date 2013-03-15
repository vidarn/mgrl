#ifndef ABILITY_EK9W4SK
#define ABILITY_EK9W4SK
#include <libtcod/libtcod.hpp>
#include <string>
#include <vector>

class Actor;
class Level;

enum{
	AB_NONE,
	AB_SACRED_NECTAR,
	AB_LIGHTNING_BOLT,
	AB_SMALL_RED_MANA_STREAM,
};

enum{
	COLOR_RED,
	COLOR_BLUE,
	COLOR_WHITE,
};

struct ManaCost
{
	ManaCost(int amount, int color)
	{
		m_amount = amount;
		m_color = color;
		switch(m_color){
			case COLOR_RED:
				m_char = 20; 
                m_col = TCODColor::lightRed;
                break;
			case COLOR_BLUE:
				m_char = 19; 
                m_col = TCODColor::lightBlue;
                break;
			case COLOR_WHITE:
				m_char = 21; 
                m_col = TCODColor::white;
                break;
		}
	}
	int m_amount;
	int m_color;
	TCODColor m_col;
	char m_char;
};

class Ability
{
	public:
		Ability(int id);
        virtual bool invoke(Actor *invoker, Level *level)=0;
        virtual void deactivate(Actor *invoker, Level *level);
        virtual bool act(Actor *invoker, Level *level);
		std::string m_name;
		std::string m_description;
		std::string m_flavour;
		bool m_active;
		int m_id;
		std::vector<ManaCost> m_cost;
};

class AbSacredNectar: public Ability
{
	public:
		AbSacredNectar(int id);
        virtual bool invoke(Actor *invoker, Level *level);
        virtual void deactivate(Actor *invoker, Level *level);
        Actor *m_nectar;
};

class AbLightningBolt: public Ability
{
	public:
		AbLightningBolt(int id);
        virtual bool invoke(Actor *invoker, Level *level);
};

class AbSmallRedManaStream: public Ability
{
	public:
		AbSmallRedManaStream(int id);
        virtual bool invoke(Actor *invoker, Level *level);
        virtual bool act(Actor *invoker, Level *level);
        int m_cooldown;
};

#endif /* end of include guard: ABILITY_EK9W4SK */
