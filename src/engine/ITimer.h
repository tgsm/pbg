#pragma once on

#include "types.h"

namespace DKDSP {

class ITimer {
public:
    ITimer();

    virtual void Release() = 0;
    virtual f64 GetDeltaTime() = 0;
    virtual f64 GetTime() = 0;
    virtual void Save() = 0;
    virtual f64 GetDeltaSinceSaved() = 0;
    virtual void Reset() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
};

}
