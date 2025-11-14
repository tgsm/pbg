#ifndef ENGINE_PHYSICS_DKPH_DYNAMICS_H
#define ENGINE_PHYSICS_DKPH_DYNAMICS_H

#include "engine/physics/DkPh_Primitives.h"
#include "engine/wrap/DKW_V3d.h"

namespace DkPh {

class Dynamics {
public:
    struct DkMoveParams {
        F32 acceleration_amount;
        F32 unk4;
        F32 unk8;
        F32 unkC;
    };

    Dynamics() : m_unk4(-1), m_unk50(0.0f, 0.0f, 0.0f), m_unk5C(0.0f, 0.0f, 0.0f), m_unk68(0.0f, 0.0f, 0.0f) {
        m_rbody = NULL;
        mp_move_params = &m_move_params;
    }
    ~Dynamics();

    virtual void Accelerate();
    virtual void ApplyFriction();

    Dynamics& SetMoveParams(F32, F32, F32);
    void SetAccelAmount(F32);
    Dynamics& Move(RBody& body, F32);

public:
    int m_unk4;
    DkMoveParams* mp_move_params;
    DkMoveParams m_move_params;
    U8 m_unk1C[0x4C - 0x1C];
    F32 m_unk4C;
    CDKW_V3d m_unk50;
    CDKW_V3d m_unk5C;
    CDKW_V3d m_unk68;
    RBody* m_rbody;
}; // Unknown size

}

#endif
