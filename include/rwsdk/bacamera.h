#ifndef RWSDK_BACAMERA_H
#define RWSDK_BACAMERA_H

#include <rwsdk/babbox.h>
#include <rwsdk/baimage.h>
#include <rwsdk/baraster.h>
#include <rwsdk/batypehf.h>
#include <rwsdk/plcore/bamatrix.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum RwCameraProjection {
    rwNACAMERAPROJECTION = 0,
    rwPERSPECTIVE = 1,
    rwPARALLEL = 2,
    rwCAMERAPROJECTIONFORCEENUMSIZEINT = 0x7FFFFFFF,
} RwCameraProjection;

typedef struct RwCamera* (* RwCameraBeginUpdateFunc)(struct RwCamera*);
typedef struct RwCamera* (* RwCameraEndUpdateFunc)(struct RwCamera*);

typedef struct RwPlane {
    RwV3d normal;
    RwReal distance;
} RwPlane;

typedef struct RwFrustumPlane {
    RwPlane plane;
    RwUInt8 closestX;
    RwUInt8 closestY;
    RwUInt8 closestZ;
    RwUInt8 pad;
} RwFrustumPlane;

// FIXME: Find a home for this.
typedef struct RwSphere {
    RwV3d center;
    RwReal radius;
} RwSphere;

typedef struct RwCamera {
    RwObjectHasFrame object;
    RwCameraProjection projectionType;
    RwCameraBeginUpdateFunc beginUpdate;
    RwCameraEndUpdateFunc endUpdate;
    RwMatrix viewMatrix;
    RwRaster* frameBuffer;
    RwRaster* zBuffer;
    RwV2d viewWindow;
    RwV2d recipViewWindow;
    RwV2d viewOffset;
    RwReal nearPlane;
    RwReal farPlane;
    RwReal fogPlane;
    RwReal zScale;
    RwReal zShift;
    RwUInt8 unk94[4];
    RwFrustumPlane frustumPlanes[6];
    RwBBox frustumBoundBox;
    RwV3d frustumCorners[8];
} RwCamera; // size: 0x188

void* _rwCameraOpen(void*, RwInt32 offset, RwInt32);
void* _rwCameraClose(void*, RwInt32, RwInt32);
RwCamera* RwCameraSetViewOffset(RwCamera* camera, RwV2d* viewOffset);
RwCamera* RwCameraSetNearClipPlane(RwCamera* camera, RwReal nearPlane);
RwCamera* RwCameraSetFarClipPlane(RwCamera* camera, RwReal farPlane);
RwCamera* RwCameraClear(RwCamera* camera, RwRGBA* clear_color, RwInt32);
RwCamera* RwCameraSetProjection(RwCamera* camera, RwCameraProjection projectionType);
RwCamera* RwCameraSetViewWindow(RwCamera* camera, RwV2d* viewWindow);

#ifdef __cplusplus
}
#endif

#endif
