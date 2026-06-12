#ifndef RWSDK_DRIVER_GCN_DL2DREND_H
#define RWSDK_DRIVER_GCN_DL2DREND_H

#include <rwsdk/badevice.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int _rwDlIm2DRenderLine(struct rwGameCube2DVertex* vertices, int, int a2, int a3);
extern int _rwDlIm2DRenderTriangle(struct rwGameCube2DVertex* vertices, int, int a2, int a3, int a4);
extern int _rwDlIm2DRenderPrimitive(RwPrimitiveType type, struct rwGameCube2DVertex* vertices, int numVertices);
extern int _rwDlIm2DRenderIndexedPrimitive(RwPrimitiveType type, struct rwGameCube2DVertex* vertices, int numVertices, unsigned short* indices, int numIndices);

#ifdef __cplusplus
}
#endif

#endif
