#ifndef RWSDK_BAIMAGE_H
#define RWSDK_BAIMAGE_H

#include <rwsdk/rwtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwRGBA {
    RwUInt8 red;
    RwUInt8 green;
    RwUInt8 blue;
    RwUInt8 alpha;
} RwRGBA;

typedef struct RwRGBAReal {
    RwReal red;
    RwReal green;
    RwReal blue;
    RwReal alpha;
} RwRGBAReal;

typedef struct RwImage {
    RwInt32 flags;
    RwInt32 width;
    RwInt32 height;
    RwInt32 depth;
    RwInt32 stride;
    RwUInt8* cpPixels;
    RwRGBA* palette;
} RwImage;

void* _rwImageOpen(void*, RwInt32 offset, RwInt32);
void* _rwImageClose(void*, RwInt32, RwInt32);
RwImage* RwImageCreate(RwInt32 width, RwInt32 height, RwInt32 depth);
RwBool RwImageDestroy(RwImage* image);
RwImage* RwImageAllocatePixels(RwImage* image);
RwImage* RwImageFreePixels(RwImage* image);
void RwImageSetPath(RwChar* path);
RwImage* RwImageGammaCorrect(RwImage* image);
RwReal RwImageGetGamma(void);
void RwImageSetGamma(RwReal gamma);

#ifdef __cplusplus
}
#endif

#endif
