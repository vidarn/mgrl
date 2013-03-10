#include <iostream>
#include <libtcod/libtcod.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <vector>
#include <ctime>
#include "callback_overlay.h"
#include "list_overlay.h"
#include "level.h"
#include "actor.h"
#include "player.h"
#include "tile.h"

int SCREEN_W = 100;
int SCREEN_H = 40;
int DUNGEON_WIN_W;
int DUNGEON_WIN_H;
boost::random::mt19937 RAND;

void
drawInventory(TCODConsole *console, void *data, int width, int height)
{
    for(int y = 0; y< height; y++){
        for(int x = 0; x< width; x++){
            console->putChar(x,y,' ');
        }
    }
}

void
drawStatus(TCODConsole *console, void *data, int width, int height)
{
    Actor *player = static_cast<Actor *>(data);
    console->print(0 ,0,"HP: %d/%d",player->m_hp,player->m_maxHp);
    console->print(0 ,1," [                ]");
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
    for(int i=0;i<16;i++){
        if(i<health){
            console->putChar(2+i,1,'+');
        }
        else{
            console->putChar(2+i,1,'.');
        }
    }
	console->setDefaultForeground(TCODColor::white);
    console->print(0 ,3,"STR:%2d",4);
    console->print(7 ,3,"CON:%2d",14);
    console->print(14,3,"DEX:%2d",14);
    console->print(0 ,4,"INT:%2d",4);
    console->print(7 ,4,"WIS:%2d",14);
    console->print(14,4,"CHA:%2d",14);
}

bool
handleInventoryInput(char key, void *data)
{
    if(key == 'q'){
        return false;
    }
    return true;
}

int
main(int argc, char **argv)
{
    time_t tim;
    time(&tim);
    RAND.seed(tim);
    TCODConsole::initRoot(SCREEN_W,SCREEN_H,"libtcod",false);

    int messagesWinHeight = 8;
    int messagesWinWidth    = SCREEN_W;
    int statusWinHeight   = SCREEN_H - messagesWinHeight;
    int statusWinWidth    = 21;
    DUNGEON_WIN_H = statusWinHeight;
    DUNGEON_WIN_W = SCREEN_W - statusWinWidth;

    Level level(DUNGEON_WIN_W,DUNGEON_WIN_H);
    level.generate();


    std::vector<Overlay*> overlays;
    Overlay *statusWin = new CallbackOverlay(5,5,"Status",&(level.m_player), &drawStatus, &handleInventoryInput);
    statusWin->setPos(DUNGEON_WIN_W+1,1);
    statusWin->setSize(statusWinWidth-1,statusWinHeight-1);
    Overlay *messagesWin = new CallbackOverlay(messagesWinHeight,messagesWinWidth,"Messages",NULL, &drawInventory, &handleInventoryInput);
    messagesWin->setPos(0,DUNGEON_WIN_H+1);
    level.update();

    while ( !TCODConsole::isWindowClosed() ) {
        TCODConsole::root->clear();
        level.render();
        statusWin->render();
        messagesWin->render();
        for(int i=0;i<overlays.size();i++){
            overlays[i]->render();
        }
        TCODConsole::flush();
        if(level.m_player->m_running){
            level.m_player->run();
            level.update();
        }
        else{
            TCOD_key_t key = TCODConsole::waitForKeypress(true);
            if(key.pressed){
                if(overlays.size() > 0){
                    if(!overlays.back()->handleInput(key.c)){
                        delete overlays.back();
                        overlays.pop_back();
                    }
                }
                else{
                    bool update = true;
                    switch(key.c) {
                        case 'q' :
                            return 0;
                        case 'h' :
                            level.m_player->move(-1,0); break;
                        case 'j' :
                            level.m_player->move(0,1); break;
                        case 'k' :
                            level.m_player->move(0,-1); break;
                        case 'l' :
                            level.m_player->move(1,0); break;
                        case 'y' :
                            level.m_player->move(-1,-1); break;
                        case 'u' :
                            level.m_player->move(1,-1); break;
                        case 'b' :
                            level.m_player->move(-1,1); break;
                        case 'n' :
                            level.m_player->move(1,1); break;
                        case 'H' :
                            level.m_player->startRun(-1,0); break;
                        case 'J' :
                            level.m_player->startRun(0,1); break;
                        case 'K' :
                            level.m_player->startRun(0,-1); break;
                        case 'L' :
                            level.m_player->startRun(1,0); break;
                        case 'Y' :
                            level.m_player->startRun(-1,-1); break;
                        case 'U' :
                            level.m_player->startRun(1,-1); break;
                        case 'B' :
                            level.m_player->startRun(-1,1); break;
                        case 'N' :
                            level.m_player->startRun(1,1); break;
                        case 'e' :
                            level.m_player->m_hp--; break;
                        case 'i':
                            if(overlays.size() < 1){
                                ListDefinition inventoryList[] = {
                                    { LIST_CATEGORY,  { "/","Weapons"}},
                                    { LIST_ENTRY,     { "a","Short Sword"}},
                                    { LIST_ENTRY,     { "b","War Axe"}},
                                    { LIST_CATEGORY,  { "[","Armour"}},
                                    { LIST_ENTRY,     { "c","Iron Chestplate"}},
                                    { LIST_ENTRY,     { "d","Iron Chestplate"}},
                                    { LIST_ENTRY,     { "e","Iron Chestplate"}},
                                    { LIST_ENTRY,     { "f","Iron Chestplate"}},
                                    { LIST_ENTRY,     { "g","Iron Chestplate"}},
                                    { LIST_ENTRY,     { "h","Iron Chestplate"}},
                                    { LIST_ENTRY,     { "i","Iron Chestplate"}},
                                    { LIST_CATEGORY,  { "~","Misc"}},
                                    { LIST_ENTRY,     { "a","Short Sword"}},
                                    { LIST_ENTRY,     { "a","Short Sword"}},
                                    { LIST_ENTRY,     { "a","Short Sword"}},
                                    { LIST_ENTRY,     { "b","War Axe"}},
                                    { LIST_ENTRY,     { "b","War Axe"}},
                                    { LIST_ENTRY,     { "b","War Axe"}},
                                    { LIST_ENTRY,     { "a","Short Sword"}},
                                    { LIST_ENTRY,     { "b","War Axe"}},
                                    { LIST_ENTRY,     { "a","Short Sword"}},
                                    { LIST_ENTRY,     { "b","War Axe"}},
                                    { LIST_ENTRY,     { "a","Short Sword"}},
                                    { LIST_ENTRY,     { "b","War Axe"}},
                                    { LIST_ENTRY,     { "f","Short Sword"}},
                                    { LIST_ENTRY,     { "k","War Axe"}},
                                    { LIST_ENTRY,     { "h","Iron Chestplate"}},
                                    { LIST_DONE,NULL}
                                };
                                overlays.push_back(new ListOverlay(30, 30, "Inventory", inventoryList));
                                overlays.push_back(new CallbackOverlay(2, 60, "What is your name?", NULL, &drawInventory, &handleInventoryInput));
                            }
                            break;
                        case 'r':
                            level.generate();
                            break;
                        default:
                            std::cout << "FALSE!\n";
                            update=false;
                            break;
                    }
                    if(update)
                        level.update();
                }
            }
        }
    }
}
