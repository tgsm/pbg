#ifndef RWSDK_BARASTER_H
#define RWSDK_BARASTER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwRaster {
    RwRaster* parent;
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

RwRaster* RwRasterUnlock(RwRaster* raster);
RwRaster* RwRasterRenderFast(RwRaster* raster, int, int);
RwRaster* RwRasterRender(RwRaster* raster, int, int);
RwRaster* RwRasterUnlockPalette(RwRaster* raster);
int RwRasterDestroy(RwRaster* raster);
int RwRasterGetNumLevels(RwRaster* raster);
int RwRasterLockPalette(RwRaster* raster, int);
RwRaster* RwRasterShowRaster(RwRaster* raster, void*, unsigned int);
RwRaster* RwRasterCreate(int width, int height, int depth, int);
int RwRasterLock(RwRaster* raster, int, int);

#ifdef __cplusplus
}
#endif

#endif
