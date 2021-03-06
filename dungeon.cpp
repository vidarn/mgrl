#include "boost/random/uniform_int_distribution.hpp"
#include "boost/random/discrete_distribution.hpp"
#include <cfloat>
#include "dungeon.h"
#include "level.h"
#include "decorations/flood.h"
#include "rooms/stairs.h"
#include "rooms/shard.h"

Dungeon::Dungeon(int width, int height, TileFactory *tileFactory)
{
	m_width  = width;
	m_height = height;
	m_tiles  = new Tile[m_width * m_height];
	m_hilight= new char[m_width * m_height];
	m_tileFactory = tileFactory;
    clearHilight();
}

void
Dungeon::render()
{
    for(int y = 0; y< DUNGEON_WIN_H; y++){
		for(int x = 0; x< DUNGEON_WIN_W; x++){
			if(y<m_height && x<m_width){
				Tile &tile = m_tiles[x+y*m_width];
				if(tile.m_discovered){
                    TCODColor col = tile.m_color * TCODColor::lighterGrey;
                    if(m_hilight[x+y*m_width] == 1){
                        TCODConsole::root->setCharBackground(x,y,TCODColor::grey);
                    }
                    else{
                        TCODConsole::root->setCharBackground(x,y,TCODColor::black);
                    }
					if(!tile.m_visible)
						col = col * TCODColor::grey;
                    if(tile.m_glyph == 31){
                        col = TCODColor::white;
                    }
                    TCODConsole::root->setDefaultForeground(col);
					TCODConsole::root->putChar(x,y,tile.m_glyph);
				}
                if(DEBUG){
                    TCODColor col = tile.m_color * TCODColor::lighterGrey;
                    if(m_hilight[x+y*m_width] == 1){
                        TCODConsole::root->setCharBackground(x,y,TCODColor::grey);
                    }
                    else{
                        TCODConsole::root->setCharBackground(x,y,TCODColor::black);
                    }
					if(!tile.m_visible)
						col = col * TCODColor::grey;
                    TCODConsole::root->setDefaultForeground(col);
                    TCODConsole::root->putChar(x,y,tile.m_glyph);
                    if(y ==0){
						TCODConsole::root->setDefaultForeground(TCODColor::grey);
						TCODConsole::root->putChar(x,y,'0'+x%10);
                    }
                    if(x ==0){
						TCODConsole::root->setDefaultForeground(TCODColor::grey);
						TCODConsole::root->putChar(x,y,'0'+y%10);
                    }
                }
			}
		}
    }
	TCODConsole::root->setDefaultForeground(TCODColor::white);
}

void
Dungeon::generate(Level *level)
{
	for(int i=0; i<m_width*m_height;i++){
		m_tiles[i] = m_tileFactory->getTile("Stone Floor");
	}
    generateCavern(0, m_height, 0, m_width,level);
    bool placedUber = false;
    for(int i=0;i<4;i++){
        for(int ii=0;ii<80;ii++){
            boost::random::uniform_int_distribution<> xDist(1,DUNGEON_WIN_W-2);
            boost::random::uniform_int_distribution<> yDist(1,DUNGEON_WIN_H-2);
            int x = xDist(RAND);
            int y = yDist(RAND);
            if(level->isWalkable(x,y)){
                std::vector<std::string> tags;
                int itemLevel = level->m_dungeonLevel;
                if(!placedUber){
                    itemLevel += 3;
                    placedUber = true;
                }
                Actor *actor = level->m_actorFactory.getItem(itemLevel,level,tags);
                if(actor != 0){
                    actor->m_x = x;
                    actor->m_y = y;
                    level->m_actors.push_back(actor);
                    ii=200;
                }
            }
        }
    }
    int numExtraEnemies = 10;
    if(level->m_dungeonLevel==15)
        numExtraEnemies = 150;
    for(int i=0;i<numExtraEnemies;i++){
        for(int ii=0;ii<80;ii++){
            boost::random::uniform_int_distribution<> xDist(1,DUNGEON_WIN_W-2);
            boost::random::uniform_int_distribution<> yDist(1,DUNGEON_WIN_H-2);
            int x = xDist(RAND);
            int y = yDist(RAND);
            if(level->isWalkable(x,y)){
                std::vector<std::string> tags;
                int hd = 1+level->m_dungeonLevel/4;
                if(level->m_dungeonLevel == 1){
                    hd = 0;
                }
                Actor *actor = level->m_actorFactory.getCreature(hd,level,tags);
                if(actor != 0){
                    actor->m_x = x;
                    actor->m_y = y;
                    level->m_actors.push_back(actor);
                    ii=200;
                }
            }
        }
    }
}

void
Dungeon::drawPath(int y0, int y1, int x0, int x1, int width, char *tiles, void (*tileCallback)(char *))
{
	TCODMap *map = new TCODMap(m_width, m_height);
    for(int y=0;y<m_height;y++){
        for(int x=0;x<m_width;x++){
            map->setProperties(x,y,true,tiles[x+y*width] != 127);
        }
    }
	TCODPath *path = new TCODPath(map,0.0f);
	bool success = path->compute(x0,y0,x1,y1);
    if(success){
        int x;
        int y;
        while(!path->isEmpty()){
            path->walk(&x,&y,false);
            tileCallback(&(tiles[x+y*width]));
        }
    }
    else{
        drawLine(y0,y1,x0,x1,width,tiles,tileCallback);
    }
}


void
Dungeon::drawLine(int y0, int y1, int x0, int x1, int width, char *tiles, void (*tileCallback)(char *))
{
    bool steep = abs(y1-y0) > abs(x1-x0);
    int tmp;
    if(steep){
        tmp = x0; x0 = y0; y0 = tmp;
        tmp = x1; x1 = y1; y1 = tmp;
    }
    if(x0>x1){
        tmp = x0; x0 = x1; x1 = tmp;
        tmp = y0; y0 = y1; y1 = tmp;
    }
    int deltaY = abs(y1-y0);
    int deltaX = x1-x0;
    int error = deltaX/2;
    int ystep = -1;
    int y = y0;
    if(y0<y1)
        ystep = 1;
    for(int x=x0;x<x1;x++){
        if(steep)
            tileCallback(&(tiles[y+x*width]));
        else
            tileCallback(&(tiles[x+y*width]));
        error -= deltaY;
        if(error < 0){
            y+=ystep;
            error += deltaX;
        }
    }
}

bool
Dungeon::isWalkable(int x, int y)
{
    if(x<0 || y<0 || x>m_width-1 || y>m_height-1)
        return false;
	return m_tiles[x+y*m_width].m_walkable;
}

bool
Dungeon::isTransparent(int x, int y)
{
	return m_tiles[x+y*m_width].m_transparent;
}


char
Dungeon::getGlyph(int x, int y)
{
	return m_tiles[x+y*m_width].m_glyph;
}

void
Dungeon::setVisible(int x, int y, bool state)
{
	m_tiles[x+y*m_width].m_visible = state;
}

bool
Dungeon::isVisible(int x, int y)
{
	return m_tiles[x+y*m_width].m_visible;
}

void
Dungeon::setDiscovered(int x, int y, bool state)
{
	m_tiles[x+y*m_width].m_discovered = state;
}

void
Dungeon::clearHilight()
{
    for(int i=0;i<m_width*m_height;i++){
        m_hilight[i] = 0;
    }
}

void
Dungeon::setHilight(int x, int y, char val)
{
    m_hilight[x+y*m_width] = val;
}

bool
Dungeon::isHilighted(int x, int y, char val)
{
    return m_hilight[x+y*m_width] == val;
}

static
char
cavernFirstPass(int numWalls)
{
    return numWalls >= 5 || numWalls < 1 ? 1: 0;
}

static
char
cavernSecondPass(int numWalls)
{
    return numWalls >= 5 ? 1: 0;
}

static
void
setFloor(char *tile)
{
    *tile = 0;
}

void
Dungeon::generateCavern(int miny, int maxy, int minx, int maxx, Level *level)
{
    int height = maxy - miny;
    int width = maxx - minx;
    int numRooms = 20;
    char *tiles = new char[width*height];
    char *tmpTiles = new char[width*height];
    double probabilities[] = {0.55, 0.45};
    probabilities[1] = 0.2;
    if(level->m_dungeonLevel > 9){
        probabilities[1] = 0.0;
        numRooms = 40;
    }
    if(level->m_dungeonLevel < 3){
        probabilities[1] = 0.6;
    }
    boost::random::discrete_distribution<> dist(probabilities);
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            tiles[x+y*width] = dist(RAND);
        }
    }
    for(unsigned int i=0;i<m_rooms.size();i++){
        delete m_rooms[i];
    }
    m_rooms.clear();
    std::vector<CavernConnectivityPoint> points;
    int roomID = 0;

    Room *stairsRoom = new StairsRoom(tiles, width, height);
    stairsRoom->reserve(tiles);
    stairsRoom->validate(tiles,points,roomID);
    roomID++;

    Room *shardRoom  = new ShardRoom(tiles, width, height, level->m_dungeonLevel==15);
    shardRoom->reserve(tiles);
    shardRoom->validate(tiles,points,roomID);
    roomID++;

    if(level->m_dungeonLevel==15){
        numRooms = 0;
    }

    for(int i=0;i<numRooms;i++){
        Room *room = Room::getRoom(0,tiles,width,height);
        if(room->validate(tiles,points,roomID)){
            room->reserve(tiles);
            m_rooms.push_back(room);
            roomID++;
        }
        else{
            delete room;
        }
    }

    m_rooms.push_back(shardRoom);
    m_rooms.push_back(stairsRoom);

    if(level->m_dungeonLevel < 14){
        int numRepetitions = 4;
        for(int r=0;r<numRepetitions;r++){
            cellularAutomata(height,width,&tiles,&tmpTiles,cavernFirstPass);
        }
        numRepetitions = 3;
        for(int r=0;r<numRepetitions;r++){
            cellularAutomata(height,width,&tiles,&tmpTiles,cavernSecondPass);
        }
    }
    else{
        for(int y=0;y<height;y++){
            for(int x=0;x<width;x++){
                tiles[x+y*width] = 0;
            }
        }
    }
    connectCaverns(miny,maxy,minx,maxx,tiles,points);
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            char c = tiles[x+y*width];
            switch(c){
                case 126:
                    c = 0;
                    break;
                case 127:
                    c = 1;
                    break;
            }
            tiles[x+y*width] = c;
        }
    }
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            char a = tiles[x+y*width];
            const char* t = "Void";
            switch(a){
                case 0:
                    t = "Stone Floor";
                    if(level->m_dungeonLevel > 10)
                        t = "Dungeon Floor";
                    break;
                case 1:
                    t = "Rock Wall";
                    if(level->m_dungeonLevel > 10)
                        t = "Dungeon Wall";
                    break;
                case 2:
                    t = "Water Stream";
                    break;
                case 127:
                    t = "Void";
                    break;
                case 126:
                    t = "Water Stream";
                    break;
            }
            m_tiles[minx+x+(miny+y)*m_width] = m_tileFactory->getTile(t);
        }
    }
    roomsRender(m_tiles,m_tileFactory);
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            tiles[x+y*width] = m_tiles[minx+x+(miny+y)*m_width].m_walkable? 0 : 1;
        }
    }
    for(unsigned int i=0;i<points.size();i++){
        if(points[i].type == CON_DUN){
            FloodDecoration fd(points[i].x, points[i].y, m_width, m_height);
            fd.render(m_tiles,m_tileFactory,level);
        }
    }
    roomsDecorate(tiles, m_tiles,m_tileFactory,level);
}

static
void
recursiveConnectCaverns(int width, int height, int y, int x, char *tiles, std::vector<CavernConnectivityPoint> *points)
{
    int val = tiles[x+y*width];
    if(val==0 || val==4 || val == 126){
        if(val==4){
            for(unsigned int i=0;i<points->size();i++){
                if((*points)[i].x == x && (*points)[i].y == y){
                    (*points)[i].tag = 1;
                }
            }
        }
        tiles[x+y*width]=2;
        for(int yy=-1;yy<=1;yy++){
            for(int xx=-1;xx<=1;xx++){
                if((yy!=0 || xx!=0) && x>0 && x<width-1 && y>0 && y<height-1){
                    recursiveConnectCaverns(width,height,y+yy,x+xx,tiles,points);
                }
            }
        }
    }
}

void
Dungeon::connectCaverns(int miny, int maxy, int minx, int maxx, char *tiles, std::vector<CavernConnectivityPoint> &points)
{
    int width = maxx-minx;
    int height = maxy-miny;
    int numSteps = 500;
    boost::random::uniform_int_distribution<> yDist(miny,maxy-1);
    boost::random::uniform_int_distribution<> xDist(minx,maxx-1);
    for(int i=0;i<numSteps;i++){
        CavernConnectivityPoint point;
        point.y = yDist(RAND);
        point.x = xDist(RAND);
        point.tag = -1;
        point.type = CON_CAV;
        if(tiles[point.x+point.y*width] == 0){
            tiles[point.x+point.y*width] = 4;
            points.push_back(point);
        }
    }
    if(points.size() > 0){
        CavernConnectivityPoint *point = &(points[0]);
        point->tag = 1;
        bool done = false;
        while(!done){
            recursiveConnectCaverns(width,height,point->y,point->x,tiles,&points);
            float closestDist = FLT_MAX;
            int   closestPoint = -1;
            int   originPoint;
            for(unsigned int i=0;i<points.size();i++){
                if(points[i].tag == 1){
                    for(unsigned int ii=0;ii<points.size();ii++){
                        CavernConnectivityPoint &tmpPoint = points[ii];
                        if(tmpPoint.tag == -1 && (tmpPoint.roomID != points[i].roomID || tmpPoint.type != CON_DUN)){
                            float y = tmpPoint.y - points[i].y;
                            float x = tmpPoint.x - points[i].x;
                            float dist = y*y + x*x;
                            if(dist < closestDist){
                                closestDist  = dist;
                                closestPoint = ii;
                                originPoint  = i;
                            }
                        }
                    }
                }
            }
            if(closestPoint != -1){
                CavernConnectivityPoint &cp = points[closestPoint];
                CavernConnectivityPoint &op = points[originPoint];
                drawPath(op.y,cp.y,op.x,cp.x,width,tiles,setFloor);
                cp.tag = 1;
                point = &cp;
            }
            else{
                done = true;
            }
        }
        for(int i=0;i<width*height;i++){
            if(tiles[i] < 126){
                if(tiles[i]>1){
                    tiles[i] = 0;
                }
                else{
                    tiles[i] = 1;
                }
            }
        }
    }
}

void
Dungeon::cellularAutomata(int height, int width, char **tiles,
    char **tmpTiles, char (*ruleCallback)(int))
{
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            int numWalls = 0;
            if((*tiles)[x+y*width] < 126){
                if(x>0 && x<width-1 && y>0 && y<height-1){
                    for(int yy=-1;yy<=1;yy++){
                        for(int xx=-1;xx<=1;xx++){
                            char c = (*tiles)[xx+x+(yy+y)*width];
                            if(c != 0 && c != 126){
                                numWalls++;
                            }
                        }
                    }
                    (*tmpTiles)[x+y*width] = ruleCallback(numWalls);
                }
                else{
                    (*tmpTiles)[x+y*width] = 1;
                }
            }
            else{
                (*tmpTiles)[x+y*width] = (*tiles)[x+y*width];
            }
        }
    }
    char *tmp = *tmpTiles;
    *tmpTiles = *tiles;
    *tiles = tmp;
}

void
Dungeon::roomsReserve(char *tiles)
{
    for(unsigned int i=0;i<m_rooms.size();i++){
        m_rooms[i]->reserve(tiles);
    }
}

void
Dungeon::roomsRender(Tile *tiles, TileFactory *tileFactory)
{
    for(unsigned int i=0;i<m_rooms.size();i++){
        m_rooms[i]->render(tiles, tileFactory);
    }
}

void
Dungeon::roomsDecorate(char *charTiles, Tile *tiles, TileFactory *tileFactory, Level *level)
{
    for(unsigned int i=0;i<m_rooms.size();i++){
        m_rooms[i]->decorate(charTiles, tiles, tileFactory, level);
    }
}

