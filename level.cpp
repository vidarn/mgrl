#include "level.h"

Level::Level(int dungeonWidth, int dungeonHeight)
{
	m_dungeon = new Dungeon(dungeonWidth, dungeonHeight, &m_tileFactory, &m_actors);
	m_fovMap = new TCODMap(dungeonWidth, dungeonHeight);
	m_player = new Player(this);
    m_player->m_glyph = '@';
    m_player->m_x = DUNGEON_WIN_W-1;
    m_player->m_y = (DUNGEON_WIN_H)/2;
}

void
Level::render()
{
	m_dungeon->render();
	for(int i=0;i<m_actors.size();i++){
		m_actors[i]->render();
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
			m_fovMap->setProperties(x,y,m_dungeon->isTransparent(x,y), m_dungeon->isWalkable(x,y));
		}
	}
	m_pathFinder = new TCODDijkstra(m_fovMap);
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
}

void
Level::updatePathFinder(int playerX, int playerY)
{
	m_pathFinder->compute(playerX,playerY);
}

void
Level::update()
{
	for(int i=0;i<m_actors.size();i++){
		m_actors[i]->walkTowardsPlayer();
	}
	computeFov(m_player->m_x, m_player->m_y);
	updatePathFinder(m_player->m_x, m_player->m_y);
}

void
Level::walkTowardsPlayer(int *creatureX, int *creatureY)
{
	m_pathFinder->setPath(*creatureX,*creatureY);
	m_pathFinder->reverse();
	m_pathFinder->walk(creatureX,creatureY);
}

