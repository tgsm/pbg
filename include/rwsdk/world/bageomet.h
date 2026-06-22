#ifndef RWSDK_WORLD_BAGEOMET_H
#define RWSDK_WORLD_BAGEOMET_H

#include <rwsdk/batypehf.h>
#include <rwsdk/world/bamateri.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RpGeometry {
    RwObject object;
    RwUInt32 flags;
    RwUInt8 unkC[0x20 - 0xC];
    RpMaterialList matList;
    RwUInt8 unk2C[0x60 - 0x2C];
} RpGeometry;

RpGeometry* RpGeometryLock(RpGeometry* geometry, RwUInt16 a1);
RpGeometry* RpGeometryUnlock(RpGeometry* geometry);

#ifdef __cplusplus
}
#endif

#endif
