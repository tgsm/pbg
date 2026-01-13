#ifndef RWSDK_BACAMERA_H
#define RWSDK_BACAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rwsdk/baraster.h"
#include "rwsdk/batypehf.h"
#include "rwsdk/plcore/bamatrix.h"

enum RwCameraProjection {
    rwNACAMERAPROJECTION = 0,
    rwPERSPECTIVE = 1,
    rwPARALLEL = 2,
    rwCAMERAPROJECTIONFORCEENUMSIZEINT = 0x7FFFFFFF,
};

typedef struct RwCamera {
    RwObjectHasFrame object;
    RwCameraProjection projectionType;
    char unk18[0x20 - 0x18];
    RwMatrix viewMatrix;
    RwRaster* frameBuffer;
    RwRaster* zBuffer;
    RwV2d viewWindow;
    RwV2d recipViewWindow;
    RwV2d viewOffset;
    float nearPlane;
    float farPlane;
    float fogPlane;
    float zScale;
    float zShift;
    char unk94[0x198 - 0x94];
} RwCamera;

RwCamera* RwCameraSetNearClipPlane(float nearPlane, RwCamera* camera);
RwCamera* RwCameraSetFarClipPlane(float farPlane, RwCamera* camera);
RwCamera* RwCameraSetViewOffset(RwCamera* camera, RwV2d* viewOffset);
RwCamera* RwCameraSetProjection(RwCamera* camera, RwCameraProjection projectionType);
RwCamera* RwCameraSetViewWindow(RwCamera* camera, RwV2d* viewWindow);

#ifdef __cplusplus
}
#endif

#endif
