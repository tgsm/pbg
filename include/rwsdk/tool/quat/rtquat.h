#ifndef RWSDK_TOOL_QUAT_RTQUAT_H
#define RWSDK_TOOL_QUAT_RTQUAT_H

#include <rwsdk/plcore/bavector.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RtQuat {
    RwV3d imag;
    RwReal real;
} RtQuat;

RtQuat* RtQuatRotate(RtQuat*, RwV3d*, RwReal, RwInt32);

#ifdef __cplusplus
}
#endif

#endif
