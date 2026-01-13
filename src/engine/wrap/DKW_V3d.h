#ifndef ENGINE_WRAP_DKW_V3D_H
#define ENGINE_WRAP_DKW_V3D_H

#include <rwsdk/plcore/bavector.h>
#include "types.h"

// Used for operator-(const CDKW_V3d&)
#pragma push
#pragma supress_warnings on

class CDKW_Matrix;

// TODO
class CDKW_V3d : public RwV3d {
public:
    CDKW_V3d() {}
    explicit CDKW_V3d(const RwV3d& rwvec) {
        x = rwvec.x;
        y = rwvec.y;
        z = rwvec.z;
    }
    CDKW_V3d(F32 x, F32 y, F32 z);
    CDKW_V3d(const CDKW_V3d& other) {
        *this = other;
    }

    // FIXME: Doesn't match yet
    CDKW_V3d operator+(const CDKW_V3d& other) const {
        CDKW_V3d result;
        volatile F32 z, y, x;
        x = result.x = (this->x + other.x);
        y = result.y = (this->y + other.y);
        z = result.z = (this->z + other.z);
        return result;
    }

    CDKW_V3d operator-() const;
    CDKW_V3d& operator-(const CDKW_V3d& other) const {
        CDKW_V3d result;
        result.x = this->x - other.x;
        result.y = this->y - other.y;
        result.z = this->z - other.z;
        return result;
    }

    CDKW_V3d operator*(const F32& scalar) const;
    CDKW_V3d operator/(const F32& scalar) const;

    CDKW_V3d& operator+=(const CDKW_V3d& rhs);
    CDKW_V3d& operator-=(const CDKW_V3d& rhs);
    CDKW_V3d& operator*=(const F32 scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    CDKW_V3d& operator*=(const CDKW_Matrix& matrix);
    CDKW_V3d& operator+=(const F32& scalar);

    CDKW_V3d Normalized() const {
        CDKW_V3d normalized = *this;
        normalized.Normalize();
        return normalized;
    }

    void Normalize() {
        if (!((x - CDKW_V3d::ZERO.x) * (x - CDKW_V3d::ZERO.x) +
            (y - CDKW_V3d::ZERO.y) * (y - CDKW_V3d::ZERO.y) +
            (z - CDKW_V3d::ZERO.z) * (z - CDKW_V3d::ZERO.z) <= CDKW_V3d::sm_Epsilon * CDKW_V3d::sm_Epsilon)) {
            RwV3dNormalize(this, this);
        }
    }

    F32 SquareDistToPoint(CDKW_V3d& other);

    static CDKW_V3d ZERO;
    static CDKW_V3d XAXIS;
    static CDKW_V3d YAXIS;
    static CDKW_V3d ZAXIS;
    static CDKW_V3d ONE;
    static F32 sm_Epsilon;
};
// Used for operator-(const CDKW_V3d&)
#pragma supress_warnings reset

REQUIRE_SIZE(CDKW_V3d, 0xC);

#endif
