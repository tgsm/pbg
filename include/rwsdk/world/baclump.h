#ifndef RWSDK_WORLD_BACLUMP_H
#define RWSDK_WORLD_BACLUMP_H

#include <rwsdk/batypehf.h> // FIXME
#include <rwsdk/world/bageomet.h>

typedef struct RpClump {
    RwObject object;
    RwUInt8 unk8[0x2C - 0x8];
} RpClump; // size: 0x2C

typedef struct RpAtomic {
    RwUInt8 unk0[0x18];
    RpGeometry* geometry;
    RwUInt8 unk1C[0x70 - 0x1C];
} RpAtomic; // size: 0x70

#endif
