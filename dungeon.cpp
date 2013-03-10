#include "dungeon.h"
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/discrete_distribution.hpp>
#include <cfloat>

Dungeon::Dungeon(int width, int height, TileFactory *tileFactory)
{
	m_width  = width;
	m_height = height;
	m_tiles  = new Tile[m_width * m_height];
	m_fovMap = new TCODMap(m_width, m_height);
	m_tileFactory = tileFactory;
}

void
Dungeon::render()
{
    for(int y = 0; y< DUNGEON_WIN_H; y++){
		for(int x = 0; x< DUNGEON_WIN_W; x++){
			if(y<m_height && x<m_width){
				Tile &tile = m_tiles[x+y*m_width];
				if(tile.m_discovered){
					if(tile.m_visible)
						TCODConsole::root->setDefaultForeground(TCODColor::white);
					else
						TCODConsole::root->setDefaultForeground(TCODColor::grey);
					TCODConsole::root->putChar(x,y,tile.m_glyph);
				}
			}
		}
    }
	TCODConsole::root->setDefaultForeground(TCODColor::white);
}

void
Dungeon::generate()
{
	for(int i=0; i<m_width*m_height;i++){
		int t = T_VOID;
		m_tiles[i] = m_tileFactory->getTile("Stone Floor");
	}
    generateCavern(0, m_height, 0, m_width);
	for(int y=0;y<m_height;y++){
		for(int x=0;x<m_width;x++){
			m_fovMap->setProperties(x,y,m_tiles[x+y*m_width].m_transparent, m_tiles[x+y*m_width].m_walkable);
		}
	}
}

void
Dungeon::computeFov(int playerX, int playerY)
{
	m_fovMap->computeFov(playerX,playerY,30,true,FOV_PERMISSIVE_8);
	for(int y=0;y<m_height;y++){
		for(int x=0;x<m_width;x++){
			if(m_fovMap->isInFov(x,y)){
				m_tiles[x+y*m_width].m_visible = true;
				m_tiles[x+y*m_width].m_discovered = true;
			}
			else{
				m_tiles[x+y*m_width].m_visible = false;
			}
		}
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
	return m_tiles[x+y*m_width].m_walkable;
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

static
char 
cavernEntrance(char *tiles, char axis, char sign, int posY, int posX, int width){
        for(int tmp1=0;tmp1<6;tmp1++){
            for(int tmp2=-tmp1;tmp2<=tmp1;tmp2++){
                int y = axis ? posY + sign*tmp1 : posY + tmp2;
                int x = axis ? posX + tmp2      : posX + sign*tmp1;
                tiles[x + y*width] = 0;
            }
        }
}

void
Dungeon::generateCavern(int miny, int maxy, int minx, int maxx)
{
    int height = maxy - miny;
    int width = maxx - minx;
    char *tiles = new char[width*height];
    char *tmpTiles = new char[width*height];
    double probabilities[] = {0.55, 0.45};
    boost::random::discrete_distribution<> dist(probabilities);
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            tiles[x+y*width] = dist(RAND);
        }
    }
    int numRepetitions = 4;
    cavernEntrance(tiles,0,-1,height/2,width-1,width);
    for(int r=0;r<numRepetitions;r++){
        cellularAutomata(height,width,&tiles,&tmpTiles,cavernFirstPass);
        cavernEntrance(tiles,0,-1,height/2,width-1,width);
    }
    numRepetitions = 3;
    for(int r=0;r<numRepetitions;r++){
        cellularAutomata(height,width,&tiles,&tmpTiles,cavernSecondPass);
        cavernEntrance(tiles,0,-1,height/2,width-1,width);
    }
    connectCaverns(miny,maxy,minx,maxx,tiles);
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            char a = tiles[x+y*width];
            const char* t = "Void";
            switch(a){
                case 0:
                    t = "Stone Floor";
                    break;
                case 1:
                    t = "Rock Wall";
                    break;
                case 2:
                    t = "Water Stream";
                    break;
            }
            m_tiles[minx+x+(miny+y)*m_width] = m_tileFactory->getTile(t);
        }
    }
}

static
void
recursiveConnectCaverns(int width, int height, int y, int x, char *tiles, std::vector<CavernConnectivityPoint> *points)
{
    int val = tiles[x+y*width];
    if(val==0 || val==4){
        if(val==4){
            for(int i=0;i<points->size();i++){
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
Dungeon::connectCaverns(int miny, int maxy, int minx, int maxx, char *tiles)
{
    int width = maxx-minx;
    int height = maxy-miny;
    int numSteps = 500;
    std::vector<CavernConnectivityPoint> points;
    //TODO handle doors
    boost::random::uniform_int_distribution<> yDist(miny,maxy-1);
    boost::random::uniform_int_distribution<> xDist(minx,maxx-1);
    for(int i=0;i<numSteps;i++){
        CavernConnectivityPoint point;
        point.y = yDist(RAND);
        point.x = xDist(RAND);
        point.tag = -1;
        if(tiles[point.x+point.y*width] == 0){
            tiles[point.x+point.y*width] = 4;
            points.push_back(point);
        }
    }
    //TODO handle case where all points hit walls
    CavernConnectivityPoint *point = &(points[0]);
    bool done = false;
    while(!done){
        recursiveConnectCaverns(width,height,point->y,point->x,tiles,&points);
        float closestDist = FLT_MAX;
        int   closestPoint = -1;
        int   originPoint;
        for(int i=0;i<points.size();i++){
            if(points[i].tag == 1){
                for(int ii=0;ii<points.size();ii++){
                    if(points[ii].tag == -1){
                        float y = points[ii].y - points[i].y;
                        float x = points[ii].x - points[i].x;
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
            drawLine(op.y,cp.y,op.x,cp.x,width,tiles,setFloor);
            cp.tag = 1;
            point = &cp;
        }
        else{
            done = true;
        }
    }
    for(int i=0;i<width*height;i++){
        if(tiles[i]>1){
            tiles[i] = 0;
        }
		else{
			tiles[i] = 1;
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
            if(x>0 && x<width-1 && y>0 && y<height-1){
                for(int yy=-1;yy<=1;yy++){
                    for(int xx=-1;xx<=1;xx++){
                        if((*tiles)[xx+x+(yy+y)*width] == 1){
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
    }
    char *tmp = *tmpTiles;
    *tmpTiles = *tiles;
    *tiles = tmp;
}
