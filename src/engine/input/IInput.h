#ifndef ENGINE_INPUT_IINPUT_H
#define ENGINE_INPUT_IINPUT_H

#include <cstring>
#include "engine/input/CState.h"
#include "engine/xmd/CChunkIterator.h"

namespace DKI {

class IDevice {
public:
    virtual ~IDevice() {}

    virtual BOOL Init(DkXmd::CChunkIterator* iter) = 0;
    virtual void Update(const F32& dt_maybe) = 0;
    virtual CState GetState(const int&) = 0;
    virtual BOOL IsConnected() = 0;
    virtual U32 GetType() = 0;

    virtual void SetRumble(const U8& a1, const U8& a2) {}
    virtual BOOL VibrationSupported() { return FALSE; }
    virtual void VibrationEnable() {}
    virtual BOOL IsVibrationModeEnable() { return FALSE; }
    virtual void VibrationDisable() {}
    virtual void SendVibration(U8 a1) {}
    virtual void StopVibration() {}
};

class IInput {
protected:
    char* m_name;

public:
    IInput(char* name) {
        m_name = new char[strlen(name) + 1];
        strcpy(m_name, name);
        m_name[strlen(name)] = '\0';
    }
    virtual ~IInput() {
        if (m_name != NULL) {
            delete[] m_name;
        }
    }

    virtual CState GetState() = 0;
    virtual BOOL IsConnected() = 0;

    char* GetName() { return m_name; }
};
REQUIRE_SIZE(IInput, 0x8);

}

#endif
