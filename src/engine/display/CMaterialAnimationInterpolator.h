#ifndef ENGINE_DISPLAY_CMATERIALANIMATIONINTERPOLATOR_H
#define ENGINE_DISPLAY_CMATERIALANIMATIONINTERPOLATOR_H

#include "types.h"

namespace DKDSP {

// TODO
class CMaterialAnimationInterpolator {
public:
    U8 m_unk0[0x28];

public:
    CMaterialAnimationInterpolator();
};
REQUIRE_SIZE(CMaterialAnimationInterpolator, 0x28);

}

#endif
