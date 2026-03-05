#ifndef RWSDK_WORLD_BACLUMP_H
#define RWSDK_WORLD_BACLUMP_H

#include "rwsdk/batypehf.h" // FIXME

typedef struct RpClump {
    RwObject object;
    char unk8[0x2C - 0x8];
} RpClump; // size: 0x2C

#endif
