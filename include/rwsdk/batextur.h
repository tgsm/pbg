#ifndef RWSDK_BATEXTUR_H
#define RWSDK_BATEXTUR_H

#ifdef __cplusplus
extern "C" {
#endif

struct RwRaster;

typedef struct RwTexture {
    struct RwRaster* raster;
    char unk4[0x4C];
    unsigned int filterAddressing;
    int refCount;
} RwTexture; // size: 0x58

RwTexture* RwTextureCreate(struct RwRaster* raster);

#ifdef __cplusplus
}
#endif

#endif
