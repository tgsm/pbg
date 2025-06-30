#ifndef ENGINE_INPUT_CGCNPAD_H
#define ENGINE_INPUT_CGCNPAD_H

#include <dolphin/pad.h>
#include "engine/input/IInput.h"

namespace DKI {

struct UnkSubstruct {
    F32 unk0;
    F32 unk4;
    F32 unk8;
    F32 unkC;
};
REQUIRE_SIZE(UnkSubstruct, 0x10);

class CGCNPad : public IDevice {
private:
    UnkSubstruct m_unk4;
    F32 m_unk14;
    F32 m_unk18;
    F32 m_unk1C;
    F32 m_unk20;
    F32 m_unk24;
    F32 m_unk28;
    F32 m_unk2C;
    F32 m_unk30;
    F32 m_unk34;
    F32 m_unk38;
    F32 m_unk3C;
    F32 m_unk40;
    int m_pad_no;
    U32 m_type;
    BOOL m_not_connected;
    int m_vibration_mode;
    U8 m_unk54;

public:
    CGCNPad(int pad_no);

    virtual BOOL Init(DkXmd::CChunkIterator* iter);
    virtual void Update(const F32& dt_maybe);
    virtual CState GetState(const int&);
    virtual void SetRumble(const U8& a1, const U8& a2);
    virtual BOOL IsConnected() {
        if (!m_not_connected) {
            return TRUE;
        } else {
            return FALSE;
        }
    }
    virtual U32 GetType() {
        return m_type;
    }
    virtual BOOL VibrationSupported() { return TRUE; }
    virtual void VibrationEnable() {
        m_vibration_mode = 1;
    }
    virtual BOOL IsVibrationModeEnable() {
        return m_vibration_mode != 0;
    }
    virtual void VibrationDisable() {
        m_vibration_mode = 0;
        SendVibration(0);
    }
    virtual void SendVibration(U8 unk) {
        if (m_vibration_mode != 0) {
            m_vibration_mode = 3;
            SetRumble(0, unk);
            m_unk54 = unk;
        }
    }
    virtual void StopVibration() {
        if (m_vibration_mode != 0) {
            SetRumble(0, 0);
        }
    }

    static void Update();

    static PADStatus ms_Datas[4];
    static PADStatus ms_LastDatas[4];
    static U32 ms_Motor[4];
    static int ms_Level[4];

    static CGCNPad* ms_Pad1;
    static CGCNPad* ms_Pad2;
    static CGCNPad* ms_Pad3;
    static CGCNPad* ms_Pad4;
};
REQUIRE_SIZE(CGCNPad, 0x58);

}

#endif
