#include "engine/physics/DkPh_Dynamics.h"

namespace DkPh {

Dynamics::~Dynamics() {
    if (m_rbody != NULL) {
        m_rbody->dynamics = NULL;
    }
    m_rbody = NULL;
}

Dynamics& Dynamics::SetMoveParams(F32 a1, F32 a2, F32 a3) {
    mp_move_params[m_unk4].acceleration_amount = a1 * a2;
    mp_move_params[m_unk4].unk4 = a2;
    mp_move_params[m_unk4].unk8 = a3;
    mp_move_params[m_unk4].unkC = a1;
    return *this;
}

void Dynamics::SetAccelAmount(F32 amount) {
    mp_move_params[m_unk4].acceleration_amount = amount;
}

// Equivalent: vector positions on stack
Dynamics& Dynamics::Move(RBody& body, F32 a1) {
    if (m_rbody != NULL) {
        m_rbody->dynamics = NULL;
    }
    m_rbody = &body;
    m_rbody->dynamics = this;
    m_unk4C = a1;
    volatile CDKW_V3d local_14 = m_rbody->unk8;

    ApplyFriction();
    Accelerate();
    CDKW_V3d local_20 = (m_rbody->unk8 - local_14);
    m_rbody->unk14 = local_20 / m_unk4C;
    m_rbody->unk8 = local_14;

    m_rbody->unk14 += m_unk5C;

    return *this;
}

// Incomplete
void Dynamics::ApplyFriction() {
    CDKW_V3d local_30 = m_rbody->unk8;
    F32 dVar4 = RwV3dLength(&local_30);
    F32 dVar5 = local_30.y;
    if (dVar4 < 0.01f) {
        local_30 = CDKW_V3d::ZERO;
    } else {
        DkMoveParams* params = &mp_move_params[m_unk4];
        F32* param = &mp_move_params[m_unk4].unk8;
        if (dVar4 > *param) {
            local_30 -= local_30 * m_unk4C * params->unk4;
        } else {
            local_30 -= local_30 / dVar4 * *param * m_unk4C * params->unk4;
        }
    }

    if (m_rbody->unk8.x * local_30.x + m_rbody->unk8.y * local_30.y + m_rbody->unk8.z * local_30.z < 0.0f) {
        local_30 = CDKW_V3d::ZERO;
    }

    m_rbody->unk8 = local_30;
}

// Equivalent: vector copy
void Dynamics::Accelerate() {
    CDKW_V3d local_14 = m_rbody->unk8;
    CDKW_V3d local_2c = m_unk50 * mp_move_params[m_unk4].unkC;
    CDKW_V3d local_20 = local_2c - local_14;
    F32 dVar3 = RwV3dLength(&local_20);
    DkMoveParams* params = &mp_move_params[m_unk4];
    F32 dVar4 = m_unk4C;
    dVar4 *= params->acceleration_amount;
    dVar4 *= params->unkC;
    if (dVar3 > dVar4) {
        local_20 *= dVar4 / dVar3;
    }

    local_14 += local_20;
    m_rbody->unk8 = local_14;
}

}
