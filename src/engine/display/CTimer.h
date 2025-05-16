#ifndef ENGINE_DISPLAY_CTIMER_H
#define ENGINE_DISPLAY_CTIMER_H

#include "engine/display/ITimer.h"

namespace DKDSP {

class CEngine;

class CTimer : public ITimer {
private:
    u32 m_unk4;
    f64 m_unk8;
    f64 m_unk10;
    f64 m_unk18;
    f64 m_saved_time;
    f64 m_unk28;
    BOOL m_paused;
    CEngine* m_engine;

public:
    CTimer(f64);
    ~CTimer();

    virtual void Release();
    virtual f64 GetDeltaTime();
    virtual f64 GetTime();
    virtual void Save();
    virtual f64 GetDeltaSinceSaved();
    virtual void Reset();
    virtual void Pause();
    virtual void Resume();
    virtual f32 GetPlatformTime() = 0;
    virtual f32 GetPlatformDeltaTime() = 0;

    void SetEngine(CEngine* engine);
};
REQUIRE_SIZE(CTimer, 0x38);

}

#endif
