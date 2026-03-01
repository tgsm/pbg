#ifndef ENGINE_DISPLAY_CANIMATIONSTAR_H
#define ENGINE_DISPLAY_CANIMATIONSTAR_H

#include "engine/display/IAnimationStar.h"

namespace DKDSP {

class CAnimDictionary;

class CAnimationStar : public IAnimationStar {
public:
    U8 m_unk4[0x40 - 0x4];

public:
    CAnimationStar();
    ~CAnimationStar();

    void Release();
    void SetName(std::string name);
    SAnimationStarValues* GetDefaultAnimationValues();
    void SetAnimDictionary(CAnimDictionary* anim_dictionary);
};
REQUIRE_SIZE(CAnimationStar, 0x40);

}

#endif
