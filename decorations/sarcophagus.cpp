#include "sarcophagus.h"
#include "../level.h"

DEC_CONS(SarcophagusDecoration)
{
}

DEC_VAL(SarcophagusDecoration)
{
	return validateTile(0, 0, tiles);
}

DEC_RES(SarcophagusDecoration)
{
	reserveTile(0,0, tiles);
}

DEC_REN(SarcophagusDecoration)
{
	//setTile(0,0,"Sarcophagus", tiles, tileFactory);
	Actor *actor = level->m_actorFactory.getActor("Sarcophagus",level);
	if(actor != 0){
		actor->m_x = m_x;
		actor->m_y = m_y;
		level->m_actors.push_back(actor);
	}
}
