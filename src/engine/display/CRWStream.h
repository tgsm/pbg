#ifndef ENGINE_DISPLAY_CRWSTREAM_H
#define ENGINE_DISPLAY_CRWSTREAM_H

#include <rwsdk/plcore/bastream.h>
#include "engine/display/CAnimation.h"
#include "engine/display/IRWStream.h"

class CDKW_Stream : public RwStream {};

struct RpDMorphAnimation;

namespace DKDSP {

class CRWStream : public IRWStream {
public:
    U8 m_unk4[0x10];

public:
    CRWStream(CDKW_Stream* wrap_stream, RwStreamType type, RwStreamAccessType access_type, void*);
    ~CRWStream();

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

    void Release();
    CDKW_Animation* ReadAnimation();
    CDKW_Animation* ReadSmoothAnimation();
    RpDMorphAnimation* ReadDMorphAnimation();
    BOOL WriteAnimation(CDKW_Animation* animation);
};

}

#endif
