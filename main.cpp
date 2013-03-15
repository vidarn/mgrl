#include <iostream>
#include <libtcod/libtcod.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <vector>
#include <ctime>
#include "callback_overlay.h"
#include "level.h"
#include "actor.h"
#include "player.h"
#include "tile.h"

int SCREEN_W = 100;
int SCREEN_H = 40;
int DUNGEON_WIN_W;
int DUNGEON_WIN_H;
bool DEBUG = true;
boost::random::mt19937 RAND;
boost::random::uniform_int_distribution<> d100(0,100);
boost::random::uniform_int_distribution<> d20(0,20);
boost::random::uniform_int_distribution<> d12(0,12);
boost::random::uniform_int_distribution<> d10(0,10);
boost::random::uniform_int_distribution<> d8(0,8);
boost::random::uniform_int_distribution<> d6(0,6);
boost::random::uniform_int_distribution<> d4(0,4);
boost::random::uniform_int_distribution<> d3(0,3);

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
    for(int i=0;i<player->m_abilities.size();i++){
        Ability *ability = player->m_abilities[i];
        console->setDefaultForeground(TCODColor::white);
        console->print(0,7+3*i,"%s",ability->m_name.c_str());
        for(int ii=0;ii<ability->m_cost.size();ii++){
            ManaCost &cost = ability->m_cost[ii];
            console->setDefaultForeground(cost.m_col);
            console->print(ii*4,8+3*i,"%2d%c",cost.m_amount,cost.m_char);
        }
    }
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


    Overlay *statusWin = new CallbackOverlay(5,5,"Status",level.m_player, &drawStatus, 0);
    statusWin->setPos(DUNGEON_WIN_W+1,1);
    statusWin->setSize(statusWinWidth-1,statusWinHeight-1);
    Overlay *messagesWin = new CallbackOverlay(messagesWinHeight,messagesWinWidth,"Messages",level.m_messages, &drawMessages, 0);
    messagesWin->setPos(0,DUNGEON_WIN_H+1);
    level.update();

    while ( !TCODConsole::isWindowClosed() ) {
        TCODConsole::root->clear();
        TCODConsole::root->setDefaultForeground(TCODColor::white);
        statusWin->render();
        messagesWin->render();
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
                    bool update = true;
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
                            level.m_player->startRun(-1,0);update=false; break;
                        case 'J' :
                            level.m_player->startRun(0,1);update=false; break;
                        case 'K' :
                            level.m_player->startRun(0,-1);update=false; break;
                        case 'L' :
                            level.m_player->startRun(1,0);update=false; break;
                        case 'Y' :
                            level.m_player->startRun(-1,-1);update=false; break;
                        case 'U' :
                            level.m_player->startRun(1,-1);update=false; break;
                        case 'B' :
                            level.m_player->startRun(-1,1);update=false; break;
                        case 'N' :
                            level.m_player->startRun(1,1);update=false; break;
                        case 'e' :
                            level.m_player->m_hp--;update=false; break;
                        case 'w' :
                            level.m_player->m_hp++;update=false; break;
                        case 'i':
                            level.m_player->showInventory();
                            update=false;
                            break;
                        case 'r':
                            level.generate();
                            break;
                        case '.':
                            break;
                        case 'o':
                            level.m_player->doOpen();
                            break;
                        case '1':
                            level.m_player->invokeAbility(0);
                            break;
                        case '2':
                            level.m_player->invokeAbility(1);
                            break;
                        case 'd':
                            DEBUG = !DEBUG;
                            if(DEBUG)
                                level.m_messages->showMessage("DEBUG MODE ON",MESSAGE_WARNING);
                            else
                                level.m_messages->showMessage("DEBUG MODE OFF",MESSAGE_WARNING);
                            update=false;
                            break;
                        default:
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
