#ifndef RWSDK_BAVECTOR_H
#define RWSDK_BAVECTOR_H

#include <rwsdk/rwtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwV2d {
    RwReal x;
    RwReal y;
} RwV2d;

typedef struct RwV3d {
    RwReal x;
    RwReal y;
    RwReal z;
} RwV3d;

void* _rwVectorOpen(void*, RwInt32 offset, RwInt32);
void* _rwVectorClose(void*, RwInt32, RwInt32);
RwReal RwV3dNormalize(RwV3d*, RwV3d*);
RwReal RwV3dLength(RwV3d* vec);
RwReal _rwSqrt(RwReal x);
RwReal _rwInvSqrt(RwReal x);

#ifdef __cplusplus
}
#endif

#endif
