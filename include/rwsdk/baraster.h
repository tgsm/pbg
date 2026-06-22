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
    RwUInt8* cpPixels;
    RwUInt8* palette;
    RwInt32 width;
    RwInt32 height;
    RwInt32 depth;
    RwInt32 stride;
    RwInt16 nOffsetX;
    RwInt16 nOffsetY;
    RwUInt8 cType;
    RwUInt8 cFlags;
    RwUInt8 privateFlags;
    RwUInt8 cFormat;
    RwUInt8* originalPixels;
    RwInt32 originalWidth;
    RwInt32 originalHeight;
    RwInt32 originalStride;
} RwRaster;

void* _rwRasterOpen(void*, RwInt32 offset, RwInt32);
void* _rwRasterClose(void*, RwInt32, RwInt32);
void* RwRasterLock(RwRaster* raster, RwInt32, RwInt32);
RwRaster* RwRasterCreate(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32);
RwRaster* RwRasterShowRaster(RwRaster* raster, void*, RwUInt32);
RwInt32 RwRasterGetNumLevels(RwRaster* raster);
void* RwRasterLockPalette(RwRaster* raster, RwInt32);
RwInt32 RwRasterRegisterPlugin(RwInt32 size, RwInt32 a1, RwPluginObjectConstructor constructCB, RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
RwBool RwRasterDestroy(RwRaster* raster);
RwRaster* RwRasterUnlockPalette(RwRaster* raster);
RwRaster* RwRasterRender(RwRaster* raster, RwInt32, RwInt32);
RwRaster* RwRasterRenderFast(RwRaster* raster, RwInt32, RwInt32);
RwRaster* RwRasterUnlock(RwRaster* raster);

#ifdef __cplusplus
}
#endif

#endif
