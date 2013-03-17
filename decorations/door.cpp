#include "door.h"
#include "../level.h"

DEC_CONS(DoorDecoration, bool open)
{
    m_open = open;
}

DEC_VAL(DoorDecoration)
{
	return validateTile(0, 0, tiles);
}

DEC_RES(DoorDecoration)
{
	reserveTile(0,0, tiles);
    return true;
}

DEC_REN(DoorDecoration)
{
	Actor *actor;
    if(m_open)
        actor = level->m_actorFactory.getActor("Open Door",level);
    else
        actor = level->m_actorFactory.getActor("Door",level);
	if(actor != 0){
		actor->m_x = m_x;
		actor->m_y = m_y;
		level->m_actors.push_back(actor);
	}
}
