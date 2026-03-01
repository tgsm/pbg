#ifndef ENGINE_DISPLAY_CANIMATION_H
#define ENGINE_DISPLAY_CANIMATION_H

#include "engine/display/IAnimation.h"

class CDKW_Animation;

namespace DKDSP {

class CAnimation : public IAnimation {
public:
    U8 m_unk4[4];
    CDKW_Animation* m_wrap_animation;
    U8 m_unkC[8];

public:
    CAnimation(CDKW_Animation* wrap_animation);
    ~CAnimation();

    virtual EANIMATIONTYPE GetType();
    virtual F32 GetDuration();
    virtual std::string* GetName();
    virtual U32 GetSize();
    virtual F32 MakeDelta(int, F32);
    virtual int OptimizeSmoothAnim(int, int, F32, F32, F32);
    virtual int ConvertToStdAnim();
    virtual BOOL IsDelta();
    virtual void SetDuration(F32 duration);
    virtual int GetNumberOfKeyframes();
    virtual int GetFlags();
    virtual void SetFlags(int flags);

    void Release();
    void SetName(std::string name);
};
REQUIRE_SIZE(CAnimation, 0x14);

}

#endif
