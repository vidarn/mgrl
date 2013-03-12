#ifndef SQUARE_KDMDMOY7
#define SQUARE_KDMDMOY7
#include "../room.h"

enum{
    ROOM_SQUARE_SMALL,
    ROOM_SQUARE_MEDIUM,
    ROOM_SQUARE_LARGE,
    ROOM_SQUARE_HUGE,
};

ROOM_DEF(SquareRoom, int size)
    int m_w, m_h;
};

#endif /* end of include guard: SQUARE_KDMDMOY7 */
