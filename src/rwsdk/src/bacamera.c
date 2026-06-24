#include <rwsdk/bacamera.h>
#include <rwsdk/badevice.h>
#include <rwsdk/baframe.h>
#include <rwsdk/plcore/baerr.h>
#include <rwsdk/plcore/bamemory.h>
#include <rwsdk/plcore/batkreg.h>
#include <rwsdk/plcore/bavector.h>
#include "dont_inline_hack.h"

static RwFreeList _rwCameraFreeList;

RwPluginRegistry cameraTKList = {
    sizeof(RwCamera),
    sizeof(RwCamera),
    0,
    0,
    NULL,
    NULL,
};

static RwModuleInfo cameraModule;

static RwInt32 _rwCameraFreeListBlockSize = 4;
static RwInt32 _rwCameraFreeListPreallocBlocks = 1;

typedef struct UnkCameraModule {
    RwFreeList* freeList;
} UnkCameraModule;

#define GET_CAMERA_MODULE() \
    ((UnkCameraModule*)((RwInt32)RwEngineInstance + cameraModule.globalsOffset))

extern RwReal RwIm2DGetNearScreenZ(void);
extern RwReal RwIm2DGetFarScreenZ(void);

static RwCamera* CameraUpdateZShiftScale(RwCamera* camera) {
    RwReal near2d = RwIm2DGetNearScreenZ();
    RwReal far2d = RwIm2DGetFarScreenZ();
    RwReal fVar1;
    RwReal fVar2;
    RwReal fVar3;
    RwReal fVar4;
    RwReal fVar5;

    DONT_INLINE_HACK();

    switch (camera->projectionType) {
        case rwPARALLEL:
            fVar3 = camera->farPlane;
            fVar4 = camera->nearPlane;
            break;
        case rwPERSPECTIVE:
        default:
            fVar3 = 1.0f / camera->farPlane;
            fVar4 = 1.0f / camera->nearPlane;
            break;
    }

    fVar2 = far2d - (0.0001f * (far2d - near2d));
    fVar1 = near2d + (0.0001f * (far2d - near2d));
    fVar5 = (fVar2 - fVar1) / (fVar3 - fVar4);
    camera->zScale = fVar5;
    camera->zShift = 0.5f * ((fVar2 + fVar1) - fVar5 * (fVar3 + fVar4));
    return camera;
}

static void CameraBuildPerspClipPlanes(RwCamera* camera);

// Incomplete
static RwCamera* CameraBuildPerspViewMatrix(RwCamera* camera) {
    RwFrame* frame = (RwFrame*)(camera->object.object.parent);
    RwMatrix* ltm = &frame->ltm;
    RwReal fVar2;
    RwReal fVar3;
    RwReal fVar6;
    RwReal fVar7;
    RwReal unk3;
    RwReal unk4;
    RwReal unk1 = -0.5f * camera->recipViewWindow.x;
    RwReal unk2 = 0.5f - (unk1 * camera->viewOffset.x);
    RwReal fVar5 = (ltm->right.y * unk1) + (ltm->at.y * unk2);
    RwReal fVar1 = (ltm->right.z * unk1) + (ltm->at.z * unk2);
    RwReal fVar4 = (ltm->right.x * unk1) + (ltm->at.x * unk2);

    DONT_INLINE_HACK();

    camera->viewMatrix.right.x = fVar4;
    camera->viewMatrix.up.x = fVar5;
    camera->viewMatrix.at.x = fVar1;
    camera->viewMatrix.pos.x = 0.5f - (unk2 + (ltm->pos.x * fVar4 + ltm->pos.y * fVar5 + ltm->pos.z * fVar1));

    fVar2 = -0.5f * camera->recipViewWindow.y;
    fVar1 = fVar2 * ltm->up.x;
    fVar3 = fVar1 + camera->viewOffset.y * fVar1;
    fVar4 = ltm->up.y * fVar2 + ltm->at.x * fVar1;
    fVar7 = ltm->up.z * fVar2 + ltm->at.y * fVar1;
    camera->viewMatrix.right.y = fVar3;
    camera->viewMatrix.up.y = fVar4;
    camera->viewMatrix.at.y = fVar7;
    camera->viewMatrix.pos.y = 0.5f - (fVar1 + (ltm->pos.x * fVar3 + ltm->pos.y * fVar4 + ltm->pos.z * fVar2));

    camera->viewMatrix.right.z = ltm->at.x;
    camera->viewMatrix.up.z = ltm->at.y;
    camera->viewMatrix.at.z = ltm->at.z;
    camera->viewMatrix.pos.z = -(ltm->pos.x * ltm->at.x + ltm->pos.y * ltm->at.y + ltm->pos.z * ltm->at.z);

    RwMatrixOptimize(&camera->viewMatrix, NULL);

    return camera;
}

static void CameraBuildParallelClipPlanes(RwCamera* camera) {
    RwFrame* frame = (RwFrame*)(camera->object.object.parent);
    RwFrustumPlane* planes = camera->frustumPlanes;
    RwV3d* corners = camera->frustumCorners;
    RwReal vwx = camera->viewWindow.x;
    RwReal vwy = camera->viewWindow.y;
    RwReal near = camera->nearPlane;
    RwReal far = camera->farPlane;
    RwReal vox = camera->viewOffset.x;
    RwReal voy = camera->viewOffset.y;
    RwReal invLength;
    RwReal dVar18;
    RwReal dVar17;
    RwReal dVar16;
    RwReal dVar14;
    RwReal dVar15;
    RwReal dVar13;

    corners[3].z = near;
    corners[2].z = near;
    corners[1].z = near;
    corners[0].z = near;
    corners[7].z = far;
    corners[6].z = far;
    corners[5].z = far;
    corners[4].z = far;

    corners[3].x = vwx + ((1.0f - near) * -vox);
    corners[0].x = vwx + ((1.0f - near) * -vox);
    corners[2].x = -vwx + ((1.0f - near) * -vox);
    corners[1].x = -vwx + ((1.0f - near) * -vox);
    corners[7].x = vwx + ((1.0f - far) * -vox);
    corners[4].x = vwx + ((1.0f - far) * -vox);
    corners[6].x = -vwx + ((1.0f - far) * -vox);
    corners[5].x = -vwx + ((1.0f - far) * -vox);
    corners[1].y = vwy + ((1.0f - near) * voy);
    corners[0].y = vwy + ((1.0f - near) * voy);
    corners[3].y = -vwy + ((1.0f - near) * voy);
    corners[2].y = -vwy + ((1.0f - near) * voy);
    corners[5].y = vwy + ((1.0f - far) * voy);
    corners[4].y = vwy + ((1.0f - far) * voy);
    corners[7].y = -vwy + ((1.0f - far) * voy);
    corners[6].y = -vwy + ((1.0f - far) * voy);
    RwV3dTransformPoints(corners, corners, 8, &frame->ltm);

    planes[0].plane.normal = frame->modelling.up;
    planes[0].plane.distance = corners[4].x * planes[0].plane.normal.x +
                               corners[4].y * planes[0].plane.normal.y +
                               corners[4].z * planes[0].plane.normal.z;
    planes[0].closestX = (*(RwInt32*)&planes[0].plane.normal.x >> 31) + 1;
    planes[0].closestY = (*(RwInt32*)&planes[0].plane.normal.y >> 31) + 1;
    planes[0].closestZ = (*(RwInt32*)&planes[0].plane.normal.z >> 31) + 1;

    planes[1].plane.normal.x = -planes[0].plane.normal.x;
    planes[1].plane.normal.y = -planes[0].plane.normal.y;
    planes[1].plane.normal.z = -planes[0].plane.normal.z;
    planes[1].plane.distance = corners[0].x * planes[1].plane.normal.x +
                               corners[0].y * planes[1].plane.normal.y +
                               corners[0].z * planes[1].plane.normal.z;
    planes[1].closestX = (*(RwInt32*)&planes[1].plane.normal.x >> 31) + 1;
    planes[1].closestY = (*(RwInt32*)&planes[1].plane.normal.y >> 31) + 1;
    planes[1].closestZ = (*(RwInt32*)&planes[1].plane.normal.z >> 31) + 1;

    dVar18 = corners[1].x - corners[5].x;
    dVar17 = corners[1].y - corners[5].y;
    dVar16 = corners[1].z - corners[5].z;
    dVar14 = corners[6].x - corners[5].x;
    dVar15 = corners[6].y - corners[5].y;
    dVar13 = corners[6].z - corners[5].z;
    planes[2].plane.normal.x = dVar17 * dVar13 - dVar16 * dVar15;
    planes[2].plane.normal.y = dVar16 * dVar14 - dVar18 * dVar13;
    planes[2].plane.normal.z = dVar18 * dVar15 - dVar17 * dVar14;
    invLength = _rwInvSqrt(planes[2].plane.normal.x * planes[2].plane.normal.x +
                           planes[2].plane.normal.y * planes[2].plane.normal.y +
                           planes[2].plane.normal.z * planes[2].plane.normal.z);
    planes[2].plane.normal.x *= invLength;
    planes[2].plane.normal.y *= invLength;
    planes[2].plane.normal.z *= invLength;
    planes[2].plane.distance = corners[1].x * planes[2].plane.normal.x +
                               corners[1].y * planes[2].plane.normal.y +
                               corners[1].z * planes[2].plane.normal.z;
    planes[2].closestX = (*(RwInt32*)&planes[2].plane.normal.x >> 31) + 1;
    planes[2].closestY = (*(RwInt32*)&planes[2].plane.normal.y >> 31) + 1;
    planes[2].closestZ = (*(RwInt32*)&planes[2].plane.normal.z >> 31) + 1;

    dVar14 = corners[4].x - corners[5].x;
    dVar15 = corners[4].y - corners[5].y;
    dVar13 = corners[4].z - corners[5].z;
    planes[3].plane.normal.x = dVar15 * dVar16 - dVar13 * dVar17;
    planes[3].plane.normal.y = dVar13 * dVar18 - dVar14 * dVar16;
    planes[3].plane.normal.z = dVar14 * dVar17 - dVar15 * dVar18;
    invLength = _rwInvSqrt(planes[3].plane.normal.x * planes[3].plane.normal.x +
                           planes[3].plane.normal.y * planes[3].plane.normal.y +
                           planes[3].plane.normal.z * planes[3].plane.normal.z);
    planes[3].plane.normal.x *= invLength;
    planes[3].plane.normal.y *= invLength;
    planes[3].plane.normal.z *= invLength;
    planes[3].plane.distance = corners[1].x * planes[3].plane.normal.x +
                               corners[1].y * planes[3].plane.normal.y +
                               corners[1].z * planes[3].plane.normal.z;
    planes[3].closestX = (*(RwInt32*)&planes[3].plane.normal.x >> 31) + 1;
    planes[3].closestY = (*(RwInt32*)&planes[3].plane.normal.y >> 31) + 1;
    planes[3].closestZ = (*(RwInt32*)&planes[3].plane.normal.z >> 31) + 1;

    planes[4].plane.normal.x = -planes[2].plane.normal.x;
    planes[4].plane.normal.y = -planes[2].plane.normal.y;
    planes[4].plane.normal.z = -planes[2].plane.normal.z;
    planes[4].plane.distance = corners[3].x * planes[4].plane.normal.x +
                               corners[3].y * planes[4].plane.normal.y +
                               corners[3].z * planes[4].plane.normal.z;
    planes[4].closestX = (*(RwInt32*)&planes[4].plane.normal.x >> 31) + 1;
    planes[4].closestY = (*(RwInt32*)&planes[4].plane.normal.y >> 31) + 1;
    planes[4].closestZ = (*(RwInt32*)&planes[4].plane.normal.z >> 31) + 1;

    planes[5].plane.normal.x = -planes[3].plane.normal.x;
    planes[5].plane.normal.y = -planes[3].plane.normal.y;
    planes[5].plane.normal.z = -planes[3].plane.normal.z;
    planes[5].plane.distance = corners[3].x * planes[5].plane.normal.x +
                               corners[3].y * planes[5].plane.normal.y +
                               corners[3].z * planes[5].plane.normal.z;
    planes[5].closestX = (*(RwInt32*)&planes[5].plane.normal.x >> 31) + 1;
    planes[5].closestY = (*(RwInt32*)&planes[5].plane.normal.y >> 31) + 1;
    planes[5].closestZ = (*(RwInt32*)&planes[5].plane.normal.z >> 31) + 1;
}

// Incomplete
static RwCamera* CameraBuildParallelViewMatrix(RwCamera* camera) {
    RwFrame* frame = (RwFrame*)(camera->object.object.parent);
    RwMatrix* ltm = &frame->ltm;
    RwReal fVar2;
    RwReal fVar3;
    RwReal fVar6;
    RwReal fVar7;
    RwReal unk3;
    RwReal unk4;
    RwReal unk1 = -0.5f * camera->recipViewWindow.x;
    RwReal unk2 = -(unk1 * camera->viewOffset.x);
    RwReal fVar4 = (ltm->right.x * unk1) + (ltm->at.x * unk2);
    RwReal fVar5 = (ltm->right.y * unk1) + (ltm->at.y * unk2);
    RwReal fVar1 = (ltm->right.z * unk1) + (ltm->at.z * unk2);

    DONT_INLINE_HACK();

    camera->viewMatrix.right.x = fVar4;
    camera->viewMatrix.up.x = fVar5;
    camera->viewMatrix.at.x = fVar1;
    camera->viewMatrix.pos.x = 0.5f - (unk2 + (ltm->pos.x * fVar4 + ltm->pos.y * fVar5 + ltm->pos.z * fVar1));

    unk3 = -0.5f * camera->recipViewWindow.y;
    fVar1 = unk3 * camera->viewOffset.y;
    fVar2 = ltm->up.x * unk3 + ltm->at.x * fVar1;
    fVar3 = ltm->up.y * unk3 + ltm->at.y * fVar1;
    fVar4 = ltm->up.z * unk3 + ltm->at.z * fVar1;

    camera->viewMatrix.right.y = fVar2;
    camera->viewMatrix.up.y = fVar3;
    camera->viewMatrix.at.y = fVar4;

    fVar2 = ltm->pos.x * fVar2;
    fVar3 = ltm->pos.y * fVar3;
    fVar4 = ltm->pos.z * fVar4;
    camera->viewMatrix.pos.y = 0.5f - (fVar1 + (fVar2 + fVar3 + fVar4));

    // Below is good
    camera->viewMatrix.right.z = ltm->at.x;
    camera->viewMatrix.up.z = ltm->at.y;
    camera->viewMatrix.at.z = ltm->at.z;
    camera->viewMatrix.pos.z = -(ltm->pos.x * ltm->at.x + ltm->pos.y * ltm->at.y + ltm->pos.z * ltm->at.z);

    RwMatrixOptimize(&camera->viewMatrix, NULL);

    return camera;
}

static RwObjectHasFrame* CameraSync(RwObjectHasFrame* object) {
    RwCamera* camera = (RwCamera*)object;
    if (camera->projectionType == rwPERSPECTIVE) {
        CameraBuildPerspViewMatrix(camera);
        CameraBuildPerspClipPlanes(camera);
    } else {
        CameraBuildParallelViewMatrix(camera);
        CameraBuildParallelClipPlanes(camera);
    }
    RwBBoxCalculate(&camera->frustumBoundBox, camera->frustumCorners, 8);

    return &camera->object;
}

static RwCamera* CameraEndUpdate(RwCamera* camera) {
    if (RwEngineInstance->stdFunc[10](NULL, camera, 0)) {
        RwEngineInstance->curCamera = NULL;
        return camera;
    }

    return NULL;
}

extern void _rwFrameSyncDirty(void);
extern void _rwPipeInitForCamera(RwCamera*);

static RwCamera* CameraBeginUpdate(RwCamera* camera) {
    RwEngineInstance->curCamera = camera;
    _rwFrameSyncDirty();

    if (RwEngineInstance->stdFunc[1](NULL, camera, 0)) {
        _rwPipeInitForCamera(camera);
        return camera;
    }

    return NULL;
}

void* _rwCameraClose(void* object, RwInt32 offset, RwInt32 size) {
    if (GET_CAMERA_MODULE()->freeList != NULL) {
        RwFreeListDestroy(GET_CAMERA_MODULE()->freeList);
        GET_CAMERA_MODULE()->freeList = NULL;
    }

    cameraModule.numInstances--;
    return object;
}

void* _rwCameraOpen(void* object, RwInt32 offset, RwInt32 size) {
    cameraModule.globalsOffset = offset;
    GET_CAMERA_MODULE()->freeList = RwFreeListCreateAndPreallocateSpace(cameraTKList.sizeOfStruct, _rwCameraFreeListBlockSize, 4, _rwCameraFreeListPreallocBlocks, &_rwCameraFreeList);
    if (GET_CAMERA_MODULE()->freeList == NULL) {
        return NULL;
    }

    cameraModule.numInstances++;
    return object;
}

void RwCameraEndUpdate(RwCamera* camera) {
    camera->endUpdate(camera);
}

void RwCameraBeginUpdate(RwCamera* camera) {
    camera->beginUpdate(camera);
}

RwCamera* RwCameraSetViewOffset(RwCamera* camera, RwV2d* viewOffset) {
    RwFrame* frame;

    camera->viewOffset = *viewOffset;

    frame = (RwFrame*)(camera->object.object.parent);
    if (frame != NULL) {
        RwFrameUpdateObjects(frame);
    }

    return camera;
}

RwCamera* RwCameraSetNearClipPlane(RwCamera* camera, RwReal near) {
    RwFrame* frame;

    camera->nearPlane = near;
    CameraUpdateZShiftScale(camera);

    frame = (RwFrame*)(camera->object.object.parent);
    if (frame != NULL) {
        RwFrameUpdateObjects(frame);
    }

    return camera;
}

RwCamera* RwCameraSetFarClipPlane(RwCamera* camera, RwReal far) {
    RwFrame* frame;

    camera->farPlane = far;
    CameraUpdateZShiftScale(camera);

    frame = (RwFrame*)(camera->object.object.parent);
    if (frame != NULL) {
        RwFrameUpdateObjects(frame);
    }

    return camera;
}

RwInt32 RwCameraFrustumTestSphere(RwCamera* camera, RwSphere* sphere) {
    RwFrustumPlane* planes = camera->frustumPlanes;
    RwInt32 ret = 2;
    RwInt32 i;
    for (i = 5; i >= 0; planes++, i--) {
        RwReal unk = (sphere->center.x * planes->plane.normal.x + sphere->center.y * planes->plane.normal.y + sphere->center.z * planes->plane.normal.z) - planes->plane.distance;
        if (unk > sphere->radius) {
            return 0;
        } else if (unk > -sphere->radius) {
            ret = 1;
        }
    }

    return ret;
}

RwCamera* RwCameraClear(RwCamera* camera, RwRGBA* clearColor, RwInt32 a2) {
    if (RwEngineInstance->stdFunc[21](camera, clearColor, a2)) {
        return camera;
    }

    return NULL;
}

RwCamera* RwCameraShowRaster(RwCamera* camera, void* a1, RwInt32 a2) {
    if (RwRasterShowRaster(camera->frameBuffer, a1, a2)) {
        return camera;
    }

    return NULL;
}

RwCamera* RwCameraSetProjection(RwCamera* camera, RwCameraProjection projection) {
    RwFrame* frame;

    switch (projection) {
        case rwPERSPECTIVE:
        case rwPARALLEL:
            camera->projectionType = projection;

            frame = (RwFrame*)(camera->object.object.parent);
            if (frame != NULL) {
                RwFrameUpdateObjects(frame);
            }
            CameraUpdateZShiftScale(camera);
            return camera;
        default:
            RwThrowErrorParams(1, E_RW_BADPARAM, "Invalid projection type specified");
            return NULL;
    }
}

RwCamera* RwCameraSetViewWindow(RwCamera* camera, RwV2d* viewWindow) {
    RwFrame* frame;

    camera->viewWindow = *viewWindow;
    camera->recipViewWindow.x = 1.0f / camera->viewWindow.x;
    camera->recipViewWindow.y = 1.0f / camera->viewWindow.y;

    frame = (RwFrame*)(camera->object.object.parent);
    if (frame != NULL) {
        RwFrameUpdateObjects(frame);
    }

    return camera;
}

RwInt32 RwCameraRegisterPlugin(RwInt32 size, RwInt32 pluginID, RwPluginObjectConstructor constructCB, RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB) {
    return _rwPluginRegistryAddPlugin(&cameraTKList, size, pluginID, constructCB, destructCB, copyCB);
}

RwBool RwCameraDestroy(RwCamera* camera) {
    _rwPluginRegistryDeInitObject(&cameraTKList, camera);
    _rwObjectHasFrameReleaseFrame(&camera->object);
    RwEngineInstance->memoryFree(GET_CAMERA_MODULE()->freeList, camera);

    return TRUE;
}

RwCamera* RwCameraCreate(void) {
    RwCamera* camera = RwEngineInstance->memoryAlloc(GET_CAMERA_MODULE()->freeList);
    if (camera == NULL) {
        return NULL;
    }

    camera->object.object.type = 4;
    camera->object.object.subType = 0;
    camera->object.object.flags = 0;
    camera->object.object.privateFlags = 0;
    camera->object.object.parent = NULL;
    camera->object.sync = CameraSync;
    camera->beginUpdate = CameraBeginUpdate;
    camera->endUpdate = CameraEndUpdate;
    camera->viewWindow.x = camera->viewWindow.y = 1.0f;
    camera->recipViewWindow.x = camera->recipViewWindow.y = 1.0f;
    camera->viewOffset.x = camera->viewOffset.y = 0.0f;
    camera->nearPlane = 0.05f;
    camera->farPlane = 10.0f;
    camera->fogPlane = 5.0f;
    camera->frameBuffer = NULL;
    camera->zBuffer = NULL;
    camera->projectionType = rwPERSPECTIVE;
    CameraUpdateZShiftScale(camera);
    camera->viewMatrix.flags = 0;
    _rwPluginRegistryInitObject(&cameraTKList, camera);

    return camera;
}
