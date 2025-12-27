#include "engine/input/CState.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

namespace DKI {

CState::CState() {
    m_unk0 = FALSE;
    m_unk4 = 0.0f;
    m_unk8 = 0.0f;
}

CState CState::operator&&(const CState& other) const {
    CState ret;

    ret.m_unk4 = (m_unk4 && other.m_unk4) ? 1.0f : 0.0f;
    ret.m_unk0 = (m_unk0 == other.m_unk0) ? m_unk0 : FALSE;

    if (ret.m_unk4) {
        ret.m_unk8 = MIN(m_unk8, other.m_unk8);
    } else if (m_unk4 != other.m_unk4) {
        ret.m_unk8 = (!m_unk4) ? m_unk8 : other.m_unk8;
    } else {
        ret.m_unk8 = MAX(m_unk8, other.m_unk8);
    }

    return ret;
}

CState CState::operator||(const CState& other) const {
    CState ret;

    ret.m_unk4 = (m_unk4 || other.m_unk4) ? 1.0f : 0.0f;
    if (ret.m_unk4) {
        ret.m_unk0 = (m_unk0 == TRUE || other.m_unk0 == TRUE) ? TRUE : FALSE;
        if (m_unk4 != other.m_unk4) {
            ret.m_unk8 = (m_unk4) ? m_unk8 : other.m_unk8;
        } else {
            ret.m_unk8 = MAX(m_unk8, other.m_unk8);
        }
    } else {
        ret.m_unk0 = (m_unk0 == FALSE || other.m_unk0 == FALSE) ? FALSE : FALSE; // what
        ret.m_unk8 = MIN(m_unk8, other.m_unk8);
    }

    return ret;
}

// Incomplete: branching, regalloc
CState CState::operator^(const CState& other) const {
    CState ret;
    volatile F32 sp10;
    volatile F32 spC;
    volatile BOOL sp8;
    F32 var_f4;
    F32 temp_f7;
    F32 var_f0;
    F32 var_f0_2;
    F32 var_f1;
    F32 var_f1_2;
    F32 var_f2;
    F32 var_f3;
    S32 var_r5;
    BOOL var_r6;
    BOOL temp_r7;

    var_f1 = (other.m_unk4) ? 0.0f : 1.0f;
    if (other.m_unk0 == TRUE) {
        var_r6 = FALSE;
    } else if (other.m_unk0 == FALSE) {
        var_r6 = TRUE;
    } else {
        var_r6 = FALSE;
    }
    temp_f7 = other.m_unk8;
    var_f2 = (m_unk4 && var_f1) ? 1.0f : 0.0f;
    temp_r7 = m_unk0;
    var_r5 = (temp_r7 == var_r6) ? temp_r7 : 0;
    if (var_f2) {
        ret.m_unk8 = MIN(m_unk8, temp_f7);
    } else if (m_unk4 != var_f1) {
        ret.m_unk8 = (!m_unk4) ? m_unk8 : temp_f7;
    } else {
        ret.m_unk8 = MAX(m_unk8, temp_f7);
    }
    var_f1_2 = (m_unk4) ? 0.0f : 1.0f;
    if (temp_r7 == TRUE) {
        ret.m_unk0 = FALSE;
    } else if (temp_r7 == FALSE) {
        ret.m_unk0 = TRUE;
    } else {
        ret.m_unk0 = FALSE;
    }
    var_f4 = this->m_unk8;
    var_f3 = (var_f1_2 && other.m_unk4) ? 1.0f : 0.0f;
    ret.m_unk0 = (ret.m_unk0 == other.m_unk0) ? other.m_unk0 : FALSE;
    if (var_f3) {
        var_f4 = MIN(var_f4, temp_f7);
    } else if (var_f1_2 != other.m_unk4) {
        var_f4 = (!var_f1_2) ? var_f4 : temp_f7;
    } else if (var_f4 > temp_f7) {
        // Here
        var_f4 = MAX(var_f4, temp_f7);
    }

    // calls operator|| from here?
    sp8 = ret.m_unk0;
    spC = var_f3;
    sp10 = var_f4;
    ret.m_unk4 = ((F32)(F64)var_f3 || var_f2) ? 1.0f : 0.0f;
    if (ret.m_unk4) {
        ret.m_unk0 = (sp8 == TRUE || var_r5 == TRUE) ? TRUE : FALSE;
        F32 lol = spC;
        if (lol != var_f2) {
            if (lol) {
                ret.m_unk8 = sp10;
            }
        } else {
            var_f0 = sp10;
            if (var_f0 > ret.m_unk8) {
                ret.m_unk8 = MAX(var_f0, ret.m_unk8);
            } else {
                var_f0 = ret.m_unk8;
            }
            ret.m_unk8 = var_f0;
        }
    } else {
        ret.m_unk0 = (sp8 == FALSE || var_r5 == FALSE) ? FALSE : FALSE; // what
        var_f0_2 = sp10;
        if (var_f0_2 < ret.m_unk8) {
            ret.m_unk8 = MIN(var_f0_2, ret.m_unk8);
        } else {
            var_f0_2 = ret.m_unk8;
        }
        ret.m_unk8 = var_f0_2;
    }
    return ret;
}

CState CState::operator-(const CState& other) const {
    CState ret;

    ret.m_unk4 = m_unk4 - other.m_unk4;

    if (m_unk0 == other.m_unk0) {
        ret.m_unk0 = FALSE;
    } else if (ret.m_unk4) {
        ret.m_unk0 = TRUE;
    } else {
        ret.m_unk0 = FALSE;
    }

    ret.m_unk0 = ret.m_unk0;
    ret.m_unk8 = (ret.m_unk0) ? 0.0f : (m_unk8 < other.m_unk8) ? m_unk8 : other.m_unk8;

    return ret;
}

}
