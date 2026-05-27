#include <cstddef>
#include <math.h>
#include <rwsdk/badevice.h>
#include <rwsdk/baframe.h>
#include <rwsdk/plcore/bavector.h>
#include <rwsdk/world/balight.h>

static RwFreeList _rpLightFreeList;
static RwModuleInfo lightModule;
static int _rpLightFreeListBlockSize = 32;
static int _rpLightFreeListPreallocBlocks = 1;

// TODO
struct UnkRwPluginRegistryStruct {
    int unk0;
    int unk4;
    char unk8[0x10];
};
struct UnkRwPluginRegistryStruct lightTKList = {
    sizeof(RpLight),
    sizeof(RpLight),
};
extern void _rwPluginRegistryInitObject(struct UnkRwPluginRegistryStruct*, void* object);
extern void _rwPluginRegistryDeInitObject(struct UnkRwPluginRegistryStruct*, void* object);

// FIXME: Stubbed param types
extern int _rwPluginRegistryAddPlugin(struct UnkRwPluginRegistryStruct*, unsigned int size, unsigned int, void*, void*, void*);

static void LightTidyDestroyLight(RpLight* light) {
    RpLightDestroy(light);
}

static RwObjectHasFrame* LightSync(RwObjectHasFrame*) {
    // UB: Does not return anything
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

void RpLightRegisterPlugin(unsigned int size, unsigned int a1, void* a2, void* a3, void* a4) {
    _rwPluginRegistryAddPlugin(&lightTKList, size, a1, a2, a3, a4);
}

int RpLightDestroy(RpLight* light) {
    _rwPluginRegistryDeInitObject(&lightTKList, light);
    _rwObjectHasFrameReleaseFrame(&light->object);
    RwEngineInstance->unk144(*(RwFreeList**)((int)RwEngineInstance + lightModule.globalsOffset), light);

    return 1;
}

RpLight* RpLightCreate(RpLightType type) {
    RpLight* light = RwEngineInstance->unk140(*(RwFreeList**)((int)RwEngineInstance + lightModule.globalsOffset));
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
    RwFreeListForAllUsed(*(RwFreeList**)((int)RwEngineInstance + lightModule.globalsOffset), (RwFreeListCallBack)LightTidyDestroyLight, NULL);
    RwFreeListDestroy(*(RwFreeList**)((int)RwEngineInstance + lightModule.globalsOffset));
    *(RwFreeList**)((int)RwEngineInstance + lightModule.globalsOffset) = NULL;

    lightModule.numInstances--;
    return a0;
}

// FIXME: Unknown return/param type
void* _rpLightOpen(void* a0, unsigned int globalsOffset) {
    lightModule.globalsOffset = globalsOffset;
    *(RwFreeList**)((int)RwEngineInstance + lightModule.globalsOffset) = RwFreeListCreateAndPreallocateSpace(lightTKList.unk0, _rpLightFreeListBlockSize, 4, _rpLightFreeListPreallocBlocks, &_rpLightFreeList);
    if (*(RwFreeList**)((int)RwEngineInstance + lightModule.globalsOffset) != NULL) {
        lightModule.numInstances++;
        return a0;
    }

    return NULL;
}
