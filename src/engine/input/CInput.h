#ifndef ENGINE_INPUT_CINPUT_H
#define ENGINE_INPUT_CINPUT_H

#include "engine/input/IInput.h"

namespace DKI {

class CInput : public IInput {
private:
    IDevice* m_device;
    int m_unkC;

public:
    CInput(char* name, IDevice* device, int a3);
    virtual ~CInput() {
        m_device = NULL;
    }

    virtual CState GetState() {
        return m_device->GetState(m_unkC);
    }

    virtual BOOL IsConnected() {
        return m_device->IsConnected();
    }
};
REQUIRE_SIZE(CInput, 0x10);

}

#endif
