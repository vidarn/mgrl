#include "sarcophagus.h"

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
	setTile(0,0,"Sarcophagus", tiles, tileFactory);
}
