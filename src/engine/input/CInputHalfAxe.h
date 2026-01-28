#ifndef ENGINE_INPUT_CINPUTHALFAXE_H
#define ENGINE_INPUT_CINPUTHALFAXE_H

#include "engine/input/IInput.h"

namespace DKI {

class CInputHalfAxe : public IInput {
public:
    IDevice* m_device;
    int m_unkC;
    int m_unk10;

public:
    CInputHalfAxe(char* name, IDevice* device, int, int);
    virtual ~CInputHalfAxe() {}

    virtual CState GetState();
    virtual BOOL IsConnected() { return m_device->IsConnected(); }
};
REQUIRE_SIZE(CInputHalfAxe, 0x14);

}

#endif
