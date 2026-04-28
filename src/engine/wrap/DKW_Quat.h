#ifndef ENGINE_WRAP_DKW_QUAT_H
#define ENGINE_WRAP_DKW_QUAT_H

#include <rwsdk/tool/quat/rtquat.h>
#include "engine/wrap/DKW_V3d.h"

class CDKW_Quat : public RtQuat {
public:
    CDKW_Quat() {
        imag.x = imag.y = imag.z = 0.0f;
        real = 1.0f;
    }
    CDKW_Quat(F32 a, F32 b, F32 c, F32 d);
    CDKW_Quat(const CDKW_V3d& abc, F32 d);
    CDKW_Quat(const CDKW_Quat& other);

    CDKW_Quat operator*(const CDKW_Quat& other) const;

    static CDKW_Quat ZERO;
};

#endif
