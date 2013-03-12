#include "pillar.h"

DEC_CONS(PillarDecoration)
{
}

DEC_VAL(PillarDecoration)
{
    return validateTile(0, 0, tiles);
}

DEC_RES(PillarDecoration)
{
    reserveTile(0,0, tiles);
}

DEC_REN(PillarDecoration)
{
    setTile(0,0,"Pillar", tiles, tileFactory);
}
