#include "engine/display/CTimerGCN.h"
#include "engine/wrap/DKW_GCNEngine.h"

namespace DKDSP {

F64 CTimerGCN::GetPlatformTime() {
    return (F64)CDKW_GCNEngine::OSGetTime() / OS_TIMER_CLOCK;
}

F64 CTimerGCN::GetPlatformDeltaTime() {
    F64 current_time = CTimerGCN::GetPlatformTime();
    F64 old_time = m_unk38;
    F64 delta = current_time - old_time;
    m_unk38 = current_time;
    return delta;
}

}
