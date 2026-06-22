#include <cstddef>
#include <math.h>
#include <rwsdk/badevice.h>
#include <rwsdk/baframe.h>
#include <rwsdk/plcore/bavector.h>
#include <rwsdk/world/balight.h>

static RwFreeList _rpLightFreeList;
static RwModuleInfo lightModule;
static RwInt32 _rpLightFreeListBlockSize = 32;
static RwInt32 _rpLightFreeListPreallocBlocks = 1;

struct RwPluginRegistry lightTKList = {
    sizeof(RpLight),
    sizeof(RpLight),
    0,
    0,
    NULL,
    NULL,
};

static void LightTidyDestroyLight(RpLight* light) {
    RpLightDestroy(light);
}

static RwObjectHasFrame* LightSync(RwObjectHasFrame* a0) {
    return a0;
}

RpLight* RpLightSetRadius(RpLight* light, float radius) {
    RwFrame* frame = (RwFrame*)light->object.object.parent;
    light->radius = radius;
    if (frame != NULL) {
        RwFrameUpdateObjects(frame);
    }

    return light;
}

RpLight* RpLightSetColor(RpLight* light, RwRGBAReal* color) {
    light->color = *color;
    if (light->color.red == light->color.green && light->color.red == light->color.blue) {
        light->object.object.privateFlags = rpLIGHTPRIVATENOCHROMA;
    } else {
        light->object.object.privateFlags = 0;
    }

    return light;
}

RpLight* RpLightSetConeAngle(RpLight* light, float coneAngle) {
    if (coneAngle < 0.0f || coneAngle > 89.9999f) {
        return NULL;
    }

    light->minusCosAngle = -cosf(coneAngle);
    return light;
}

void RpLightRegisterPlugin(RwUInt32 size, RwUInt32 a1, void* a2, void* a3, void* a4) {
    _rwPluginRegistryAddPlugin(&lightTKList, size, a1, a2, a3, a4);
}

RwBool RpLightDestroy(RpLight* light) {
    _rwPluginRegistryDeInitObject(&lightTKList, light);
    _rwObjectHasFrameReleaseFrame(&light->object);
    RwEngineInstance->memoryFree(*(RwFreeList**)((RwInt32)RwEngineInstance + lightModule.globalsOffset), light);

    return TRUE;
}

RpLight* RpLightCreate(RpLightType type) {
    RpLight* light = RwEngineInstance->memoryAlloc(*(RwFreeList**)((RwInt32)RwEngineInstance + lightModule.globalsOffset));
    if (light == NULL) {
        return NULL;
    }

    light->object.object.type = 3;
    light->object.object.subType = type;
    light->object.object.flags = 0;
    light->object.object.privateFlags = 0;
    light->object.object.parent = NULL;
    light->object.sync = LightSync;
    light->radius = 0.0f;
    light->minusCosAngle = 1.0f;
    light->color.red = 1.0f;
    light->color.green = 1.0f;
    light->color.blue = 1.0f;
    light->color.alpha = 1.0f;
    light->object.object.privateFlags = rpLIGHTPRIVATENOCHROMA;
    light->WorldSectorsInLight.link.next = &light->WorldSectorsInLight.link;
    light->WorldSectorsInLight.link.prev = &light->WorldSectorsInLight.link;
    light->inWorld.prev = NULL;
    light->inWorld.next = NULL;
    light->lightFrame = RwEngineInstance->lightFrame - 1;
    light->object.object.flags = rpLIGHTLIGHTATOMICS | rpLIGHTLIGHTWORLD;

    _rwPluginRegistryInitObject(&lightTKList, light);
    return light;
}

void* _rpLightClose(void* a0) {
    RwFreeListForAllUsed(*(RwFreeList**)((RwInt32)RwEngineInstance + lightModule.globalsOffset), (RwFreeListCallBack)LightTidyDestroyLight, NULL);
    RwFreeListDestroy(*(RwFreeList**)((RwInt32)RwEngineInstance + lightModule.globalsOffset));
    *(RwFreeList**)((RwInt32)RwEngineInstance + lightModule.globalsOffset) = NULL;

    lightModule.numInstances--;
    return a0;
}

// FIXME: Unknown return/param type
void* _rpLightOpen(void* a0, RwUInt32 globalsOffset) {
    lightModule.globalsOffset = globalsOffset;
    *(RwFreeList**)((RwInt32)RwEngineInstance + lightModule.globalsOffset) = RwFreeListCreateAndPreallocateSpace(lightTKList.sizeOfStruct, _rpLightFreeListBlockSize, 4, _rpLightFreeListPreallocBlocks, &_rpLightFreeList);
    if (*(RwFreeList**)((RwInt32)RwEngineInstance + lightModule.globalsOffset) != NULL) {
        lightModule.numInstances++;
        return a0;
    }

    return NULL;
}
