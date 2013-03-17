#include <libtcod/libtcod.hpp>
#include <algorithm>
#include <cfloat>
#include <sstream>
#include "player.h"
#include "level.h"
#include "list_overlay.h"

Player::Player(int race):
	Creature()
{
    m_race = race;
	m_glyph = 3;
	m_running = false;
	m_hitDie = 12;
	m_hp = m_maxHp = m_hitDie;
    m_exp = 0;
    m_hd = 1;
    m_expForNextLevel = 1000;
	m_name = "You";
    m_mana[0] = m_maxMana[0] = 0;
    m_mana[1] = m_maxMana[1] = 0;
    m_mana[2] = m_maxMana[2] = 0;
	/*m_allSpells.push_back(new AbDefensiveStance(AB_DEFENSIVE_STANCE));
	m_allSpells.push_back(new AbBlastTrap(AB_BLAST_TRAP,0));
	m_allSpells.push_back(new AbSmallManaStream(AB_SMALL_MANA_STREAM, COLOR_BLUE));
	m_allSpells.push_back(new AbInstantTunnel(AB_INSTANT_TUNNEL));
	m_allSpells.push_back(new AbSmallManaStream(AB_SMALL_MANA_STREAM, COLOR_RED));
	m_allSpells.push_back(new AbBlink(AB_BLINK));
	m_allSpells.push_back(new AbBlink(AB_BLINK));
	m_allSpells.push_back(new AbSacredNectar(AB_SACRED_NECTAR));
	m_allSpells.push_back(new AbLightningBolt(AB_LIGHTNING_BOLT));
	m_allSpells.push_back(new AbSmallManaStream(AB_SMALL_MANA_STREAM, COLOR_WHITE));

    for(int i=0;i<m_allSpells.size();i++){
        m_deck.push_back(m_allSpells[i]);
        if(i<4){
            m_abilities.push_back(m_allSpells[i]);
        }
    }
    restockLibrary();*/

    addTag(TAG_PLAYER);
}

static void
drawNameInput(TCODConsole *console, void *data, int width, int height)
{
    std::string *str = static_cast<std::string *>(data);
    console->print(0,1," %s",str->c_str());
}

static bool
handleNameInput(char key, void *data)
{
    std::string *str = static_cast<std::string *>(data);
    if(key >= 'A' && key <= 'z'){
        str->push_back(key);
    }
    if(key == BS){
        if(str->size() > 0){
            str->erase(str->size()-1);
        }
    }
    return key != ENTER;
}

static bool
handleWinScreen(char key, void *data)
{
    if(key == 'q'){
        return false;
    }
    return true;
}

static void
drawWinScreen(TCODConsole *console, void *data, int width, int height)
{
    std::string *str = static_cast<std::string *>(data);
    console->print(0,1,"You have obtained the Orb of Knowledge");
    console->print(0,2,"Your new powers are beyond human imagination");
    console->print(0,3,"Game Over");
    console->print(0,5,"Press q to quit");
}

static void
drawDeathScreen(TCODConsole *console, void *data, int width, int height)
{
	console->setDefaultForeground(TCODColor::red);
    std::string *str = static_cast<std::string *>(data);
    console->print(0,1,"You have been slain");
    console->print(0,3,"Game Over");
    console->print(0,5,"Press q to quit");
}

void
Player::customize()
{
    m_characterName = "";
    CallbackOverlay nameInput(3,30, "What is your name?", &m_characterName, &drawNameInput, &handleNameInput);
    //nameInput.main(m_level);
    std::vector<ListDefinition>raceList;
    raceList.push_back(ListDefinition(LIST_ENTRY,'a',"Human"));
    raceList.push_back(ListDefinition(LIST_ENTRY,'b',"Vedalken"));
    raceList.push_back(ListDefinition(LIST_ENTRY,'c',"Leonin"));
    raceList.push_back(ListDefinition(LIST_ENTRY,'d',"Viashino"));
    raceList.push_back(ListDefinition(LIST_ENTRY,'r',"Random"));
    ListOverlay raceOverlay(5, 30, "Select starting race", false, false, raceList);
    raceOverlay.main(m_level);
    for(int i=0;i<raceOverlay.m_definition.size();i++){
        ListDefinition &def = raceOverlay.m_definition[i];
        if(def.m_selected){
            switch(i){
                case 0:
                    m_race=PLAYER_HUMAN; break;
                case 1:
                    m_race=PLAYER_VEDALKEN; break;
                case 2:
                    m_race=PLAYER_LEONIN; break;
                case 3:
                    m_race=PLAYER_VIASHINO; break;
                case 4:
                    boost::random::uniform_int_distribution<> raceDist(0,3);
                    m_race = raceDist(RAND);
                    break;
            }
        }
    }
    pickUp(m_level->m_actorFactory.getActor("Short Sword",m_level));
    pickUp(m_level->m_actorFactory.getActor("Minor Mana Potion",m_level));
    pickUp(m_level->m_actorFactory.getActor("Short Bow",m_level));
    Actor *arrow = m_level->m_actorFactory.getActor("Arrow",m_level);
    arrow->m_amount = 20;
    pickUp(arrow);
    m_weapon = 0;
    m_quiver = 3;
    int manaPoints = generateAttributes();
	calculateBonuses();
    calculateHp();
    calculateAc();
    TCODConsole::root->clear();
    increaseMana(manaPoints);
    drawNewCard(0,true);
    drawNewCard(1);
    drawNewCard(2);
    m_level->m_playerGenerated = true;
    std::string msg = "Welcome ";
    /*msg += m_characterName;
    msg += " the ";*/
    msg += m_raceName;
    m_level->m_messages->showMessage(msg, MESSAGE_WARNING);
    m_level->m_messages->showMessage("You enter the dungeons of Svogthos in search of the Orb of Knowledge", MESSAGE_WARNING);
}

void
Player::increaseMana(int amount)
{
    for(int i=0;i<amount;i++){
        std::vector<ListDefinition>manaList;

        std::stringstream redEntry;
        redEntry << "Red   (Current: " << m_maxMana[COLOR_RED] << ")";
        manaList.push_back(ListDefinition(LIST_ENTRY,'a',redEntry.str().c_str()));

        std::stringstream blueEntry;
        blueEntry << "Blue  (Current: " << m_maxMana[COLOR_BLUE] << ")";
        manaList.push_back(ListDefinition(LIST_ENTRY,'b',blueEntry.str().c_str()));

        std::stringstream whiteEntry;
        whiteEntry << "White (Current: " << m_maxMana[COLOR_WHITE] << ")";
        manaList.push_back(ListDefinition(LIST_ENTRY,'c',whiteEntry.str().c_str()));

        std::stringstream heading;
        heading << "Select mana to increase (" << i+1 << "/" << amount << ")";
        ListOverlay manaOverlay(3, 32, heading.str().c_str(), false, false, manaList);
        manaOverlay.main(m_level);
        for(int i=0;i<manaOverlay.m_definition.size();i++){
            ListDefinition &def = manaOverlay.m_definition[i];
            if(def.m_selected){
                m_maxMana[i] += 1;
            }
        }
    }
    for(int i=0;i<3;i++){
        m_mana[i] = m_maxMana[i] - m_lockedMana[i];
    }
}

int
Player::generateAttributes()
{
    for(int i=0;i<6;i++){
        int dice[3];
        for(int ii=0;ii<3;ii++){
            dice[ii] = d6(RAND);
        }
        int lowest = 0;
        for(int ii=1;ii<3;ii++){
            if(dice[ii] < dice[lowest]){
                lowest = ii;
            }
        }
        int amount = 6 + dice[(lowest+1)%3] + dice[(lowest+2)%3];
        switch(i){
            case 0:
                m_str = amount; break;
            case 1:
                m_dex = amount; break;
            case 2:
                m_con = amount; break;
            case 3:
                m_int = amount; break;
            case 4:
                m_wis = amount; break;
            case 5:
                m_cha = amount; break;
        }
    }
    int manaPoints = 0;
    switch(m_race){
        case PLAYER_HUMAN:
            m_raceName = "Human";
            manaPoints = 4;
            break;
        case PLAYER_VEDALKEN:
            m_raceName = "Vedalken";
            m_str -= 3;
            m_con -= 3;
            manaPoints = 3;
            m_maxMana[COLOR_BLUE] = 3;
            break;
        case PLAYER_VIASHINO:
            m_raceName = "Viashino";
            m_str += 3;
            m_con -= 3;
            manaPoints = 1;
            m_maxMana[COLOR_RED] = 3;
            break;
        case PLAYER_LEONIN:
            m_raceName = "Leonin";
            m_dex += 3;
            m_str -= 3;
            manaPoints = 2;
            m_maxMana[COLOR_WHITE] = 2;
            break;
    }
    return manaPoints;
}

bool
Player::walk(int dx, int dy)
{
	int tmpX = m_x + dx;
	int tmpY = m_y + dy;
	if(m_level->isWalkable(tmpX,tmpY)){
        if(!hasTag(STATUS_HELD)){
            m_x = tmpX;
            m_y = tmpY;
            act();
            return true;
        }
        else{
            m_level->m_messages->showMessage("You cannot walk", MESSAGE_ACTION);
            return false;
        }
    }
    else{
        std::vector<int> tags;
        tags.push_back(TAG_ATTACKABLE);
        std::vector<Actor *> actors = m_level->getActors(tmpX, tmpY,tags);
		if(actors.size()>0){
			attack(actors[0],ATTACK_MELEE);
            act();
            return true;
		}
        else{
            std::vector<int> tags;
            tags.push_back(ITEM_DOOR);
            std::vector<Actor *> actors = m_level->getActors(tmpX, tmpY,tags);
            if(actors.size()>0){
                actors[0]->open(this);
                act(0.6);
                return true;
            }
        }
	}
	return false;
}

void
Player::startRun(int dx, int dy)
{
    std::vector<int> tags;
    tags.push_back(TAG_CREATURE);
	if(m_level->getVisibleActors(tags).size() == 0){
		m_running = true;
		m_runX = dx;
		m_runY = dy;
		m_glyphLeft  = m_level->m_dungeon->getGlyph(m_x+dy+dx,m_y-dx+dy);
		m_glyphRight = m_level->m_dungeon->getGlyph(m_x-dy+dx,m_y+dx+dy);
	}
	else{
		m_running = false;
		m_level->m_messages->showMessage("There are monsters nearby", MESSAGE_WARNING);
	}
}

void
Player::run()
{
	if(m_glyphLeft == m_level->m_dungeon->getGlyph(     m_x+m_runY+m_runX,m_y-m_runX+m_runY)){
		if(m_glyphRight == m_level->m_dungeon->getGlyph(m_x-m_runY+m_runX,m_y+m_runX+m_runY)){
			m_running = walk(m_runX, m_runY);
			return;
		}
	}
	m_running = false;
}

void
Player::die(Actor *source)
{
	std::string msg = "You were killed by a ";
	msg += source->m_name;
	m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
	m_level->m_messages->showMessage("You are dead...",MESSAGE_NOTIFICATION);
	m_level->m_playerAlive = false;

    CallbackOverlay winScreen(7,50, "You are dead...", 0, &drawDeathScreen, &handleWinScreen);
    winScreen.main(m_level);
}

void
Player::finish(Level *level)
{
    m_level = level;
}

void
Player::act(float time)
{
    m_time += m_speed*time;
    for(int i=0; i<m_abilities.size();i++){
        Ability *ability = m_abilities[i];
        if(ability->m_active)
            ability->act(this, m_level);
    }
    if(m_regainLifeCooldown <= 0){
        m_regainLifeCooldown = 15;
        healDamage(1+m_hd/2,this);
    }
    m_regainLifeCooldown--;
}

Actor *
Player::getTarget(int type)
{
    Actor *target=0;
    std::vector<int> tags;
	m_level->m_messages->showMessage("Choose a target",MESSAGE_ACTION);
    bool done=false;
    int targetX = m_x;
    int targetY = m_y;
    if(type == TARGET_HOSTILE){
        tags.push_back(ACTOR_CREATURE);
        std::vector<Actor *> visibleActors = m_level->getVisibleActors(tags);
        if(visibleActors.size()>0){
            int closestActor=0;
            float closestDist = FLT_MAX;
            for(int i=0;i<visibleActors.size();i++){
                Actor *actor = visibleActors[i];
                float x = m_x - actor->m_x;
                float y = m_y - actor->m_y;
                float dist = x*x + y*y;
                if(dist < closestDist){
                    closestDist = dist;
                    closestActor = i;
                }
            }
            targetX = visibleActors[closestActor]->m_x;
            targetY = visibleActors[closestActor]->m_y;
        }
    }
    while(!done){
        m_level->m_dungeon->clearHilight();
        int tmpX = m_x;
        int tmpY = m_y;
        TCODLine::init (tmpX, tmpY, targetX, targetY);
        do {
            if(m_level->m_dungeon->isWalkable(tmpX,tmpY) && m_level->m_dungeon->isVisible(tmpX,tmpY)){
                m_level->m_dungeon->setHilight(tmpX,tmpY,1);
                std::vector<Actor *> hitActors = m_level->getActors(tmpX,tmpY,tags);
                if(hitActors.size() >0){
                    target = hitActors[0];
                    break;
                }
            }
            else{
                break;
            }
        } while (!TCODLine::step(&tmpX,&tmpY));
        m_level->m_dungeon->setHilight(targetX,targetY,1);
        m_level->render();
        TCODConsole::flush();

        TCOD_key_t key = TCODConsole::waitForKeypress(true);
        if(key.pressed){
            switch(key.c) {
                case 'h' :
                    targetX--; break;
                case 'j' :
                    targetY++; break;
                case 'k' :
                    targetY--; break;
                case 'l' :
                    targetX++; break;
                case 'y' :
                    targetX--;targetY--; break;
                case 'u' :
                    targetX++;targetY--; break;
                case 'b' :
                    targetX--;targetY++; break;
                case 'n' :
                    targetX++;targetY++; break;
                case ENTER :
                    done=true; break;
                case ESC :
                    target=0; done=true; break;
            }
        }
        targetX = std::min(DUNGEON_WIN_W-1, std::max(0, targetX));
        targetY = std::min(DUNGEON_WIN_H-1, std::max(0, targetY));
    }
    m_level->m_dungeon->clearHilight();
    return target;
}

bool
Player::getDirection(int &x, int &y, bool cancelable)
{
    m_level->m_messages->showMessage("Choose a direction (hjklyubn)",MESSAGE_NOTIFICATION);
    m_level->render();
    TCODConsole::flush();
    bool done =false;
    while(!done){
        TCOD_key_t key = TCODConsole::waitForKeypress(true);
        if(key.pressed){
            switch(key.c) {
                case 'h' :
                    x=-1;y=0;done=true;break;
                case 'j' :
                    x= 0;y= 1;done=true;break;
                case 'k' :
                    x= 0;y=-1;done=true;break;
                case 'l' :
                    x= 1;y= 0;done=true;break;
                case 'y' :
                    x=-1;y=-1;done=true;break;
                case 'u' :
                    x= 1;y=-1;done=true;break;
                case 'b' :
                    x=-1;y= 1;done=true;break;
                case 'n' :
                    x= 1;y= 1;done=true;break;
                case ESC :
                    if(cancelable){
                        return false;
                    }
            }
        }
    }
    return true;
}

void
Player::doFire()
{
    if(m_quiver == -1){
        m_level->m_messages->showMessage("You have nothing quivered", MESSAGE_NOTIFICATION);
        return;
    }
    bool ok = false;
    if(m_weapon != -1){
        if(m_inventory[m_weapon]->hasTag(ITEM_RANGED)){
            if(m_quiver != -1 && m_inventory[m_quiver]->hasTag(ITEM_AMMO_BOW)){
                ok = true;
            }
        }
    }
    if(ok){
        Actor *target = getTarget(TARGET_HOSTILE);
        if(target != 0){
            double breakProbability[] = {0.3, 0.7};
            boost::random::discrete_distribution<> breakDist(breakProbability);
            if(true){
                bool done = false;
                std::vector<int> tags;
                tags.push_back(ITEM_STACK);
                std::vector<Actor *> onGround= m_level->getActors(target->m_x,target->m_y,tags);
                for(int i=0;i<onGround.size();i++){
                    Actor *a = onGround[i];
                    if(a->m_name == m_inventory[m_quiver]->m_name){
                        a->m_amount++;
                        done = true;
                    }
                }
                if(!done){
                    Actor *shot;
                    shot = new Actor;
                    *shot = *(m_inventory[m_quiver]);
                    shot->m_amount = 1;
                    shot->m_x = target->m_x;
                    shot->m_y = target->m_y;
                    m_level->m_actors.push_back(shot);
                }
            }

            attack(target,ATTACK_RANGED);
            act();

            m_inventory[m_quiver]->m_amount--;
            if(m_inventory[m_quiver]->m_amount <=0){
                m_quiver = -1;
                Actor *tmp = m_inventory[m_quiver];
                removeFromInventory(tmp);
            }
        }
        else
        {
            m_level->m_messages->showMessage("You change your mind", MESSAGE_FLAVOUR);
        }
    }
    else{
        std::string msg = "You cannot fire ";
        msg += m_inventory[m_quiver]->m_name;
        msg += " with your ";
        if(m_weapon != -1){
            msg += m_inventory[m_weapon]->m_name;
        }
        else{
            msg += "bare hands";
        }
        m_level->m_messages->showMessage(msg, MESSAGE_NOTIFICATION);
    }
}

void
Player::doOpen()
{
    act();
    std::vector<int> tags;
    tags.push_back(ITEM_OPEN);
    std::vector<Actor *> actors = m_level->getActors(m_x, m_y,tags);
    if(actors.size()>0){
        actors[0]->open(this);
    }
    else{
        m_level->m_messages->showMessage("There is nothing here to open", MESSAGE_NOTIFICATION);
    }
}

void
Player::doQuaff()
{
    act();
    std::vector<ListDefinition>itemList;
    bool empty = true;
    for(int i=0;i<m_inventory.size();i++){
        Actor *actor = m_inventory[i];
        if(actor->hasTag(ITEM_QUAFF)){
            std::string st = actor->m_name;
            itemList.push_back(ListDefinition(LIST_ENTRY, actor->m_letter, st));
        }
    }
    ListOverlay itemOverlay(30, 30, "Drink what?", false, true, itemList);
    itemOverlay.main(m_level);
    for(int i=0;i<itemOverlay.m_definition.size();i++){
        ListDefinition &def = itemOverlay.m_definition[i];
        if(def.m_selected){
            Actor *item = getFromInventory(def.m_key);
            if(item != 0){
                std::string msg = "You drink the ";
                msg += item->m_name;
                m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
                item->quaff(this);
                item->m_amount--;
                if(item->m_amount <= 0){
                    removeFromInventory(item);
                    if(item->hasTag(POTION_SACRED_NECTAR)){
                        for(int i=0;i<m_abilities.size();i++){
                            Ability *ab = m_abilities[i];
                            if(ab->m_id == AB_SACRED_NECTAR && ab->m_active){
                                AbSacredNectar *bt = static_cast<AbSacredNectar *>(ab);
                                if(bt->m_nectar == item){
                                    invokeAbility(i);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void
Player::doQuiver()
{
    act();
    std::vector<ListDefinition>itemList;
    bool empty = true;
    for(int i=0;i<m_inventory.size();i++){
        Actor *actor = m_inventory[i];
        if(actor->hasTag(ITEM_QUIVER)){
            std::string st = actor->m_name;
            itemList.push_back(ListDefinition(LIST_ENTRY, actor->m_letter, st));
        }
    }
    ListOverlay itemOverlay(30, 30, "Quiver what?", false, true, itemList);
    itemOverlay.main(m_level);
    for(int i=0;i<itemOverlay.m_definition.size();i++){
        ListDefinition &def = itemOverlay.m_definition[i];
        if(def.m_selected){
            for(int ii=0;ii<m_inventory.size();ii++){
                Actor *item = m_inventory[ii];
                if(item->m_letter == def.m_key){
                    std::string msg = "You quiver the ";
                    msg += item->m_name;
                    m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
                    m_quiver = ii;
                }
            }
        }
    }
}

void
Player::doWield()
{
    act();
    std::vector<ListDefinition>itemList;
    bool empty = true;
    for(int i=0;i<m_inventory.size();i++){
        Actor *actor = m_inventory[i];
        if(actor->hasTag(ITEM_WEAPON)){
            std::string st = actor->m_name;
            itemList.push_back(ListDefinition(LIST_ENTRY, actor->m_letter, st));
        }
    }
    ListOverlay itemOverlay(30, 30, "Wield what?", false, true, itemList);
    itemOverlay.main(m_level);
    for(int i=0;i<itemOverlay.m_definition.size();i++){
        ListDefinition &def = itemOverlay.m_definition[i];
        if(def.m_selected){
            for(int ii=0;ii<m_inventory.size();ii++){
                Actor *item = m_inventory[ii];
                if(item->m_letter == def.m_key){
                    std::string msg = "You wield the ";
                    msg += item->m_name;
                    m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
                    m_weapon = ii;
                }
            }
        }
    }
}

void
Player::doDrop()
{
    bool dropped = false;
    std::vector<ListDefinition>itemList;
    bool empty = true;
    for(int i=0;i<m_inventory.size();i++){
        Actor *actor = m_inventory[i];
        if(!actor->hasTag(ITEM_DONT_DROP)){
            std::string st = actor->m_name;
            itemList.push_back(ListDefinition(LIST_ENTRY, actor->m_letter, st));
        }
    }
    ListOverlay itemOverlay(30, 30, "Drop what?", true, true, itemList);
    itemOverlay.main(m_level);
    for(int i=0;i<itemOverlay.m_definition.size();i++){
        ListDefinition &def = itemOverlay.m_definition[i];
        if(def.m_selected){
            Actor *item = getFromInventory(def.m_key);
            if(item != 0){
                std::string msg = "You drop the ";
                msg += item->m_name;
                m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
                item->m_x = m_x;
                item->m_y = m_y;
                m_level->m_actors.push_back(item);
                removeFromInventory(item);
                dropped = true;
            }
        }
    }
    if(dropped)
        act();
}

void
Player::doPickUp()
{
    bool showNothingMessage = true;
    if(m_level->m_dungeon->getGlyph(m_x,m_y) == 28){
        m_level->m_messages->showMessage("You absorb the shard of knowledge",MESSAGE_NOTIFICATION);
        int manaAmount = m_hd/7 + 1;
        increaseMana(manaAmount);
        int w = m_level->m_dungeon->m_width;
        m_level->m_dungeon->m_tiles[m_x+m_y*w] = m_level->m_tileFactory.getTile("Crystal Floor");
        showNothingMessage = false;
    }
    if(m_level->m_dungeon->getGlyph(m_x,m_y) == 31){
        m_level->m_messages->showMessage("Congratulations",MESSAGE_NOTIFICATION);
        m_level->m_messages->showMessage("You have obtained the Orb of Knowledge",MESSAGE_NOTIFICATION);
        int w = m_level->m_dungeon->m_width;
        m_level->m_dungeon->m_tiles[m_x+m_y*w] = m_level->m_tileFactory.getTile("Crystal Floor");
        showNothingMessage = false;
        m_level->m_playerWon = true;

        CallbackOverlay winScreen(7,50, "You Win!", 0, &drawWinScreen, &handleWinScreen);
        winScreen.main(m_level);
    }
    bool pickedUp = false;
    std::vector<int> tags;
    tags.push_back(ITEM_PICK_UP);
    std::vector<Actor *> actors = m_level->getActors(m_x,m_y,tags);
    if(m_inventory.size()<26){
        if(actors.size()>0){
            showNothingMessage = false;
            std::vector<ListDefinition>itemList;
            bool empty = true;
            char letter = 'a';
            for(int i=0;i<actors.size();i++,letter++){
                Actor *actor = actors[i];
                std::stringstream st;
                st << actor->m_name;
                if(actor->hasTag(ITEM_STACK)){
                    st << " x";
                    st << actor->m_amount;
                }
                itemList.push_back(ListDefinition(LIST_ENTRY, letter, st.str()));
            }
            ListOverlay itemOverlay(30, 30, "Pick up what?", itemList.size()>1, true, itemList);
            itemOverlay.main(m_level);
            for(int i=0;i<itemOverlay.m_definition.size();i++){
                ListDefinition &def = itemOverlay.m_definition[i];
                if(def.m_selected){
                    Actor *item = actors[i];
                    if(item != 0){
                        std::string msg = "You pick up the ";
                        msg += item->m_name;
                        m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
                        pickUp(item);
                        m_level->removeActor(item);
                        pickedUp = true;
                    }
                }
            }
        }
        if(showNothingMessage){
            m_level->m_messages->showMessage("There is nothing here to pick up",MESSAGE_NOTIFICATION);
        }
        if(pickedUp)
            act();
    }
    else{
        if(showNothingMessage){
            m_level->m_messages->showMessage("You cannot carry more things",MESSAGE_NOTIFICATION);
        }
    }
}

void
Player::pickUp(Actor *item)
{
    if(item != 0){
        char itemLetter = item->m_letter;
        std::string itemName = item->m_name;
        bool done = false;
        for(int i=0;i<m_inventory.size();i++){
            Actor *a = m_inventory[i];
            if(a->m_name == item->m_name && item->hasTag(ITEM_STACK)){
                a->m_amount += item->m_amount;
                m_level->removeActor(item);
                delete item;
                done = true;
            }
        }
        if(!done){
            m_inventory.push_back(item);
            bool keepLetter = item->m_letter != 0;
            char freeLetter = 'a';
            bool done=false;
            if(keepLetter){
                for(int i=0;i<m_inventory.size();i++){
                    Actor *a = m_inventory[i];
                    if(a->m_letter == item->m_letter){
                        keepLetter = false;
                    }
                }
            }
            if(!keepLetter){
                while(!done){
                    done = true;
                    for(int i=0;i<m_inventory.size();i++){
                        Actor *a = m_inventory[i];
                        if(a->m_letter == freeLetter){
                            done = false;
                        }
                    }
                    if(!done){
                        freeLetter++;
                    }
                }
                item->m_letter = freeLetter;
            }
            itemLetter = item->m_letter;
        }
        std::string msg = "";
        msg += itemLetter;
        msg += " - ";
        msg += itemName;
        m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
    }
}

void
Player::showInventory()
{
    std::vector<int>tagList;
    std::vector<ListDefinition>categoryList;
    tagList.push_back(ITEM_WEAPON);categoryList.push_back(ListDefinition(LIST_CATEGORY,')',"Weapons"));
    tagList.push_back(ITEM_ARMOR );categoryList.push_back(ListDefinition(LIST_CATEGORY,'[',"Armor"));
    tagList.push_back(ITEM_POTION);categoryList.push_back(ListDefinition(LIST_CATEGORY,'!',"Potions"));
    std::vector<ListDefinition>itemList;
    for(int category=0;category<categoryList.size();category++){
        int tag = tagList[category];
        itemList.push_back(categoryList[category]);
        bool empty = true;
        for(int i=0;i<m_inventory.size();i++){
            Actor *actor = m_inventory[i];
            if(actor->hasTag(tag)){
                std::stringstream st;
                st << actor->m_name;
                if(actor->hasTag(ITEM_STACK)){
                    st << " x";
                    st << actor->m_amount;
                }
                if(m_weapon == i){
                    st << " (in hand)";
                }
                if(m_helmet==i||m_shield==i||m_bodyArmor==i||m_gloves==i||m_boots==i){
                    st << " (worn)";
                }
                if(m_quiver==i){
                    st << " (in quiver)";
                }
                itemList.push_back(ListDefinition(LIST_ENTRY, actor->m_letter, st.str()));
                empty = false;
            }
        }
        if(empty){
            itemList.pop_back();
        }
    }
    ListOverlay itemOverlay(40, 40, "Inventory", false, true, itemList);
    itemOverlay.main(m_level);
}

void
Player::drawNewCard(int slot, bool mana)
{
    Ability *ab;
    double cardProbability[] = { 1.0, 1.5, 1.0};
    boost::random::discrete_distribution<> cardDist(cardProbability);
    boost::random::discrete_distribution<> colorDist(m_maxMana);
    bool done = false;
    while(!done){
        int color = colorDist(RAND);
        int card = cardDist(RAND);
        if(mana)
            card = 2;
        switch(color){
            case 0: // RED
                switch(card){
                    case 0:
                        ab = new AbInstantTunnel(AB_INSTANT_TUNNEL);break;
                    case 1:
                        ab = new AbLightningBolt(AB_LIGHTNING_BOLT);break;
                    case 2:
                        ab = new AbSmallManaStream(AB_SMALL_MANA_STREAM, COLOR_RED);break;
                }
                break;
            case 1: // BLUE
                switch(card){
                    case 0:
                        ab = new AbBlink(AB_BLINK);break;
                    case 1:
                        ab = new AbBlastTrap(AB_BLAST_TRAP,0);break;
                    case 2:
                        ab = new AbSmallManaStream(AB_SMALL_MANA_STREAM, COLOR_BLUE);break;
                }
                break;
            case 2: // WHITE
                switch(card){
                    case 0:
                        ab = new AbSacredNectar(AB_SACRED_NECTAR);break;
                    case 1:
                        ab = new AbDefensiveStance(AB_DEFENSIVE_STANCE);break;
                    case 2:
                        ab = new AbSmallManaStream(AB_SMALL_MANA_STREAM, COLOR_WHITE);break;
                }
                break;
        }
        done = m_maxMana[ab->m_color] > 0;
        if(done){
            for(int i=0;i<ab->m_cost.size();i++){
                ManaCost &cost = ab->m_cost[i];
                if(m_maxMana[cost.m_color] < cost.m_amount){
                    done = false;
                }
            }
        }
        if(!done){
            delete ab;
        }
    }
    if(m_abilities.size() > slot){
        if(m_abilities[slot] != 0){
            delete m_abilities[slot];
        }
        m_abilities[slot] = ab;
    }
    else{
        m_abilities.push_back(ab);
    }

    /*if(slot<m_abilities.size() && slot>=0){
        if(m_library.size()<=0){
            restockLibrary();
        }
        m_abilities[slot] = m_library.back();
        m_library.pop_back();
    }*/
}

void
Player::shuffleLibrary()
{
    shuffler shuf;
    std::random_shuffle(m_library.begin(), m_library.end(), shuf);
}

void
Player::restockLibrary()
{
    m_library.clear();
    for(int i=0;i<m_deck.size();i++){
        bool ok=true;
        Ability *ab = m_deck[i];
        for(int ii=0;ii<3;ii++){
            if(ab == m_abilities[ii]){
                ok=false;
            }
        }
        if(ok){
            m_library.push_back(ab);
        }
    }
    shuffleLibrary();
}

void
Player::levelUp(){
    if(m_hd < 20){
        std::stringstream ss;
        m_hd++;
        m_bab++;
        m_expForNextLevel += m_hd*1000;
        calculateBonuses();
        calculateHp();
        calculateAc();
        ss << "You are now level " << m_hd;
        m_level->m_messages->showMessage(ss.str(),MESSAGE_NOTIFICATION);
    }
}

void
Player::goDown(){
    std::vector<int> tags;
    tags.push_back(ITEM_STAIRS_DOWN);
    std::vector<Actor *> actors = m_level->getActors(m_x,m_y,tags);
    if(actors.size() > 0){
        m_level->descend();
        act();
    }
}

void
Player::goUp(){
    std::vector<int> tags;
    tags.push_back(ITEM_STAIRS_UP);
    std::vector<Actor *> actors = m_level->getActors(m_x,m_y,tags);
    if(actors.size() > 0){
        if(m_level->m_dungeonLevel > 1){
            m_level->ascend();
            act();
        }
        else{
            m_level->m_messages->showMessage("You have not yet retrieved the Orb of Knowledge",MESSAGE_NOTIFICATION);
        }
    }
}

void
Player::gainExp(int amount){
    m_exp += amount;
    while(m_exp >= m_expForNextLevel){
        levelUp();
    }
}

void
Player::invokeAbility(int id)
{
    Ability *ab = m_abilities[id];
    int cost[3] = {0,0,0};
    for(int i=0;i<ab->m_cost.size();i++){
        ManaCost &abCost = ab->m_cost[i];
        switch(abCost.m_color){
            case COLOR_RED:
                cost[0] += abCost.m_amount;
                break;
            case COLOR_BLUE:
                cost[1] += abCost.m_amount;
                break;
            case COLOR_WHITE:
                cost[2] += abCost.m_amount;
                break;
        }
    }
    if(!ab->m_active){
        bool canAfford = true;
        for(int i=0;i<3;i++){
            if(m_mana[i] < cost[i])
                canAfford = false;
        }
        if(canAfford){
            bool cancelled = false;
            bool discard;
            std::string msg = "You invoke ";
            msg += ab->m_name;
            m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
            discard = ab->invoke(this,m_level,cancelled);
            if(!cancelled){
                act();
                for(int i=0;i<3;i++){
                    m_mana[i] -= cost[i];
                }
                if(discard){
                    drawNewCard(id);
                }
                else{
                    ab->m_active = true;
                    for(int i=0;i<3;i++){
                        m_lockedMana[i] += cost[i];
                    }
                }
            }
            else{
                m_level->m_messages->showMessage("Never mind",MESSAGE_FLAVOUR);
            }
        }
        else{
            m_level->m_messages->showMessage("You don't have enough mana",MESSAGE_FLAVOUR);
        }
    }
    else{
        for(int i=0;i<3;i++){
            m_lockedMana[i] -= cost[i];
        }
        ab->deactivate(this,m_level);
        ab->m_active = false;
        drawNewCard(id);
    }
}

Actor *
Player::getFromInventory(char key){
    for(int i=0;i<m_inventory.size();i++){
        Actor *item = m_inventory[i];
        if(item->m_letter == key){
            return item;
        }
    }
    return 0;
}
