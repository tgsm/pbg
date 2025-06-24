#include "engine/input/CCombo.h"

namespace DKI {

CCombo::CCombo(char* name, IInput* a2, IInput* a3, int a4) : IInput(name) {
    m_unk8 = a2;
    m_unkC = a3;
    m_unk10 = a4;
}

CState CCombo::GetState() {
    switch (m_unk10) {
        case 0:
            return m_unk8->GetState() && m_unkC->GetState();
        case 1:
            return m_unk8->GetState() || m_unkC->GetState();
        case 2:
            return m_unk8->GetState() ^ m_unkC->GetState();
        case 3:
            return m_unk8->GetState() - m_unkC->GetState();
        default:
            return m_unk8->GetState();
    }
}

}
