#ifndef PIGLET_CRTCFADE_H
#define PIGLET_CRTCFADE_H

#include "types.h"

class CRTCFade {
public:
    u32 m_type;
    u32 m_duration;
    u32 m_unk8;
    u32 m_color_maybe; // 0 if white

public:
    CRTCFade();
    ~CRTCFade();
};
REQUIRE_SIZE(CRTCFade, 0x10);

#endif
