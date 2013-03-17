#include "stairs.h"
#include "../level.h"

DEC_CONS(StairsDecoration, bool down)
{
    m_down = down;
}

DEC_VAL(StairsDecoration)
{
	return validateTile(0, 0, tiles);
}

DEC_RES(StairsDecoration)
{
	reserveTile(0,0, tiles);
    return true;
}

DEC_REN(StairsDecoration)
{
	Actor *actor;
    if(m_down)
        actor = level->m_actorFactory.getActor("Stairs Down",level);
    else
        actor = level->m_actorFactory.getActor("Stairs Up",level);
	if(actor != 0){
		actor->m_x = m_x;
		actor->m_y = m_y;
		level->m_actors.push_back(actor);
	}
}
