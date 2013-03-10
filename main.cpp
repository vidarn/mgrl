#include <iostream>
#include <libtcod/libtcod.hpp>
#include <ncurses.h>
#include <vector>
#include "callback_overlay.h"
#include "list_overlay.h"
#include "dungeon.h"
#include <boost/random/mersenne_twister.hpp>

int SCREEN_W = 100;
int SCREEN_H = 40;
int DUNGEON_WIN_W;
int DUNGEON_WIN_H;
boost::random::mt19937 RAND;

void
drawStatusWin(WINDOW* statusWin, int statusWinHeight, int statusWinWidth)
{
    for(int i = 0; i< statusWinHeight; i++){
        mvwaddch(statusWin,i,0,'|');
    }
    wrefresh(statusWin);
}

void
drawMessagesWin(WINDOW* messagesWin, int messagesWinHeight, int messagesWinWidth)
{
    for(int i = 0; i< messagesWinWidth; i++){
        mvwaddch(messagesWin,0,i,'-');
    }
    wrefresh(messagesWin);
}

void
drawInventory(WINDOW *window, void *data, int width, int height)
{
    for(int i = 0; i< width*height; i++){
        waddch(window,'+');
    }
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
    //TCODConsole::setCustomFont("font.png",TCOD_FONT_LAYOUT_ASCII_INROW);
    TCODConsole::initRoot(SCREEN_W,SCREEN_H,"libtcod",false);
    TCODConsole::root->clear();
    TCODConsole::root->putChar(1,1,'@');
    TCODConsole::root->putChar(2,2,'d');
    TCODConsole::flush();

    int messagesWinHeight = 8;
    int messagesWinWidth    = SCREEN_W;
    int statusWinHeight   = SCREEN_H - messagesWinHeight;
    int statusWinWidth    = 20;
    DUNGEON_WIN_H = statusWinHeight;
    DUNGEON_WIN_W = SCREEN_W - statusWinWidth;

    Dungeon dungeon(DUNGEON_WIN_W,DUNGEON_WIN_H);
    dungeon.generate();
    std::vector<Overlay*> overlays;

    while ( !TCODConsole::isWindowClosed() ) {
        dungeon.render();
        //drawStatusWin(  statusWin   , statusWinHeight   , statusWinWidth);
        //drawMessagesWin(messagesWin , messagesWinHeight , messagesWinWidth);
        for(int i=0;i<overlays.size();i++){
            overlays[i]->render();
        }
        TCODConsole::flush();
        TCOD_key_t key = TCODConsole::waitForKeypress(true);
        if(key.pressed){
            if(overlays.size() > 0){
                if(!overlays.back()->handleInput(key.c)){
                    delete overlays.back();
                    overlays.pop_back();
                }
            }
            else{
                switch(key.c) {
                    case 'q' :
                        return 0;
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
                        dungeon.generate();
                        break;
                    default:break;
                }
            }
        }
    }
    /*initscr();
    start_color();
    noecho();
    refresh();
    getmaxyx(stdscr,SCREEN_H,SCREEN_W);
    WINDOW *dungeonWin, *messagesWin, *statusWin;
    int messagesWinHeight = 8;
    int messagesWinWidth    = SCREEN_W;
    int statusWinHeight   = SCREEN_H - messagesWinHeight;
    int statusWinWidth    = 40;
    DUNGEON_WIN_H = statusWinHeight;
    DUNGEON_WIN_W = SCREEN_W - statusWinWidth;
    statusWin = newwin(statusWinHeight, statusWinWidth, 0, DUNGEON_WIN_W);
    messagesWin = newwin(messagesWinHeight, messagesWinWidth, DUNGEON_WIN_H, 0);
    std::vector<Overlay*> overlays;
    Dungeon dungeon(40,200);
    dungeon.generate();
    char key = ' ';
    while(1){
        if(overlays.size() > 0){
            if(!overlays.back()->handleInput(key)){
                delete overlays.back();
                overlays.pop_back();
            }
        }
        else{
            switch(key){
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
                    dungeon.generate();
                    break;
                case 'q':
                    endwin();
                    return 0;
            }
        }
        dungeon.render();
        drawStatusWin(  statusWin   , statusWinHeight   , statusWinWidth);
        drawMessagesWin(messagesWin , messagesWinHeight , messagesWinWidth);
        for(int i=0;i<overlays.size();i++){
            overlays[i]->render();
        }
        move(SCREEN_H-1,SCREEN_W-1);
        key = getch();
    }*/
}
