#ifndef RWSDK_PLCORE_BABINARY_H
#define RWSDK_PLCORE_BABINARY_H

#include <rwsdk/plcore/bastream.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwChunkHeaderInfo {
    RwUInt32 type;
    RwUInt32 length;
    RwUInt32 version;
    RwUInt32 buildNum;
    RwBool isComplex;
} RwChunkHeaderInfo; // size: 0x14

RwBool _rwStreamReadChunkHeader(RwStream* stream, RwUInt32* type, RwUInt32* length, RwUInt32* version, RwUInt32* buildNum);
void _rwStreamWriteVersionedChunkHeader(RwStream* stream, RwUInt32 type, RwUInt32 length, RwUInt32 version, RwUInt32 buildNum);
RwBool RwStreamFindChunk(RwStream* stream, RwUInt32 type, RwUInt32* length, RwUInt32* version);
RwUInt32* RwMemLittleEndian32(RwUInt32* data, RwUInt32 size);
RwUInt16* RwMemLittleEndian16(RwUInt16* data, RwUInt32 size);
RwUInt32* RwMemNative32(RwUInt32* data, RwUInt32 size);
RwUInt16* RwMemNative16(RwUInt16* data, RwUInt32 size);
RwStream* RwStreamWriteInt32(RwStream* stream, RwUInt32* data, RwUInt32 size);
RwStream* RwStreamWriteInt16(RwStream* stream, RwUInt16* data, RwUInt32 size);
RwStream* RwStreamReadInt16(RwStream* stream, RwUInt16* data, RwUInt32 size);
RwStream* RwStreamReadInt32(RwStream* stream, RwUInt32* data, RwUInt32 size);
RwStream* RwStreamReadChunkHeaderInfo(RwStream* stream, RwChunkHeaderInfo* info);

#ifdef __cplusplus
}
#endif

#endif
