#ifndef RWA_CORE_RWAWAVEDEF_H
#define RWA_CORE_RWAWAVEDEF_H

#include <rwa/core/rwafreelist.h>
#include <rwa/core/rwaobjdef.h>
#include <rwa/core/rwawave.h>
#include <rwa/core/rwauuid.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwaWaveFormat {
    RwUInt32 sampleRate;
    const RwaUUID* dataType;
    RwUInt32 length;
    RwUInt8 bitDepth;
    RwUInt8 noChannels;
    void* miscData;
    RwUInt32 miscDataSize;
    RwUInt8 flags;
    RwUInt8 reserved;
} RwaWaveFormat; // size: 0x1C

struct RwaWaveDef;

typedef RwaWave* (*RwaWaveDefCreateFunc)(); // FIXME: determine params
typedef void (*RwaWaveDefDestroyFunc)(); // FIXME: determine params
typedef RwaWave* (*RwaWaveDefUploadFunc)(); // FIXME: determine params
typedef RwaWave* (*RwaWaveDefDownloadFunc)(); // FIXME: determine params
typedef RwBool (*RwaWaveDefUploadBusyFunc)(); // FIXME: determine params
typedef void (*RwaWaveDefUnregisterCallback)(struct RwaWaveDef*);

typedef struct RwaWaveDefFormat {
    RwaUUID* dataType;
    RwUInt8 bitDepth;
    RwUInt8 noChannels;
    RwUInt32 minSampleRate;
    RwUInt32 maxSampleRate;
    RwUInt8 flags;
} RwaWaveDefFormat; // size: 0x14

typedef struct RwaWaveDef {
    RwaUniqueID uniqueID;
    RwaWaveDefCreateFunc createFunction;
    RwaWaveDefDestroyFunc destroyFunction;
    RwaWaveDefUploadFunc uploadFunction;
    RwaWaveDefDownloadFunc downloadFunction;
    RwaWaveDefUploadBusyFunc uploadBusyFunction;
    RwaWaveDefUnregisterCallback unregisterCallback;
    RwUInt32 noSupportedFormats;
    union {
        RwaWaveDefFormat** formats;
        const RwaWaveDefFormat** copyFormats;
    } supportedFormats;
    RwUInt32 defaultFormatIndex;
    RwaObjDef* objDef;
    RwUInt32 stateSize;
    RwUInt32 stateAlign;
    RwaFreeList* freeList;
    RwLLLink link;
    RwUInt32 flags;
} RwaWaveDef; // size: 0x4C

typedef RwaWaveDef* (*RwaWaveDefRegisterFunc)(void);

RwBool _rwaWaveDefOpenModule(void);
void _rwaWaveDefCloseModule(void);
RwBool _rwaWaveDefRegister(RwaWaveDefRegisterFunc* registerFuncs, RwUInt32 noRegisterFuncs);
RwBool RwaWaveFormatCompare(RwaWaveFormat* a, RwaWaveFormat* b);
RwUInt32 RwaWaveFormatGetFrameSize(RwaWaveFormat* format);
void _rwaWaveFormatFreeMiscData(RwaWaveFormat* format);
RwaWaveDef* RwaWaveDefAssignID(RwaWaveDef* def, const RwaUUID* uuid, const char* name);
RwaWaveDef* RwaWaveDefCreate(RwaObjDef* objDef, RwaWaveDef* def);
RwBool RwaWaveDefDestroy(RwaWaveDef* def);
void _rwaWaveDefFormatCopyToWaveFormat(RwaWaveDefFormat* defFormat, RwaWaveFormat* format, RwBool initialize);
RwBool RwaWaveDefFormatWaveFormatCompare(RwaWaveDefFormat* defFormat, RwaWaveFormat* format);
void RwaWaveDefAssignSupportedFormats(RwaWaveDef* def, const RwaWaveDefFormat** formats, RwUInt32 noFormats);
RwUInt32 _rwaWaveFormatGetSize(RwaWaveFormat* format);
RwaWaveFormat* _rwaWaveFormatSerialize(RwaWaveFormat* a, RwaWaveFormat* b, RwBool a2, RwInt32 endianness);
RwBool _rwaWaveDefIsTargetFormatSupported(RwaWaveDef* def, RwaWaveFormat* format);
const RwaUUID* _rwaWaveFormatFindUUID(RwaUUID* uuid);
RwInt32 RwaWaveFormatGetSamplesPerAudioFrame(RwaWaveFormat* format);

#ifdef __cplusplus
}
#endif

#endif
