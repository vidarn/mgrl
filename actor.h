#ifndef ACTOR_U9AMWJYS
#define ACTOR_U9AMWJYS
#include <libtcod/libtcod.hpp>
#include <vector>
class Level;

enum{
	DAMAGE_PHYSICAL,
	DAMAGE_FIRE,
};

enum{
	TAG_CREATURE,
	TAG_PLAYER,
	TAG_ITEM,
};

class Actor
{
	public:
		Actor(Level *level);
		void walkTowardsPlayer();
		void walkRandomly();
		void render();
		void addTag(int tag);
		bool hasTag(int tag);
		virtual bool walk(int dx, int dy);
		virtual void playerSpotted();
		virtual void act();
		virtual bool takeDamage(int amount, int type, Actor *source);
		virtual void die(Actor *source);
		virtual bool fortSave(int dc);
		virtual bool willSave(int dc);
		virtual bool reflSave(int dc);
		int m_x, m_y;
		char m_glyph;
		int m_hp, m_maxHp;
		int m_dx, m_dy;
		int m_ac;
		const char *m_name;
		std::vector<int> m_tags;
	protected:
		Level *m_level;
};

#endif /* end of include guard: ACTOR_U9AMWJYS */
