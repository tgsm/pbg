#ifndef ENGINE_INPUT_CCOMBO_H
#define ENGINE_INPUT_CCOMBO_H

#include "engine/input/IInput.h"

namespace DKI {

class CCombo : public IInput {
public:
    IInput* m_unk8;
    IInput* m_unkC;
    int m_unk10;

public:
    CCombo(char* name, IInput*, IInput*, int);
    virtual ~CCombo() {
        m_unk8 = NULL;
        m_unkC = NULL;
    }

    virtual CState GetState();
    virtual BOOL IsConnected() { // Equivalent
        return m_unk8->IsConnected() || m_unkC->IsConnected();
    }
};

}

#endif
