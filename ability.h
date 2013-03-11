#ifndef ABILITY_EK9W4SK
#define ABILITY_EK9W4SK
#include <string>
#include <vector>

enum{
	AB_NONE,
	AB_SACRED_NECTAR,
	AB_LIGHTNING_BOLT,
};

enum{
	COLOR_RED,
	COLOR_BLUE,
	COLOR_WHITE,
	COLOR_LESS,
};

struct ManaCost
{
	ManaCost(int amount, int color)
	{
		m_amount = amount;
		m_color = color;
		switch(m_color){
			case COLOR_RED:
				m_char = 'R'; break;
			case COLOR_BLUE:
				m_char = 'B'; break;
			case COLOR_WHITE:
				m_char = 'W'; break;
			case COLOR_LESS:
				m_char = 'N'; break;
		}
	}
	int m_amount;
	int m_color;
	char m_char;
};

class Ability
{
	public:
		Ability(int id);
		std::string m_name;
		std::string m_description;
		std::string m_flavour;
		int m_id;
		std::vector<ManaCost> m_cost;
};

class AbSacredNectar: public Ability
{
	public:
		AbSacredNectar(int id);
};

class AbLightningBolt: public Ability
{
	public:
		AbLightningBolt(int id);
};

#endif /* end of include guard: ABILITY_EK9W4SK */
