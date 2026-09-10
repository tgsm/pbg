#ifndef RWSDK_BATEXTUR_H
#define RWSDK_BATEXTUR_H

#include <rwsdk/baimage.h>
#include <rwsdk/baraster.h>
#include <rwsdk/batypehf.h>

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

typedef struct RwTexDictionary {
    RwObject object;
    RwLinkList texturesInDict;
    RwLLLink lInInstance;
} RwTexDictionary; // size: 0x18

typedef struct RwTexture {
    RwRaster* raster;
    RwTexDictionary* dict;
    RwLLLink lInDictionary;
    RwChar name[32];
    RwChar mask[32];
    RwUInt32 filterAddressing;
    RwInt32 refCount;
} RwTexture; // size: 0x58

typedef RwTexture* (*RwTextureCallBackRead)(const char*, const char*);
typedef RwRaster* (*RwTextureCallBackMipmapGeneration)(RwRaster*, RwImage*);
typedef RwTexture* (*RwTextureCallBack)(RwTexture*, void*);
typedef RwTexDictionary* (*RwTexDictionaryCallBack)(RwTexDictionary*, void*);
typedef RwBool (*RwTextureCallBackMipmapName)(char*, char*, RwUInt8, RwInt32);

RwBool RwTextureSetMipmapping(RwBool);
RwBool RwTextureGetMipmapping(void);
RwBool RwTextureSetAutoMipmapping(RwBool);
RwBool RwTextureGetAutoMipmapping(void);
RwTexture* RwTextureSetRaster(RwTexture* texture, RwRaster* raster);
RwTexDictionary* RwTexDictionaryCreate(void);
RwBool RwTexDictionaryDestroy(RwTexDictionary* dict);
RwTexDictionary* RwTexDictionaryForAllTextures(RwTexDictionary* dict, RwTextureCallBack callback, void* data);
RwBool RwTexDictionaryForAllTexDictionaries(RwTexDictionaryCallBack callback, void* data);
RwTexture* RwTextureCreate(RwRaster* raster);
RwBool RwTextureDestroy(RwTexture* texture);
RwTexture* RwTextureSetName(RwTexture* texture, RwChar* name);
RwTexture* RwTextureSetMaskName(RwTexture* texture, RwChar* mask);
RwTexture* RwTexDictionaryAddTexture(RwTexDictionary* dict, RwTexture* texture);
RwTexture* RwTexDictionaryFindNamedTexture(RwTexDictionary* dict, char* name);
RwTexDictionary* RwTexDictionaryGetCurrent(void);
RwBool RwTextureGenerateMipmapName(RwChar*, RwChar*, RwUInt8, RwInt32);
RwTexture* RwTextureRead(RwChar* path, RwChar* a1);
RwInt32 RwTextureRegisterPlugin(RwInt32 size, RwInt32 pluginID, RwPluginObjectConstructor constructCB, RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
RwBool RwTextureRasterGenerateMipmaps(RwRaster* raster, RwImage* a1);
void* _rwTextureClose(void*, RwInt32, RwInt32);
void* _rwTextureOpen(void*, RwInt32 offset, RwInt32);

#ifdef __cplusplus
}
#endif

#endif
