#ifndef ENGINE_WRAP_DKW_V3D_H
#define ENGINE_WRAP_DKW_V3D_H

#include "types.h"

// TODO
class CDKW_V3d {
public:
    CDKW_V3d() {}
    CDKW_V3d(F32 x, F32 y, F32 z);

    F32 SquareDistToPoint(CDKW_V3d& other);

public:
    F32 m_x, m_y, m_z;
};
REQUIRE_SIZE(CDKW_V3d, 0xC);

#endif
