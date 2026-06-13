#ifndef RWSDK_BARASTER_H
#define RWSDK_BARASTER_H

#include <rwsdk/plcore/batkreg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum RwRasterType {
    rwRASTERTYPENORMAL = 0,
    rwRASTERTYPEZBUFFER = 1,
    rwRASTERTYPECAMERA = 2,
    rwRASTERTYPEUNK3 = 3,
    rwRASTERTYPETEXTURE = 4,
    rwRASTERTYPECAMERATEXTURE = 5,
    rwRASTERTYPEMASK = 7,
    rwRASTERPALETTEVOLATILE = 64,
    rwRASTERDONTALLOCATE = 128,
    rwRASTERTYPEFORCEENUMSIZEINT = 0x7FFFFFFF,
} RwRasterType;

typedef enum RwRasterFormat {
    rwRASTERFORMATDEFAULT = 0,
    rwRASTERFORMAT1555 = 0x100,
    rwRASTERFORMAT565 = 0x200,
    rwRASTERFORMAT4444 = 0x300,
    rwRASTERFORMATLUM8 = 0x400,
    rwRASTERFORMAT8888 = 0x500,
    rwRASTERFORMAT888 = 0x600,
    rwRASTERFORMAT16 = 0x700,
    rwRASTERFORMAT24 = 0x800,
    rwRASTERFORMAT32 = 0x900,
    rwRASTERFORMAT555 = 0xA00,
    rwRASTERFORMATAUTOMIPMAP = 0x1000,
    rwRASTERFORMATPAL8 = 0x2000,
    rwRASTERFORMATPAL4 = 0x4000,
    rwRASTERFORMATMIPMAP = 0x8000,
    rwRASTERFORMATPIXELFORMATMASK = 0xF00,
    rwRASTERFORMATMASK = 0xFF00,
    rwRASTERFORMATFORCEENUMSIZEINT = 0x7FFFFFFF,
} RwRasterFormat;

typedef enum RwRasterPrivateFlag {
    rwRASTERGAMMACORRECTED = (1 << 0),
    rwRASTERPIXELLOCKEDREAD = (1 << 1),
    rwRASTERPIXELLOCKEDWRITE = (1 << 2),
    rwRASTERPALETTELOCKEDREAD = (1 << 3),
    rwRASTERPALETTELOCKEDWRITE = (1 << 4),
    rwRASTERPIXELLOCKEDRAW = (1 << 5),
    rwRASTERPRIVATEFLAGFORCEENUMSIZEINT = 0x7FFFFFFF,
} RwRasterPrivateFlag;

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

void* _rwRasterOpen(void*, int offset, int);
void* _rwRasterClose(void*, int, int);
void* RwRasterLock(RwRaster* raster, int, int);
RwRaster* RwRasterCreate(int width, int height, int depth, int);
RwRaster* RwRasterShowRaster(RwRaster* raster, void*, unsigned int);
int RwRasterGetNumLevels(RwRaster* raster);
void* RwRasterLockPalette(RwRaster* raster, int);
int RwRasterRegisterPlugin(int size, int a1, RwPluginObjectConstructor constructCB, RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
int RwRasterDestroy(RwRaster* raster);
RwRaster* RwRasterUnlockPalette(RwRaster* raster);
RwRaster* RwRasterRender(RwRaster* raster, int, int);
RwRaster* RwRasterRenderFast(RwRaster* raster, int, int);
RwRaster* RwRasterUnlock(RwRaster* raster);

#ifdef __cplusplus
}
#endif

#endif
