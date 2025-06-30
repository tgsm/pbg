#ifndef ENGINE_DISPLAY_CRWSTREAM_H
#define ENGINE_DISPLAY_CRWSTREAM_H

#include "engine/display/IRWStream.h"

namespace DKDSP {

class CRWStream : public IRWStream {
public:
    virtual BOOL FindChunk(U32, U32, U32);
    virtual BOOL ReadChunkHeaderInfo(SRWChunckHeaderInfo*);
    virtual BOOL Read(void*, U32); // does this actually return a BOOL?
    virtual BOOL ReadS16(S16*, U32);
    virtual BOOL ReadS32(S32*, U32);
    virtual BOOL ReadFloat(F32*, U32);
    virtual BOOL WriteChunkHeaderInfo(int, int);
    virtual BOOL Write(void*, U32);
    virtual BOOL WriteS16(S16*, U32);
    virtual BOOL WriteS32(S32*, U32);
    virtual BOOL WriteFloat(F32*, U32);
    virtual BOOL Skip(U32);
};

}

#endif
