#ifndef ENGINE_SOUND_CSAMPLE_H
#define ENGINE_SOUND_CSAMPLE_H

#include "engine/sound/ISample.h"

namespace DKSND {

class CSample : public ISample {
public:
    virtual ~CSample();

    virtual U32 GetSize();
    virtual std::string* GetName();
    virtual U32 GetFrequency();
    virtual U8 GetBitRate();
    virtual SSAMPLE_FORMAT GetFormat();
    virtual BOOL Is3D();
    virtual BOOL IsStereo();
    virtual BOOL IsStreamed();
    virtual void SetName(std::string* name);
    virtual void SetFormat(SSAMPLE_FORMAT* format);
    virtual U32 LoadSampleFromMemory(U8*, U8*); // Currently unknown return type
    virtual void LoadStreamedSampleFromMemory(U8*);
    virtual BOOL Set3D(BOOL is_3d);
    virtual BOOL SetStreamed(BOOL streamed);
    virtual BOOL IsValid();
};

}

#endif
