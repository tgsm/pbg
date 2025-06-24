#ifndef ENGINE_INPUT_CSTATE_H
#define ENGINE_INPUT_CSTATE_H

#include "types.h"

namespace DKI {

class CState {
public:
    BOOL m_unk0;
    F32 m_unk4;
    F32 m_unk8;

public:
    CState();

    CState operator-(const CState& other) const;
    CState operator^(const CState& other) const;
    CState operator||(const CState& other) const;
    CState operator&&(const CState& other) const;
};

#endif

}
