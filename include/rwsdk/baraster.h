#ifndef RWSDK_BARASTER_H
#define RWSDK_BARASTER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwRaster {
    struct RwRaster* parent;
    unsigned char* cpPixels;
    unsigned char* palette;
    int width;
    int height;
    int depth;
    int stride;
    short nOffsetX;
    short nOffsetY;
    unsigned char cType;
    unsigned char cFlags;
    unsigned char privateFlags;
    unsigned char cFormat;
    unsigned char* originalPixels;
    int originalWidth;
    int originalHeight;
    int originalStride;
} RwRaster;

void* RwRasterLock(RwRaster* raster, int, int);
RwRaster* RwRasterCreate(int width, int height, int depth, int);
RwRaster* RwRasterShowRaster(RwRaster* raster, void*, unsigned int);
int RwRasterGetNumLevels(RwRaster* raster);
void* RwRasterLockPalette(RwRaster* raster, int);
int RwRasterRegisterPlugin(unsigned int size, unsigned int a1, void* a2, void* a3, void* a4); // FIXME: Stubbed param types
int RwRasterDestroy(RwRaster* raster);
RwRaster* RwRasterUnlockPalette(RwRaster* raster);
RwRaster* RwRasterRender(RwRaster* raster, int, int);
RwRaster* RwRasterRenderFast(RwRaster* raster, int, int);
RwRaster* RwRasterUnlock(RwRaster* raster);

#ifdef __cplusplus
}
#endif

#endif
