#include <iostream>
#include <libtcod/libtcod.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <vector>
#include <ctime>
#include "level.h"
#include "actor.h"
#include "player.h"
#include "tile.h"

int SCREEN_W = 100;
int SCREEN_H = 40;
int DUNGEON_WIN_W;
int DUNGEON_WIN_H;
bool DEBUG = false;
boost::random::mt19937 RAND;
boost::random::uniform_int_distribution<> d100(0,100);
boost::random::uniform_int_distribution<> d20(0,20);
boost::random::uniform_int_distribution<> d12(0,12);
boost::random::uniform_int_distribution<> d10(0,10);
boost::random::uniform_int_distribution<> d8(0,8);
boost::random::uniform_int_distribution<> d6(0,6);
boost::random::uniform_int_distribution<> d4(0,4);
boost::random::uniform_int_distribution<> d3(0,3);


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

    Level level(DUNGEON_WIN_W,DUNGEON_WIN_H,statusWinWidth,statusWinHeight
            ,messagesWinWidth,messagesWinHeight);
    level.generate();
    level.m_player->customize();


    level.update();

    while ( !TCODConsole::isWindowClosed() ) {
        TCODConsole::root->clear();
        level.render();
        TCODConsole::flush();
        if(level.m_player->m_running){
            level.m_player->run();
            level.update();
        }
        else{
            TCOD_key_t key = TCODConsole::waitForKeypress(true);
            if(key.pressed){
                if(!level.m_playerAlive){
                    return 0;
                }
                else{
                    switch(key.c) {
                        case 'S' :
                            return 0;
                        case 'q' :
                            level.m_player->doQuaff();break;
                        case 'h' :
                            level.m_player->walk(-1,0); break;
                        case 'j' :
                            level.m_player->walk(0,1); break;
                        case 'k' :
                            level.m_player->walk(0,-1); break;
                        case 'l' :
                            level.m_player->walk(1,0); break;
                        case 'y' :
                            level.m_player->walk(-1,-1); break;
                        case 'u' :
                            level.m_player->walk(1,-1); break;
                        case 'b' :
                            level.m_player->walk(-1,1); break;
                        case 'n' :
                            level.m_player->walk(1,1); break;
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
                        case '-' :
                            level.m_player->m_hp--; break;
                        case '+' :
                            level.m_player->m_hp++; break;
                        case 'z' :
                            level.m_player->gainExp(300); break;
                        case 'i':
                            level.m_player->showInventory();
                            break;
                        /*case 'r':
                            level.generate();
                            break;*/
                        case '.':
                            level.m_player->act();
                            break;
                        case 'g':
                            level.m_player->doPickUp();
                            break;
                        case 'f':
                            level.m_player->doFire();
                            break;
                        case 'o':
                            level.m_player->doOpen();
                            break;
                        case 'Q':
                            level.m_player->doQuiver();
                            break;
                        case 'w':
                            level.m_player->doWield();
                            break;
                        case 'd':
                            level.m_player->doDrop();
                            break;
                        case '1':
                            level.m_player->invokeAbility(0);
                            break;
                        case '2':
                            level.m_player->invokeAbility(1);
                            break;
                        case '3':
                            level.m_player->invokeAbility(2);
                            break;
                        case '4':
                            level.m_player->invokeAbility(3);
                            break;
                        case '>':
                            level.m_player->goDown();
                            break;
                        case '<':
                            level.m_player->goUp();
                            break;
                        /*case 'D':
                            DEBUG = !DEBUG;
                            if(DEBUG)
                                level.m_messages->showMessage("DEBUG MODE ON",MESSAGE_WARNING);
                            else
                                level.m_messages->showMessage("DEBUG MODE OFF",MESSAGE_WARNING);
                            break;*/
                        default:
                            break;
                    }
                    if(level.m_playerWon){
                        return 0;
                    }
                    if(level.m_player->m_time > 0.0f){
                        level.update();
                    }
                }
            }
        }
    }
}
