#ifndef RWSDK_DRIVER_GCN_DL2DREND_H
#define RWSDK_DRIVER_GCN_DL2DREND_H

#include <rwsdk/badevice.h>

#ifdef __cplusplus
extern "C" {
#endif

extern RwBool _rwDlIm2DRenderLine(struct rwGameCube2DVertex* vertices, RwInt32, RwInt32 a2, RwInt32 a3);
extern RwBool _rwDlIm2DRenderTriangle(struct rwGameCube2DVertex* vertices, RwInt32, RwInt32 a2, RwInt32 a3, RwInt32 a4);
extern RwBool _rwDlIm2DRenderPrimitive(RwPrimitiveType type, struct rwGameCube2DVertex* vertices, RwInt32 numVertices);
extern RwBool _rwDlIm2DRenderIndexedPrimitive(RwPrimitiveType type, struct rwGameCube2DVertex* vertices, RwInt32 numVertices, RwUInt16* indices, RwInt32 numIndices);

#ifdef __cplusplus
}
#endif

#endif
