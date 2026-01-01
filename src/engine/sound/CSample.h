#ifndef ENGINE_SOUND_CSAMPLE_H
#define ENGINE_SOUND_CSAMPLE_H

#include <string>
#include "engine/sound/CSampleDictionary.h"
#include "engine/sound/ISample.h"

namespace DKSND {

class CSample : public ISample {
public:
    std::string m_name;
    std::string m_filename;
    SSAMPLE_FORMAT m_format;
    U8 m_unk20[0x24 - 0x20];
    U32 m_is_3d;
    BOOL m_is_streamed;
    CSampleDictionary* m_dictionary;
    U8 m_unk30[0x50 - 0x30];

public:
    CSample(ISampleDictionary* dictionary);
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
    virtual U8* LoadSampleFromMemory(U8*, U8*);
    virtual void LoadStreamedSampleFromMemory(U8*);
    virtual void Set3D(BOOL is_3d);
    virtual void SetStreamed(BOOL streamed);
    virtual BOOL IsValid();

    void SetFullFileName(std::string* filename);
    std::string* GetFullFileName();
    void ReleaseSpecific();
    void Invalidate();
    BOOL LoadSpecific(U8*);
};
REQUIRE_SIZE(CSample, 0x50);

}

#endif
