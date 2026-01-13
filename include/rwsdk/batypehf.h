#ifndef RWSDK_BATYPEHF_H
#define RWSDK_BATYPEHF_H

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

// TODO
typedef struct RwObjectHasFrame {
    RwObject object;
    char unk8[0x14 - 0x8];
} RwObjectHasFrame;

#ifdef __cplusplus
}
#endif

#endif
