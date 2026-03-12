#ifndef RWSDK_WORLD_BACLUMP_H
#define RWSDK_WORLD_BACLUMP_H

#include "rwsdk/batypehf.h" // FIXME
#include "rwsdk/world/bageomet.h"

typedef struct RpClump {
    RwObject object;
    char unk8[0x2C - 0x8];
} RpClump; // size: 0x2C

typedef struct RpAtomic {
    char unk0[0x18];
    RpGeometry* geometry;
    char unk1C[0x70 - 0x1C];
} RpAtomic; // size: 0x70

#endif
