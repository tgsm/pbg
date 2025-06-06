#ifndef ENGINE_WRAP_DKW_V2D_H
#define ENGINE_WRAP_DKW_V2D_H

#include "types.h"

class CDKW_V2d {
public:
    CDKW_V2d();
    CDKW_V2d(const CDKW_V2d& other);
    CDKW_V2d(F32 x, F32 y);

    CDKW_V2d operator-(const CDKW_V2d& other) const;
    F32 operator|(const CDKW_V2d& other) const;
    F32 operator^(const CDKW_V2d& other) const;
    void operator=(const CDKW_V2d& other);

    static const CDKW_V2d ZERO;
    static const CDKW_V2d XAXIS;
    static const CDKW_V2d YAXIS;
    static const CDKW_V2d ONE;

    static F32 sm_Epsilon;

private:
    F32 m_x, m_y;
};
REQUIRE_SIZE(CDKW_V2d, 0x8);

#endif
