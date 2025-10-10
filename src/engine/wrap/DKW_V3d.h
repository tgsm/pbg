#ifndef ENGINE_WRAP_DKW_V3D_H
#define ENGINE_WRAP_DKW_V3D_H

#include <rwsdk/plcore/bavector.h>
#include "types.h"

// Used for operator-(const CDKW_V3d&)
#pragma push
#pragma supress_warnings on

// TODO
class CDKW_V3d {
public:
    CDKW_V3d() {}
    CDKW_V3d(const RwV3d& rwvec) {
        m_x = rwvec.x;
        m_y = rwvec.y;
        m_z = rwvec.z;
    }
    CDKW_V3d(F32 x, F32 y, F32 z);

    CDKW_V3d operator-() const;
    CDKW_V3d& operator-(const CDKW_V3d& other) const {
        CDKW_V3d result;
        result.m_x = this->m_x - other.m_x;
        result.m_y = this->m_y - other.m_y;
        result.m_z = this->m_z - other.m_z;
        return result;
    }

    F32 SquareDistToPoint(CDKW_V3d& other);

    static CDKW_V3d ZERO;
    static CDKW_V3d XAXIS;
    static CDKW_V3d YAXIS;
    static CDKW_V3d ZAXIS;
    static CDKW_V3d ONE;
    static F32 sm_Epsilon;

public:
    F32 m_x, m_y, m_z;
};
// Used for operator-(const CDKW_V3d&)
#pragma supress_warnings reset

REQUIRE_SIZE(CDKW_V3d, 0xC);

#endif
