#ifndef RWSDK_WORLD_BAMATERI_H
#define RWSDK_WORLD_BAMATERI_H

#include <rwsdk/baimage.h>
#include <rwsdk/batextur.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RpMaterial {
    RwTexture* texture;
    RwRGBA color;
    RwUInt8 unk8[0x1C - 0x8];
} RpMaterial;

struct RpMaterialList {
    struct RpMaterial** materials;
    RwInt32 numMaterials;
    RwInt32 space;
};

RpMaterial* RpMaterialSetTexture(RpMaterial* material, RwTexture* texture);

#ifdef __cplusplus
}
#endif

#endif
