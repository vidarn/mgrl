#ifndef FLOOD_6CNWXTJP
#define FLOOD_6CNWXTJP

#include "../decoration.h"
DEC_DEF(FloodDecoration)
	void recursiveFlood(int recursionDepth, int x, int y, std::string &name, float frequency, Tile *tiles, TileFactory *tileFactory);
};

#endif /* end of include guard: FLOOD_6CNWXTJP */
