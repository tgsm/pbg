#pragma once

#include "types.h"

class CDKW_V2d {
public:
    CDKW_V2d();
    CDKW_V2d(const CDKW_V2d& other);
    CDKW_V2d(f32 x, f32 y);

    CDKW_V2d operator-(const CDKW_V2d& other) const;
    f32 operator|(const CDKW_V2d& other) const;
    f32 operator^(const CDKW_V2d& other) const;
    void operator=(const CDKW_V2d& other);

    static const CDKW_V2d ZERO;
    static const CDKW_V2d XAXIS;
    static const CDKW_V2d YAXIS;
    static const CDKW_V2d ONE;

    static f32 sm_Epsilon;

private:
    f32 m_x, m_y;
};
