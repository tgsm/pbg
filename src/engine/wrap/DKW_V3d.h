#ifndef ENGINE_WRAP_DKW_V3D_H
#define ENGINE_WRAP_DKW_V3D_H

#include "types.h"

// TODO
class CDKW_V3d {
public:
    CDKW_V3d() {}
    CDKW_V3d(f32 x, f32 y, f32 z);

public:
    f32 m_x, m_y, m_z;
};
REQUIRE_SIZE(CDKW_V3d, 0xC);

#endif
