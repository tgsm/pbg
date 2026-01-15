#ifndef RWSDK_WORLD_BAGEOMET_H
#define RWSDK_WORLD_BAGEOMET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rwsdk/batypehf.h"
#include "rwsdk/world/bamateri.h"

typedef struct RpGeometry {
    RwObject object;
    unsigned int flags;
    char unkC[0x20 - 0xC];
    RpMaterialList matList;
    char unk2C[0x60 - 0x2C];
} RpGeometry;

RpGeometry* RpGeometryLock(RpGeometry* geometry, unsigned short a1);
RpGeometry* RpGeometryUnlock(RpGeometry* geometry);

#ifdef __cplusplus
}
#endif

#endif
