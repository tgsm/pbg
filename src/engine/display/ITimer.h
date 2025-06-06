#ifndef ENGINE_DISPLAY_ITIMER_H
#define ENGINE_DISPLAY_ITIMER_H

#include "types.h"

namespace DKDSP {

class ITimer {
public:
    ITimer();

    virtual void Release() = 0;
    virtual F64 GetDeltaTime() = 0;
    virtual F64 GetTime() = 0;
    virtual void Save() = 0;
    virtual F64 GetDeltaSinceSaved() = 0;
    virtual void Reset() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
};
REQUIRE_SIZE(ITimer, 0x4);

}

#endif
