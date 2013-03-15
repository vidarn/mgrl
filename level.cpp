#include "level.h"
#include <cfloat>

static void drawMessages(TCODConsole *console, void *data, int width, int height);

static void drawStatus(TCODConsole *console, void *data, int width, int height);

Level::Level(int dungeonWidth, int dungeonHeight, int statusWinWidth, int statusWinHeight,
                int messagesWinWidth, int messagesWinHeight)
{
	m_dungeon = new Dungeon(dungeonWidth, dungeonHeight, &m_tileFactory);
	m_fovMap = new TCODMap(dungeonWidth, dungeonHeight);
	m_dungeonFovMap = new TCODMap(dungeonWidth, dungeonHeight);
	m_player = new Player();
	m_messages = new MessageHandler(m_player);
    m_player->m_x = DUNGEON_WIN_W-1;
    m_player->m_y = (DUNGEON_WIN_H)/2;
    m_player->finish(this);
	m_playerAlive = true;
	m_pathFinder = 0;
    m_statusWin = new CallbackOverlay(5,5,"Status",m_player, &drawStatus, 0);
    m_statusWin->setPos(DUNGEON_WIN_W+1,1);
    m_statusWin->setSize(statusWinWidth-1,statusWinHeight-1);
    m_messagesWin = new CallbackOverlay(messagesWinHeight,messagesWinWidth,"Messages",m_messages, &drawMessages, 0);
    m_messagesWin->setPos(0,DUNGEON_WIN_H+1);
}

void
Level::render()
{
    TCODConsole::root->setDefaultForeground(TCODColor::white);
    m_statusWin->render();
    m_messagesWin->render();
	m_dungeon->render();
	for(int i=0;i<m_actors.size();i++){
		Actor &actor = *(m_actors[i]);
        bool visible = false;
		if(m_fovMap->isInFov(actor.m_x, actor.m_y))
            visible = true;
        if(visible || (actor.m_discovered && actor.hasTag(TAG_REMEMBER))){
			actor.render(m_dungeon->isHilighted(actor.m_x,actor.m_y,1),visible);
		}
		else{
			if(DEBUG){
                actor.render(m_dungeon->isHilighted(actor.m_x,actor.m_y,1),false);
			}
		}
	}
    if(m_playerAlive)
        m_player->render(m_dungeon->isHilighted(m_player->m_x,m_player->m_y,1),true);
}

void
Level::generate()
{
    //TODO free actors
    m_actors.clear();
	m_dungeon->generate(this);
	int w = m_dungeon->m_width;
	int h = m_dungeon->m_height;
	for(int y=0;y<h;y++){
		for(int x=0;x<w;x++){
			m_dungeonFovMap->setProperties(x,y,m_dungeon->isTransparent(x,y), m_dungeon->isWalkable(x,y));
		}
	}
}

void
Level::computeFov(int playerX, int playerY)
{
	int w = m_dungeon->m_width;
	int h = m_dungeon->m_height;
	m_fovMap->computeFov(playerX,playerY,0,true,FOV_PERMISSIVE_8);
	for(int y=0;y<h;y++){
		for(int x=0;x<w;x++){
			if(m_fovMap->isInFov(x,y)){
				m_dungeon->setVisible(x,y,true);
				m_dungeon->setDiscovered(x,y,true);
			}
			else{
				m_dungeon->setVisible(x,y,false);
			}
		}
	}
	for(int i=0;i<m_actors.size();i++){
		Actor &actor = *(m_actors[i]);
		if(m_fovMap->isInFov(actor.m_x, actor.m_y)){
			actor.playerSpotted();
		}
	}
}

void
Level::updatePathFinder(int playerX, int playerY)
{
}

void
Level::update()
{
	m_fovMap->copy(m_dungeonFovMap);
	computeFov(m_player->m_x, m_player->m_y);
	for(int i=0;i<m_actors.size();i++){
		m_fovMap->setProperties(m_actors[i]->m_x,m_actors[i]->m_y,true,false);
	}
	for(int i=0;i<m_actors.size();i++){
		m_actors[i]->act();
	}
}

void
Level::walkTowardsPlayer(int *creatureX, int *creatureY)
{
	int x = *creatureX;
	int y = *creatureY;
	m_fovMap->setProperties(x,y,true,true);
	TCODPath *path = new TCODPath(m_fovMap);
	bool success = path->compute(x,y,m_player->m_x,m_player->m_y);
	if(!success){
		delete path;
		path = new TCODPath(m_dungeonFovMap);
		path->compute(x,y,m_player->m_x,m_player->m_y);
	}
	path->walk(creatureX,creatureY,false);
	if(*creatureX==m_player->m_x && *creatureY == m_player->m_y){
		*creatureX = x;
		*creatureY = y;
	}
	if(!m_fovMap->isWalkable(*creatureX,*creatureY)){
		*creatureX = x;
		*creatureY = y;
	}
	m_fovMap->setProperties(*creatureX,*creatureY,true,false);
	delete path;
}

void
Level::killActor(Actor *victim, Actor *killer)
{
	for(int i=0;i<m_actors.size();i++){
		Actor *actor = m_actors[i];
		if(actor == victim){
            if(!actor->hasTag(TAG_PLAYER)){
                m_actors.erase(m_actors.begin()+i);
            }
		}
	}
	victim->die(killer);
    if(!victim->hasTag(TAG_PLAYER)){
        delete victim;
    }
}

std::vector<Actor *>
Level::getVisibleActors(std::vector<int> tags)
{
	std::vector<Actor *> actors;
	for(int i=0;i<m_actors.size();i++){
		Actor *actor = m_actors[i];
		if(m_fovMap->isInFov(actor->m_x, actor->m_y)){
            bool ok = true;
            for(int ii=0;ii<tags.size();ii++){
                if(!actor->hasTag(tags[ii])){
                    ok=false;
                }
            }
            if(ok){
                actors.push_back(actor);
            }
		}
	}
	return actors;
}

std::vector<Actor *>
Level::getActors(int x, int y, std::vector<int> tags)
{
    std::vector<Actor *> actors;
	for(int i=0;i<m_actors.size();i++){
        bool ok=true;
		Actor *actor = m_actors[i];
		if(actor->m_x==x && actor->m_y==y){
            for(int ii=0;ii<tags.size();ii++){
                if(!actor->hasTag(tags[ii])){
                    ok=false;
                }
            }
            if(ok){
                actors.push_back(actor);
            }
		}
	}
    return actors;
}

void
drawMessages(TCODConsole *console, void *data, int width, int height)
{
    MessageHandler *messages = static_cast<MessageHandler *>(data);
    for(int y = 0; y<7; y++){
        console->print(0, 7-y, " %s", messages->getNthLatestMessage(y).m_message.c_str());
    }
}

void
drawStatus(TCODConsole *console, void *data, int width, int height)
{
    Player *player = static_cast<Player *>(data);
	console->setDefaultForeground(TCODColor::white);
    int health = 16.0f * float(player->m_hp)/float(player->m_maxHp);
    if(health < 16)
        console->setDefaultForeground(TCODColor::darkGreen);
    if(health < 13)
        console->setDefaultForeground(TCODColor::yellow);
    if(health < 7)
        console->setDefaultForeground(TCODColor::orange);
    if(health < 3)
        console->setDefaultForeground(TCODColor::red);
    console->print(0 ,0,"HP: %d/%d",player->m_hp,player->m_maxHp);
    console->print(0 ,1," [                ]");
    for(int i=0;i<16;i++){
        if(i<health){
            console->putChar(2+i,1,'+');
        }
        else{
            console->putChar(2+i,1,'.');
        }
    }
	console->setDefaultForeground(TCODColor::white);
    console->print(0 ,3,"STR:%2d",player->m_str);
    console->print(7 ,3,"DEX:%2d",player->m_dex);
    console->print(14,3,"CON:%2d",player->m_con);
    console->print(0 ,4,"INT:%2d",player->m_int);
    console->print(7 ,4,"WIS:%2d",player->m_wis);
    console->print(14,4,"CHA:%2d",player->m_cha);

    ManaCost manaColors[] = {
        ManaCost(0,COLOR_RED),
        ManaCost(0,COLOR_BLUE),
        ManaCost(0,COLOR_WHITE)
    };
    for(int i=0;i<3;i++){
        ManaCost &col = manaColors[i];
        console->setDefaultForeground(col.m_col);
        console->print(i*7,6,"%2d/%2d%c",player->m_mana[i],player->m_maxMana[i],col.m_char);
    }

    for(int i=0;i<player->m_abilities.size();i++){
        Ability *ability = player->m_abilities[i];
        if(ability->m_active)
            console->setDefaultForeground(TCODColor::grey);
        else
            console->setDefaultForeground(TCODColor::white);
        console->print(0,8+3*i,"%s",ability->m_name.c_str());
        for(int ii=0;ii<ability->m_cost.size();ii++){
            ManaCost &cost = ability->m_cost[ii];
            console->setDefaultForeground(cost.m_col);
            console->print(ii*4,9+3*i,"%2d%c",cost.m_amount,cost.m_char);
        }
    }
}
