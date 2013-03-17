#include "dinner.h"

DEC_CONS(DinnerDecoration)
{
    double weights[] = {
		0.30f,
        0.50f,
        0.60f,
        0.20f,
        0.10f,
    };
    boost::random::discrete_distribution<> dist(weights);
    m_type = dist(RAND);
}

DEC_VAL(DinnerDecoration)
{
    switch(m_type){
        case 0:
			if(validateTile(-1,0,tiles) && validateTile(0,0,tiles) && validateTile(1,0,tiles))
				return true;
			else
				m_type = 1;
		default:
			return validateTile(0, 0, tiles);
    }
	return false;
}

DEC_RES(DinnerDecoration)
{
    switch(m_type){
		case 0:
			reserveTile(-1,0, tiles);
			reserveTile(0,0, tiles);
			reserveTile(1,0, tiles);
			break;
		default:
			reserveTile(0,0, tiles);
			break;
    }
    return true;
}

DEC_REN(DinnerDecoration)
{
    switch(m_type){
		case 0:
			setTile(-1,0,"Right Facing Chair", tiles, tileFactory);
			setTile(0,0,"Table", tiles, tileFactory);
			setTile(1,0,"Left Facing Chair", tiles, tileFactory);
			break;
        case 1:
			setTile(0,0,"Left Facing Chair", tiles, tileFactory);
			break;
        case 2:
			setTile(0,0,"Right Facing Chair", tiles, tileFactory);
			break;
        case 3:
			setTile(0,0,"Cupboard", tiles, tileFactory);
			break;
        case 4:
			setTile(0,0,"Stove", tiles, tileFactory);
			break;
    }
}
