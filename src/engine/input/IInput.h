#ifndef ENGINE_INPUT_IINPUT_H
#define ENGINE_INPUT_IINPUT_H

#include <cstring>
#include "engine/xmd/CChunkIterator.h"

namespace DKI {

class IDevice {
public:
    virtual ~IDevice();

    virtual void Init(DkXmd::CChunkIterator* iter) = 0;
    virtual void Update(const f32& dt_maybe) = 0;
    virtual u32 GetState(const int&) = 0;
    virtual BOOL IsConnected() = 0;
    virtual u32 GetType() = 0;

    virtual void SetRumble(const u8& a1, const u8& a2) {}
    virtual BOOL VibrationSupported() { return FALSE; }
    virtual void VibrationEnable() {}
    virtual BOOL IsVibrationModeEnable() { return FALSE; }
    virtual void VibrationDisable() {}
    virtual void SendVibration(u8 a1) {}
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

    virtual u32 GetState() = 0;
    virtual BOOL IsConnected() = 0;
};
REQUIRE_SIZE(IInput, 0x8);

}

#endif
