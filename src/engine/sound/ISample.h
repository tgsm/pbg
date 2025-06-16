#ifndef ENGINE_SOUND_ISAMPLE_H
#define ENGINE_SOUND_ISAMPLE_H

#include <string>
#include "types.h"

namespace DKSND {

struct SSAMPLE_FORMAT {

};

class ISample {
public:
    int m_unk0; // refcount?

public:
    ISample();
    virtual ~ISample() {}

    virtual U32 GetSize() = 0;
    virtual std::string* GetName() = 0;
    virtual U32 GetFrequency() = 0;
    virtual U8 GetBitRate() = 0;
    virtual SSAMPLE_FORMAT GetFormat() = 0;
    virtual BOOL Is3D() = 0;
    virtual BOOL IsStereo() = 0;
    virtual BOOL IsStreamed() = 0;
    virtual void SetName(std::string* name) = 0;
};

}

#endif
