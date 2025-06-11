#ifndef ENGINE_DISPLAY_CTIMER_H
#define ENGINE_DISPLAY_CTIMER_H

#include "engine/display/ITimer.h"

namespace DKDSP {

class CEngine;

class CTimer : public ITimer {
private:
    U32 m_unk4;
    F64 m_unk8;
    F64 m_unk10;
    F64 m_unk18;
    F64 m_saved_time;
    F64 m_unk28;
    BOOL m_paused;
    CEngine* m_engine;

public:
    CTimer(F64);
    ~CTimer();

    virtual void Release();
    virtual F64 GetDeltaTime();
    virtual F64 GetTime();
    virtual void Save();
    virtual F64 GetDeltaSinceSaved();
    virtual void Reset();
    virtual void Pause();
    virtual void Resume();
    virtual F64 GetPlatformTime() = 0;
    virtual F64 GetPlatformDeltaTime() = 0;

    void SetEngine(CEngine* engine);
};
REQUIRE_SIZE(CTimer, 0x38);

}

#endif
