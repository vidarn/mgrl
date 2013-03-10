#include "level.h"
#include <cfloat>

Level::Level(int dungeonWidth, int dungeonHeight)
{
	m_dungeon = new Dungeon(dungeonWidth, dungeonHeight, &m_tileFactory, &m_actors);
	m_fovMap = new TCODMap(dungeonWidth, dungeonHeight);
	m_dungeonFovMap = new TCODMap(dungeonWidth, dungeonHeight);
	m_messages = new MessageHandler();
	m_player = new Player(this);
    m_player->m_glyph = '@';
    m_player->m_x = DUNGEON_WIN_W-1;
    m_player->m_y = (DUNGEON_WIN_H)/2;
	m_pathFinder = 0;
}

void
Level::render()
{
	m_dungeon->render();
	for(int i=0;i<m_actors.size();i++){
		Actor *actor = m_actors[i];
		if(m_fovMap->isInFov(actor->m_x, actor->m_y)){
			actor->render();
		}
		else{
			if(DEBUG){
				actor->render();
			}
		}
	}
	m_player->render();
}

void
Level::generate()
{
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
		Actor *actor = m_actors[i];
		if(m_fovMap->isInFov(actor->m_x, actor->m_y)){
			actor->playerSpotted();
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

