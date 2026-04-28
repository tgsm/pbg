#ifndef RWSDK_TOOL_QUAT_RTQUAT_H
#define RWSDK_TOOL_QUAT_RTQUAT_H

#include "rwsdk/plcore/bavector.h"

#ifdef __cplusplus
extern "C" {
#endif

struct RtQuat {
    RwV3d imag;
    float real;
};

RtQuat* RtQuatRotate(RtQuat*, RwV3d*, float, int);

#ifdef __cplusplus
}
#endif

#endif
