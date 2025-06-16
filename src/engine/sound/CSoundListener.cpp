#include "engine/sound/CSoundListener.h"

namespace DKSND {

CSoundListener::CSoundListener() {
    Clear();
    CreateListenerSpecific();
}

CSoundListener::~CSoundListener() {
    Destroy();
}

void CSoundListener::Clear() {
    m_pPosition = &m_position;
    m_pUp = &m_up;
    m_pFront = &m_front;
    m_position = CDKW_V3d::ZERO;
    m_up = CDKW_V3d::YAXIS;
    m_front = CDKW_V3d::ZAXIS;
    m_attenuation = 1.0f;
}

void CSoundListener::SetPosition(CDKW_V3d* position) {
    m_pPosition = position;
}

void CSoundListener::SetPosition(CDKW_V3d position) {
    m_position = position;
    m_pPosition = &m_position;
}

CDKW_V3d* CSoundListener::GetPosition() {
    return m_pPosition;
}

void CSoundListener::Update() {
    UpdateListenerSpecific();
}

void CSoundListener::SetUpVector(CDKW_V3d* up) {
    m_pUp = up;
}

void CSoundListener::SetUpVector(CDKW_V3d up) {
    m_up = up;
    m_pUp = &m_up;
}

CDKW_V3d* CSoundListener::GetUpVector() {
    return m_pUp;
}

void CSoundListener::SetFrontVector(CDKW_V3d* front) {
    m_pFront = front;
}

void CSoundListener::SetFrontVector(CDKW_V3d front) {
    m_front = front;
    m_pFront = &m_front;
}

CDKW_V3d* CSoundListener::GetFrontVector() {
    return m_pFront;
}

void CSoundListener::SetAttenuation(F32 attenuation) {
    m_attenuation = attenuation;
}

F32 CSoundListener::GetAttenuation() {
    return m_attenuation;
}

void CSoundListener::Destroy() {
    ReleaseListenerSpecific();
}

}
