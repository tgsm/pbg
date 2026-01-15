#ifndef RWSDK_WORLD_BAMATERI_H
#define RWSDK_WORLD_BAMATERI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rwsdk/baimage.h"
#include "rwsdk/batextur.h"

typedef struct RpMaterial {
    RwTexture* texture;
    RwRGBA color;
    char unk8[0x1C - 0x8];
} RpMaterial;

struct RpMaterialList {
    struct RpMaterial** materials;
    int numMaterials;
    int space;
};

RpMaterial* RpMaterialSetTexture(RpMaterial* material, RwTexture* texture);

#ifdef __cplusplus
}
#endif

#endif
