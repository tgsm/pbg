#ifndef ENGINE_WRAP_DKW_QUAT_H
#define ENGINE_WRAP_DKW_QUAT_H

#include "engine/wrap/DKW_V3d.h"

class CDKW_Quat {
public:
    CDKW_Quat() {
        m_abc.m_x = m_abc.m_y = m_abc.m_z = 0.0f;
        m_d = 1.0f;
    }
    CDKW_Quat(F32 a, F32 b, F32 c, F32 d);
    CDKW_Quat(const CDKW_V3d& abc, F32 d);
    CDKW_Quat(const CDKW_Quat& other);

    CDKW_Quat operator*(const CDKW_Quat& other) const;

public:
    CDKW_V3d m_abc;
    F32 m_d;

    static CDKW_Quat ZERO;
};

#endif
