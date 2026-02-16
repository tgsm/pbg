#ifndef ENGINE_DISPLAY_CTIMERGCN_H
#define ENGINE_DISPLAY_CTIMERGCN_H

#include <dolphin/os.h>
#include "engine/display/CTimer.h"
#include "engine/wrap/DKW_GCNEngine.h"

namespace DKDSP {

class CTimerGCN : public CTimer {
private:
    F64 m_unk38;

public:
    CTimerGCN(F64 a1) : CTimer(a1) {
        m_unk38 = (F64)CDKW_GCNEngine::OSGetTime() / OS_TIMER_CLOCK;
    }

    virtual F64 GetPlatformTime();
    virtual F64 GetPlatformDeltaTime();
};
REQUIRE_SIZE(CTimerGCN, 0x40);

}

#endif
