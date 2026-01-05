#ifndef RWSDK_BATEXTUR_H
#define RWSDK_BATEXTUR_H

#include <rwsdk/baraster.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwTexture {
    RwRaster* raster;
    char unk4[0xC];
    char name[32];
    char mask[32];
    unsigned int filterAddressing;
    int refCount;
} RwTexture; // size: 0x58

RwTexture* RwTextureSetRaster(RwTexture* texture, RwRaster* raster);
RwTexture* RwTextureCreate(RwRaster* raster);
int RwTextureDestroy(RwTexture* texture);
RwTexture* RwTextureSetName(RwTexture* texture, char* name);
RwTexture* RwTextureSetMaskName(RwTexture* texture, char* mask);
int RwTextureRasterGenerateMipmaps(RwRaster* raster, int a1);

#ifdef __cplusplus
}
#endif

#endif
