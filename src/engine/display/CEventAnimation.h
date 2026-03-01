#ifndef ENGINE_DISPLAY_CEVENTANIMATION_H
#define ENGINE_DISPLAY_CEVENTANIMATION_H

#include "engine/display/IEventAnimation.h"
#include "engine/xmd/CXmdFile.h"

namespace DKDSP {

class CEventAnimation : public IEventAnimation {
public:
    U8 m_unk4[0x14 - 0x4];
    void* m_unk14;
    U8 m_unk18[4];

public:
    CEventAnimation();
    ~CEventAnimation();

    virtual EANIMATIONTYPE GetType();
    virtual F32 GetDuration();
    virtual std::string* GetName();
    virtual U32 GetSize();

    void Release();
    void SetName(std::string name);
    void SetDuration(F32 duration);
    void Create(int, F32, DkXmd::CXmdFile*, int);
    void SetKey(int, SEVENTANIMKEY& key);
};
REQUIRE_SIZE(CEventAnimation, 0x1C);

}

#endif
