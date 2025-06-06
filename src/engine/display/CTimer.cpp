#include "display/CTimer.h"

namespace DKDSP {

CTimer::CTimer(F64 unk) {
    m_unk8 = 0.0;
    m_unk10 = 0.0;
    m_unk18 = 0.0;
    m_saved_time = 0.0;
    m_unk28 = unk;
    m_paused = FALSE;
    m_engine = NULL;
}

CTimer::~CTimer() {
    Release();
}

void CTimer::Release() {
    m_unk8 = 0.0;
    m_unk10 = 0.0;
    m_unk18 = 0.0;
    m_saved_time = 0.0;
    m_unk28 = 0.0;
    m_paused = FALSE;
    m_engine = NULL;
}

void CTimer::SetEngine(CEngine* engine) {
    m_engine = engine;
}

F64 CTimer::GetDeltaTime() {
    if (!m_paused) {
        m_unk18 = GetPlatformDeltaTime();
        return m_unk18;
    } else {
        return 0.0;
    }
}

F64 CTimer::GetTime() {
    if (!m_paused) {
        F64 unk = GetPlatformTime();
        m_unk18 = unk - m_unk28;
        m_unk28 = unk;
        m_unk8 += m_unk18;
    }
    return m_unk8 - m_unk10;
}

void CTimer::Save() {
    m_saved_time = GetTime();
}

F64 CTimer::GetDeltaSinceSaved() {
    return GetTime() - m_saved_time;
}

void CTimer::Reset() {
    m_unk10 = m_unk8;
}

void CTimer::Pause() {
    m_paused = TRUE;
}

void CTimer::Resume() {
    if (m_paused == TRUE) {
        m_unk28 = GetPlatformTime();
        F64 unused_dt = GetPlatformDeltaTime();
    }
    m_paused = FALSE;
}

}
