#ifndef RWSDK_BACAMERA_H
#define RWSDK_BACAMERA_H

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

typedef struct RwCamera {
    RwObjectHasFrame object;
    RwCameraProjection projectionType;
    RwUInt8 unk18[0x20 - 0x18];
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
    RwUInt8 unk94[0x188 - 0x94];
} RwCamera; // size: 0x188

void* _rwCameraOpen(void*, RwInt32 offset, RwInt32);
void* _rwCameraClose(void*, RwInt32, RwInt32);
RwCamera* RwCameraSetViewOffset(RwCamera* camera, RwV2d* viewOffset);
RwCamera* RwCameraSetNearClipPlane(RwReal nearPlane, RwCamera* camera);
RwCamera* RwCameraSetFarClipPlane(RwReal farPlane, RwCamera* camera);
RwCamera* RwCameraClear(RwCamera* camera, RwRGBA* clear_color, RwInt32);
RwCamera* RwCameraSetProjection(RwCamera* camera, RwCameraProjection projectionType);
RwCamera* RwCameraSetViewWindow(RwCamera* camera, RwV2d* viewWindow);

#ifdef __cplusplus
}
#endif

#endif
