#include "spider.h"

DEC_CONS(SpiderDecoration)
{
    double weights[] = {
		1.00f,
        0.20f,
    };
    boost::random::discrete_distribution<> dist(weights);
    m_type = dist(RAND);
}

DEC_VAL(SpiderDecoration)
{
	return validateTile(0, 0, tiles);
}

DEC_RES(SpiderDecoration)
{
	reserveTile(0,0, tiles);
}

DEC_REN(SpiderDecoration)
{
    switch(m_type){
		case 0:
			setTile(0,0,"Web", tiles, tileFactory);
			break;
        case 1:
			setTile(0,0,"Spider Egg", tiles, tileFactory);
			break;
    }
}
