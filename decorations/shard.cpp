#include "shard.h"
#include "../level.h"

DEC_CONS(ShardDecoration)
{
}

DEC_VAL(ShardDecoration)
{
	return validateTile(0, 0, tiles);
}

DEC_RES(ShardDecoration)
{
	reserveTile(0,0, tiles);
}

DEC_REN(ShardDecoration)
{
	Actor *actor;
    actor = level->m_actorFactory.getActor("Shard of Knowledge",level);
	if(actor != 0){
		actor->m_x = m_x;
		actor->m_y = m_y;
		level->m_actors.push_back(actor);
	}
}
