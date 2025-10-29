#ifndef ENGINE_PHYSICS_DKPH_DYNAMICS_H
#define ENGINE_PHYSICS_DKPH_DYNAMICS_H

#include "engine/wrap/DKW_V3d.h"

namespace DkPh {

class Dynamics {
public:
    Dynamics() : m_unk4(-1), m_unk50(0.0f, 0.0f, 0.0f), m_unk5C(0.0f, 0.0f, 0.0f), m_unk68(0.0f, 0.0f, 0.0f) {
        m_unk74 = 0;
        m_unk8 = &m_unkC;
    }
    ~Dynamics();

    virtual void Accelerate();
    virtual void ApplyFriction();

public:
    int m_unk4;
    void* m_unk8; // Unknown type
    void* m_unkC; // Unknown type
    U8 m_unk10[0x50 - 0x10];
    CDKW_V3d m_unk50;
    CDKW_V3d m_unk5C;
    CDKW_V3d m_unk68;
    U32 m_unk74;
}; // Unknown size

}

#endif
