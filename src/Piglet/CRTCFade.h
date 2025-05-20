#ifndef PIGLET_CRTCFADE_H
#define PIGLET_CRTCFADE_H

#include "types.h"

class CRTCFade {
private:
    u32 m_unk0;
    u32 m_unk4;
    u32 m_unk8;
    u32 m_unkC;

public:
    CRTCFade();
    ~CRTCFade();
};
REQUIRE_SIZE(CRTCFade, 0x10);

#endif
