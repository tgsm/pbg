#include "engine/input/CInputHalfAxe.h"

namespace DKI {

CInputHalfAxe::CInputHalfAxe(char* name, IDevice* device, int a3, int a4) : IInput(name) {
    m_device = device;
    m_unkC = a3;
    m_unk10 = a4;
}

CState CInputHalfAxe::GetState() {
    CState state = m_device->GetState(m_unkC);

    if (m_unk10 != 0) {
        state.m_unk4 = (state.m_unk4 < 0.0f) ? 0.0f : state.m_unk4;
    } else {
        state.m_unk4 = (state.m_unk4 < 0.0f) ? 1.0f : 0.0f;
    }

    return state;
}

}
