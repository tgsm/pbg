#ifndef ENGINE_DISPLAY_CTIMERGCN_H
#define ENGINE_DISPLAY_CTIMERGCN_H

#include "engine/display/CTimer.h"

namespace DKDSP {

class CTimerGCN : public CTimer {
private:
    F64 m_unk38;

public:
    CTimerGCN();

    virtual F64 GetPlatformTime();
    virtual F64 GetPlatformDeltaTime();
};
REQUIRE_SIZE(CTimerGCN, 0x40);

}

#endif
