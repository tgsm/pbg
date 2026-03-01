#ifndef ENGINE_DISPLAY_CDMORPHANIMATION_H
#define ENGINE_DISPLAY_CDMORPHANIMATION_H

#include "engine/display/IDMorphAnimation.h"

class CDKW_Animation;
struct RpDMorphAnimation;

namespace DKDSP {

class CDMorphAnimation : public IDMorphAnimation {
public:
    U8 m_unk4[0x14 - 0x4];

public:
    CDMorphAnimation(RpDMorphAnimation* rp_dmorph_animation);
    ~CDMorphAnimation();

    virtual EANIMATIONTYPE GetType();
    virtual F32 GetDuration();
    virtual std::string* GetName();
    virtual U32 GetSize();

    void Release();
    void SetName(std::string name);
};
REQUIRE_SIZE(CDMorphAnimation, 0x14);

}

#endif
