#ifndef FLOOD_6CNWXTJP
#define FLOOD_6CNWXTJP

#include "../decoration.h"
DEC_DEF(FloodDecoration)
    public:
    ~FloodDecoration();
	void recursiveFlood(int recursionDepth, int x, int y, std::string &name, std::string &wallName, float frequency, Tile *tiles, TileFactory *tileFactory);
    bool *m_visited;
};

#endif /* end of include guard: FLOOD_6CNWXTJP */
