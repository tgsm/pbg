#ifndef ENGINE_DISPLAY_IRWSTREAM_H
#define ENGINE_DISPLAY_IRWSTREAM_H

#include "types.h"

namespace DKDSP {

struct SRWChunckHeaderInfo;

class IRWStream {
public:
    virtual BOOL FindChunk(U32, U32, U32) = 0;
    virtual BOOL ReadChunkHeaderInfo(SRWChunckHeaderInfo*) = 0;
    virtual BOOL Read(void*, U32) = 0; // does this actually return a BOOL?
    virtual BOOL ReadS16(S16*, U32) = 0;
    virtual BOOL ReadS32(S32*, U32) = 0;
    virtual BOOL ReadFloat(F32*, U32) = 0;
    virtual BOOL WriteChunkHeaderInfo(int, int) = 0;
    virtual BOOL Write(void*, U32) = 0;
    virtual BOOL WriteS16(S16*, U32) = 0;
    virtual BOOL WriteS32(S32*, U32) = 0;
    virtual BOOL WriteFloat(F32*, U32) = 0;
    virtual BOOL Skip(U32) = 0;
};

}

#endif
