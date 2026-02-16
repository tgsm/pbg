#ifndef RWSDK_BAIMAGE_H
#define RWSDK_BAIMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwRGBA {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
} RwRGBA;

typedef struct RwImage {
    int flags;
    int width;
    int height;
    int depth;
    int stride;
    unsigned char* cpPixels;
    RwRGBA* palette;
} RwImage;

RwImage* RwImageCreate(int width, int height, int depth);
int RwImageDestroy(RwImage* image);
RwImage* RwImageAllocatePixels(RwImage* image);
RwImage* RwImageFreePixels(RwImage* image);
void RwImageSetPath(char* path);
RwImage* RwImageGammaCorrect(RwImage* image);
float RwImageGetGamma(void);
void RwImageSetGamma(float gamma);

#ifdef __cplusplus
}
#endif

#endif
