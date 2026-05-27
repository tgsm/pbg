#ifndef RWSDK_BATYPEHF_H
#define RWSDK_BATYPEHF_H

#include <rwsdk/plcore/bamemory.h>

#ifdef __cplusplus
extern "C" {
#endif

// Not sure where to put this yet
typedef struct RwObject {
    unsigned char type;
    unsigned char subType;
    unsigned char flags;
    unsigned char privateFlags;
    void* parent;
} RwObject;

typedef struct RwObjectHasFrame {
    RwObject object;
    RwLLLink lFrame;
    struct RwObjectHasFrame* (*sync)(struct RwObjectHasFrame*);
} RwObjectHasFrame;

void _rwObjectHasFrameReleaseFrame(RwObjectHasFrame* object);

#ifdef __cplusplus
}
#endif

#endif
