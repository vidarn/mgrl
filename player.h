#ifndef PLAYER_R44I7II0
#define PLAYER_R44I7II0
#include "creature.h"
#include "ability.h"

enum{
    PLAYER_HUMAN,
    PLAYER_VEDALKEN,
    PLAYER_VIASHINO,
    PLAYER_LEONIN,
};

class Player: public Creature
{
	public:
		Player(int race);
		bool walk(int dx, int dy);
		void startRun(int dx, int dy);
		void run();
        void customize();
        void increaseMana(int amount);
        int generateAttributes();
		virtual void die(Actor *source);
		virtual void finish(Level *level);
        virtual void act();
		virtual void doOpen();
		virtual void doQuaff();
        virtual void pickUp(Actor *item);
        virtual Actor *getTarget(int type);
		virtual void showInventory();
        void drawNewCard(int slot);
        void shuffleLibrary();
        void restockLibrary();
        void levelUp();
        void gainExp(int amount);
		virtual void invokeAbility(int id);
        Actor *getFromInventory(char key);
		bool m_running;
		std::vector<Ability *> m_abilities;
		std::vector<Ability *> m_deck;
		std::vector<Ability *> m_library;
		std::vector<Ability *> m_allSpells;
        int m_exp, m_expForNextLevel;
        int m_race;
        std::string m_raceName;
        std::string m_characterName;
	private:
		int m_runX, m_runY;
		char m_glyphLeft, m_glyphRight;
};

#endif /* end of include guard: PLAYER_R44I7II0 */
