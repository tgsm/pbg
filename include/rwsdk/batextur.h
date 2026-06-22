#ifndef RWSDK_BATEXTUR_H
#define RWSDK_BATEXTUR_H

#include <rwsdk/baraster.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum RwTextureFilterMode {
    rwFILTERNAFILTERMODE = 0,
    rwFILTERNEAREST = 1,
    rwFILTERLINEAR = 2,
    rwFILTERMIPNEAREST = 3,
    rwFILTERMIPLINEAR = 4,
    rwFILTERLINEARMIPNEAREST = 5,
    rwFILTERLINEARMIPLINEAR = 6,
    rwTEXTUREFILTERMODEFORCEENUMSIZEINT = 0x7FFFFFFF,
} RwTextureFilterMode;

typedef struct RwTexture {
    RwRaster* raster;
    RwUInt8 unk4[0xC];
    RwChar name[32];
    RwChar mask[32];
    RwUInt32 filterAddressing;
    RwInt32 refCount;
} RwTexture; // size: 0x58

void* _rwTextureOpen(void*, RwInt32 offset, RwInt32);
void* _rwTextureClose(void*, RwInt32, RwInt32);
RwBool RwTextureSetMipmapping(RwBool);
RwBool RwTextureSetAutoMipmapping(RwBool);
RwTexture* RwTextureSetRaster(RwTexture* texture, RwRaster* raster);
RwTexture* RwTextureCreate(RwRaster* raster);
RwBool RwTextureDestroy(RwTexture* texture);
RwTexture* RwTextureSetName(RwTexture* texture, RwChar* name);
RwTexture* RwTextureSetMaskName(RwTexture* texture, RwChar* mask);
RwBool RwTextureRasterGenerateMipmaps(RwRaster* raster, RwInt32 a1);

#ifdef __cplusplus
}
#endif

#endif
