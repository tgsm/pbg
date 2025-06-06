#include <dolphin/pad.h>
#include <dolphin/si.h>
#include "engine/input/CGCNPad.h"

PADStatus DKI::CGCNPad::ms_Datas[PAD_MAX_CONTROLLERS];
PADStatus DKI::CGCNPad::ms_LastDatas[PAD_MAX_CONTROLLERS];
U32 DKI::CGCNPad::ms_Motor[PAD_MAX_CONTROLLERS];
int DKI::CGCNPad::ms_Level[PAD_MAX_CONTROLLERS];

DKI::CGCNPad* DKI::CGCNPad::ms_Pad1;
DKI::CGCNPad* DKI::CGCNPad::ms_Pad2;
DKI::CGCNPad* DKI::CGCNPad::ms_Pad3;
DKI::CGCNPad* DKI::CGCNPad::ms_Pad4;

namespace DKI {

CGCNPad::CGCNPad(int pad_no) {
    memset(&m_unk4, 0, sizeof(UnkSubstruct));
    m_pad_no = pad_no;
    m_not_connected = FALSE;
    m_type = SIProbe(m_pad_no);
    m_vibration_mode = 0;
    m_unk54 = 0;
}

BOOL CGCNPad::Init(DkXmd::CChunkIterator* iter) {
    static BOOL Firstcall = TRUE;

    if (Firstcall) {
        PADInit();
        memset(&ms_Datas, 0, sizeof(ms_Datas));
        memset(&ms_LastDatas, 0, sizeof(ms_LastDatas));
        memset(ms_Motor, 0, sizeof(ms_Motor));
        memset(ms_Level, 0, sizeof(ms_Level));

        Firstcall = FALSE;
    }

    m_type = SIProbe(m_pad_no);

    return TRUE;
}

void CGCNPad::Update() {
    memcpy(&ms_LastDatas, &ms_Datas, sizeof(PADStatus) * PAD_MAX_CONTROLLERS);
    PADRead(&ms_Datas[0]);
    PADClamp(&ms_Datas[0]);
}

void CGCNPad::Update(const F32& dt_maybe) {
    if (m_not_connected) {
        switch (m_pad_no) {
            case 0:
                if (PADReset(PAD_CHAN0_BIT)) {
                    m_not_connected = FALSE;
                }
                break;
            case 1:
                if (PADReset(PAD_CHAN1_BIT)) {
                    m_not_connected = FALSE;
                }
                break;
            case 2:
                if (PADReset(PAD_CHAN2_BIT)) {
                    m_not_connected = FALSE;
                }
                break;
            case 3:
                if (PADReset(PAD_CHAN3_BIT)) {
                    m_not_connected = FALSE;
                }
                break;
        }
        m_type = SIProbe(m_pad_no);
    }

    S8 error = ms_Datas[m_pad_no].err;
    if (error != PAD_ERR_NONE) {
        if (error == PAD_ERR_NOT_READY || error == PAD_ERR_TRANSFER) {
            ms_Datas[m_pad_no] = ms_LastDatas[m_pad_no];
        } else {
            m_not_connected = 1;
        }
    }

    m_unk4.unk8 = ((ms_Datas[m_pad_no].button & PAD_BUTTON_UP) == (ms_LastDatas[m_pad_no].button & PAD_BUTTON_UP)) ? m_unk4.unk8 + dt_maybe : 0.0f;
    m_unk4.unkC = ((ms_Datas[m_pad_no].button & PAD_BUTTON_DOWN) == (ms_LastDatas[m_pad_no].button & PAD_BUTTON_DOWN)) ? m_unk4.unkC + dt_maybe : 0.0f;
    m_unk4.unk4 = ((ms_Datas[m_pad_no].button & PAD_BUTTON_LEFT) == (ms_LastDatas[m_pad_no].button & PAD_BUTTON_LEFT)) ? m_unk4.unk4 + dt_maybe : 0.0f;
    m_unk4.unk0 = ((ms_Datas[m_pad_no].button & PAD_BUTTON_RIGHT) == (ms_LastDatas[m_pad_no].button & PAD_BUTTON_RIGHT)) ? m_unk4.unk0 + dt_maybe : 0.0f;
    m_unk28 = ((ms_Datas[m_pad_no].button & PAD_TRIGGER_Z) == (ms_LastDatas[m_pad_no].button & PAD_TRIGGER_Z)) ? m_unk28 + dt_maybe : 0.0f;
    m_unk2C = ((ms_Datas[m_pad_no].button & PAD_BUTTON_A) == (ms_LastDatas[m_pad_no].button & PAD_BUTTON_A)) ? m_unk2C + dt_maybe : 0.0f;
    m_unk30 = ((ms_Datas[m_pad_no].button & PAD_BUTTON_B) == (ms_LastDatas[m_pad_no].button & PAD_BUTTON_B)) ? m_unk30 + dt_maybe : 0.0f;
    m_unk34 = ((ms_Datas[m_pad_no].button & PAD_BUTTON_X) == (ms_LastDatas[m_pad_no].button & PAD_BUTTON_X)) ? m_unk34 + dt_maybe : 0.0f;
    m_unk38 = ((ms_Datas[m_pad_no].button & PAD_BUTTON_Y) == (ms_LastDatas[m_pad_no].button & PAD_BUTTON_Y)) ? m_unk38 + dt_maybe : 0.0f;
    m_unk14 = ((ms_Datas[m_pad_no].button & PAD_BUTTON_START) == (ms_LastDatas[m_pad_no].button & PAD_BUTTON_START)) ? m_unk14 + dt_maybe : 0.0f;
    m_unk3C = ((ms_Datas[m_pad_no].triggerLeft) == (ms_LastDatas[m_pad_no].triggerLeft)) ? m_unk3C + dt_maybe : 0.0f;
    m_unk18 = ((ms_Datas[m_pad_no].stickX) == (ms_LastDatas[m_pad_no].stickX)) ? m_unk18 + dt_maybe : 0.0f;
    m_unk1C = ((ms_Datas[m_pad_no].stickY) == (ms_LastDatas[m_pad_no].stickY)) ? m_unk1C + dt_maybe : 0.0f;
    m_unk40 = ((ms_Datas[m_pad_no].triggerRight) == (ms_LastDatas[m_pad_no].triggerRight)) ? m_unk40 + dt_maybe : 0.0f;
    m_unk20 = ((ms_Datas[m_pad_no].substickX) == (ms_LastDatas[m_pad_no].substickX)) ? m_unk20 + dt_maybe : 0.0f;
    m_unk24 = ((ms_Datas[m_pad_no].substickY) == (ms_LastDatas[m_pad_no].substickY)) ? m_unk24 + dt_maybe : 0.0f;

    if (m_vibration_mode == 3) {
        int unk = m_unk54 * (1.0f - dt_maybe);
        if (unk <= 0) {
            m_unk54 = 0;
            m_vibration_mode = 1;
        } else {
            m_unk54 = unk;
        }

        SendVibration(m_unk54);
    }
}

CState CGCNPad::GetState(const int& unk) {
    CState ret;

    switch (unk) {
        case 2: {
            ret.m_unk8 = m_unk4.unk8;
            ret.m_unk4 = (ms_Datas[m_pad_no].button & PAD_BUTTON_UP) ? 1.0f : 0.0f;

            if (ret.m_unk4 && !(ms_LastDatas[m_pad_no].button & PAD_BUTTON_UP)) {
                ret.m_unk0 = 1;
            } else if (!ret.m_unk4 && (ms_LastDatas[m_pad_no].button & PAD_BUTTON_UP)) {
                ret.m_unk0 = 0;
            }

            return ret;
        }
        case 3: {
            ret.m_unk8 = m_unk4.unkC;
            ret.m_unk4 = (ms_Datas[m_pad_no].button & PAD_BUTTON_DOWN) ? 1.0f : 0.0f;

            if (ret.m_unk4 && !(ms_LastDatas[m_pad_no].button & PAD_BUTTON_DOWN)) {
                ret.m_unk0 = 1;
            } else if (!ret.m_unk4 && (ms_LastDatas[m_pad_no].button & PAD_BUTTON_DOWN)) {
                ret.m_unk0 = 0;
            }

            return ret;
        }
        case 1: {
            ret.m_unk8 = m_unk4.unk4;
            ret.m_unk4 = (ms_Datas[m_pad_no].button & PAD_BUTTON_LEFT) ? 1.0f : 0.0f;

            if (ret.m_unk4 && !(ms_LastDatas[m_pad_no].button & PAD_BUTTON_LEFT)) {
                ret.m_unk0 = 1;
            } else if (!ret.m_unk4 && (ms_LastDatas[m_pad_no].button & PAD_BUTTON_LEFT)) {
                ret.m_unk0 = 0;
            }

            return ret;
        }
        case 0: {
            ret.m_unk8 = m_unk4.unk0;
            ret.m_unk4 = (ms_Datas[m_pad_no].button & PAD_BUTTON_RIGHT) ? 1.0f : 0.0f;

            if (ret.m_unk4 && !(ms_LastDatas[m_pad_no].button & PAD_BUTTON_RIGHT)) {
                ret.m_unk0 = 1;
            } else if (!ret.m_unk4 && (ms_LastDatas[m_pad_no].button & PAD_BUTTON_RIGHT)) {
                ret.m_unk0 = 0;
            }

            return ret;
        }
        case 18: {
            ret.m_unk8 = m_unk28;
            ret.m_unk4 = (ms_Datas[m_pad_no].button & PAD_TRIGGER_Z) ? 1.0f : 0.0f;

            if (ret.m_unk4 && !(ms_LastDatas[m_pad_no].button & PAD_TRIGGER_Z)) {
                ret.m_unk0 = 1;
            } else if (!ret.m_unk4 && (ms_LastDatas[m_pad_no].button & PAD_TRIGGER_Z)) {
                ret.m_unk0 = 0;
            }

            return ret;
        }
        case 19: {
            ret.m_unk8 = m_unk2C;
            ret.m_unk4 = (ms_Datas[m_pad_no].button & PAD_BUTTON_A) ? 1.0f : 0.0f;

            if (ret.m_unk4 && !(ms_LastDatas[m_pad_no].button & PAD_BUTTON_A)) {
                ret.m_unk0 = 1;
            } else if (!ret.m_unk4 && (ms_LastDatas[m_pad_no].button & PAD_BUTTON_A)) {
                ret.m_unk0 = 0;
            }

            return ret;
        }
        case 20: {
            ret.m_unk8 = m_unk30;
            ret.m_unk4 = (ms_Datas[m_pad_no].button & PAD_BUTTON_B) ? 1.0f : 0.0f;

            if (ret.m_unk4 && !(ms_LastDatas[m_pad_no].button & PAD_BUTTON_B)) {
                ret.m_unk0 = 1;
            } else if (!ret.m_unk4 && (ms_LastDatas[m_pad_no].button & PAD_BUTTON_B)) {
                ret.m_unk0 = 0;
            }

            return ret;
        }
        case 21: {
            ret.m_unk8 = m_unk34;
            ret.m_unk4 = (ms_Datas[m_pad_no].button & PAD_BUTTON_X) ? 1.0f : 0.0f;

            if (ret.m_unk4 && !(ms_LastDatas[m_pad_no].button & PAD_BUTTON_X)) {
                ret.m_unk0 = 1;
            } else if (!ret.m_unk4 && (ms_LastDatas[m_pad_no].button & PAD_BUTTON_X)) {
                ret.m_unk0 = 0;
            }

            return ret;
        }
        case 22: {
            ret.m_unk8 = m_unk38;
            ret.m_unk4 = (ms_Datas[m_pad_no].button & PAD_BUTTON_Y) ? 1.0f : 0.0f;

            if (ret.m_unk4 && !(ms_LastDatas[m_pad_no].button & PAD_BUTTON_Y)) {
                ret.m_unk0 = 1;
            } else if (!ret.m_unk4 && (ms_LastDatas[m_pad_no].button & PAD_BUTTON_Y)) {
                ret.m_unk0 = 0;
            }

            return ret;
        }
        case 12: {
            ret.m_unk8 = m_unk14;
            ret.m_unk4 = (ms_Datas[m_pad_no].button & PAD_BUTTON_START) ? 1.0f : 0.0f;

            if (ret.m_unk4 && !(ms_LastDatas[m_pad_no].button & PAD_BUTTON_START)) {
                ret.m_unk0 = 1;
            } else if (!ret.m_unk4 && (ms_LastDatas[m_pad_no].button & PAD_BUTTON_START)) {
                ret.m_unk0 = 0;
            }

            return ret;
        }
        case 23: {
            ret.m_unk8 = m_unk3C;
            ret.m_unk4 = ms_Datas[m_pad_no].triggerLeft / 150.0;
            if (ms_LastDatas[m_pad_no].triggerLeft < ms_Datas[m_pad_no].triggerLeft) {
                ret.m_unk0 = 1;
            } else if (ms_LastDatas[m_pad_no].triggerLeft > ms_Datas[m_pad_no].triggerLeft) {
                ret.m_unk0 = 0;
            }
            return ret;
        }
        case 24: {
            ret.m_unk8 = m_unk40;
            ret.m_unk4 = ms_Datas[m_pad_no].triggerRight / 150.0;
            if (ms_LastDatas[m_pad_no].triggerRight < ms_Datas[m_pad_no].triggerRight) {
                ret.m_unk0 = 1;
            } else if (ms_LastDatas[m_pad_no].triggerRight > ms_Datas[m_pad_no].triggerRight) {
                ret.m_unk0 = 0;
            }
            return ret;
        }
        case 14: {
            U32 value;
            ret.m_unk8 = m_unk18;
            ret.m_unk4 = ms_Datas[m_pad_no].stickX / 40.0;
            F32 old_stick_x = ms_LastDatas[m_pad_no].stickX / 40.0;
            if (ret.m_unk4 > 0.0f) {
                if (old_stick_x < 0.5f && ret.m_unk4 > 0.5f) {
                    value = 1;
                } else if (old_stick_x > 0.5f && ret.m_unk4 < 0.5f) {
                    value = 0;
                } else {
                    goto why1;
                }
            } else if (old_stick_x < -0.5f && ret.m_unk4 > -0.5f) {
                value = 0;
            } else if (old_stick_x > -0.5f && ret.m_unk4 < -0.5f) {
                value = 1;
            } else {
why1:
                value = 0;
            }
            ret.m_unk0 = value;
            return ret;
        }
        case 15: {
            U32 value;
            ret.m_unk8 = m_unk1C;
            ret.m_unk4 = ms_Datas[m_pad_no].stickY / 40.0;
            F32 old_stick_y = ms_LastDatas[m_pad_no].stickY / 40.0;
            if (ret.m_unk4 > 0.0f) {
                if (old_stick_y < 0.5f && ret.m_unk4 > 0.5f) {
                    value = 1;
                } else if (old_stick_y > 0.5f && ret.m_unk4 < 0.5f) {
                    value = 0;
                } else {
                    goto why2;
                }
            } else if (old_stick_y < -0.5f && ret.m_unk4 > -0.5f) {
                value = 0;
            } else if (old_stick_y > -0.5f && ret.m_unk4 < -0.5f) {
                value = 1;
            } else {
why2:
                value = 0;
            }
            ret.m_unk0 = value;
            return ret;
        }
        case 16: {
            U32 value;
            ret.m_unk8 = m_unk20;
            ret.m_unk4 = ms_Datas[m_pad_no].substickX / 31.0;
            F32 old_substick_x = ms_LastDatas[m_pad_no].substickX / 31.0;
            if (ret.m_unk4 > 0.0f) {
                if (old_substick_x < 0.5f && ret.m_unk4 > 0.5f) {
                    value = 1;
                } else if (old_substick_x > 0.5f && ret.m_unk4 < 0.5f) {
                    value = 0;
                } else {
                    goto why3;
                }
            } else if (old_substick_x < -0.5f && ret.m_unk4 > -0.5f) {
                value = 0;
            } else if (old_substick_x > -0.5f && ret.m_unk4 < -0.5f) {
                value = 1;
            } else {
why3:
                value = 0;
            }
            ret.m_unk0 = value;
            return ret;
        }
        case 17: {
            U32 value;
            ret.m_unk8 = m_unk24;
            ret.m_unk4 = ms_Datas[m_pad_no].substickY / 31.0;
            F32 old_substick_y = ms_LastDatas[m_pad_no].substickY / 31.0;
            if (ret.m_unk4 > 0.0f) {
                if (old_substick_y < 0.5f && ret.m_unk4 > 0.5f) {
                    value = 1;
                } else if (old_substick_y > 0.5f && ret.m_unk4 < 0.5f) {
                    value = 0;
                } else {
                    goto why4;
                }
            } else if (old_substick_y < -0.5f && ret.m_unk4 > -0.5f) {
                value = 0;
            } else if (old_substick_y > -0.5f && ret.m_unk4 < -0.5f) {
                value = 1;
            } else {
why4:
                value = 0;
            }
            ret.m_unk0 = value;
            return ret;
        }
        default:
            return ret;
    }
}

void CGCNPad::SetRumble(const U8& a1, const U8& a2) {
    BOOL control = FALSE;
    if (a2 > 230 && ms_Motor[m_pad_no] != 1) {
        ms_Motor[m_pad_no] = 1;
        control = TRUE;
    } else if (a2 < 10 && ms_Motor[m_pad_no] != 0) {
        ms_Motor[m_pad_no] = 0;
        control = TRUE;
    } else {
        ms_Level[m_pad_no] += a2;
        if (ms_Level[m_pad_no] > 230) {
            ms_Level[m_pad_no] = 0;
            if (ms_Motor[m_pad_no] != 1) {
                ms_Motor[m_pad_no] = 1;
                control = TRUE;
            }
        } else {
            if (ms_Motor[m_pad_no] != 0) {
                ms_Motor[m_pad_no] = 0;
                control = TRUE;
            }
        }
    }

    if (control) {
        PADControlMotor(m_pad_no, ms_Motor[m_pad_no]);
    }
}

}
