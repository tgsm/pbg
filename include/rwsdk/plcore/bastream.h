#ifndef RWSDK_PLCORE_BASTREAM_H
#define RWSDK_PLCORE_BASTREAM_H

#include <rwsdk/rwtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum RwStreamType {
    rwNASTREAM = 0,
    rwSTREAMFILE = 1,
    rwSTREAMFILENAME = 2,
    rwSTREAMMEMORY = 3,
    rwSTREAMCUSTOM = 4,
    rwSTREAMTYPEFORCEENUMSIZEINT = 0x7FFFFFFF,
} RwStreamType;

typedef enum RwStreamAccessType {
    rwNASTREAMACCESS = 0,
    rwSTREAMREAD = 1,
    rwSTREAMWRITE = 2,
    rwSTREAMAPPEND = 3,
    rwSTREAMACCESSTYPEFORCEENUMSIZEINT = 0x7FFFFFFF,
} RwStreamAccessType;

typedef struct RwStreamMemory {
    RwUInt32 position;
    RwUInt32 nSize;
    RwUInt8* memBlock;
} RwStreamMemory; // size: 0xC

typedef union RwStreamFile {
    void* fpFile;
    const void* constfpFile;
} RwStreamFile;

typedef struct RwStreamCustom {
    RwInt32 (*sfnclose)(void*);
    RwUInt32 (*sfnread)(void*, void*, RwUInt32);
    RwInt32 (*sfnwrite)(void*, const void*, RwUInt32);
    RwInt32 (*sfnskip)(void*, RwUInt32);
    void* data;
} RwStreamCustom; // size: 0x14

typedef union RwStreamUnion {
    RwStreamMemory memory;
    RwStreamFile file;
    RwStreamCustom custom;
} RwStreamUnion;

typedef struct RwStream {
    enum RwStreamType type;
    enum RwStreamAccessType accessType;
    RwInt32 position;
    RwStreamUnion Type;
    RwBool rwOwned;
} RwStream; // size: 0x24

typedef struct RwMemory {
    RwUInt8* start;
    RwUInt32 length;
} RwMemory; // size: 0x8

void* _rwStreamModuleOpen(void* a0, RwInt32 offset, RwInt32);
void* _rwStreamModuleClose(void* a0, RwInt32, RwInt32);
RwStream* _rwStreamInitialize(RwStream* stream, RwBool rwOwned, RwStreamType type, RwStreamAccessType accessType, void* data);
RwUInt32 RwStreamRead(RwStream* stream, void* dest, RwUInt32 size);
RwStream* RwStreamWrite(RwStream* stream, const void* src, RwUInt32 size);
RwStream* RwStreamSkip(RwStream* stream, RwUInt32 size);
RwBool RwStreamClose(RwStream* stream, void* data);
RwStream* RwStreamOpen(enum RwStreamType type, enum RwStreamAccessType accessType, void* data);

#ifdef __cplusplus
}
#endif

#endif
