#include "entities/CEntityNPC.h"
#include <cstring>
#include <iostream>
#include <math.h>

CEntityNPC::CEntityNPC(CEntityManager* entity_manager, std::string name) : CEntityMesh(entity_manager, name) {
    m_generic_behaviour = GENERIC_BEHAVIOUR_0;
    m_unk1A8 = 3;
    m_cone_angle = 1.5707964f; // pi/2
    m_unk104 = cos(m_cone_angle);
    m_unk104 *= m_unk104;
    m_unk108 = CDKW_V3d(0.0f, 0.0f, 0.0f);
    m_unk154 = 0;
    m_unk158 = 6.0f;
    m_unkFC = NULL;
    m_unk100 = "";
    m_unk130 = CDKW_V3d(0.0f, 0.0f, 0.0f);
    m_unk13C = -1.0f;
    m_unk140 = 1.0f;
    m_unk150 = 1.0f;
    m_unk1B4 = (2.0f * rand()) / 32767.0f;
    m_unkF4 = 1;
    m_path_finder_spline = NULL;
    m_unk1CC = -1;
    m_unk1E0 = CDKW_V3d(10000.0f, 10000.0f, 10000.0f);
    m_unk144 = CDKW_V3d::ZERO;
    m_phial = NULL;
    m_batch2CC = NULL;
    m_unk1F8 = 0;
    m_unk2D0 = 0;
    m_unk1A4 = 0.0f;
    m_current_anim = NULL;

    if (m_entity_manager->GetHero()->GetType() == ENTITY_WINNIE) {
        m_unkF4 |= (1 << 17);
    }
}

// Equivalent: stack offsets
void CEntityNPC::Init() {
    CEntityObject::Init();

    m_entity_manager->m_unk1C->AddBVolume(&m_unk1FC, this);

    DkPh::Collider::Body& body = m_entity_manager->m_unk1C->GetBodyRef(&m_unk1FC);
    body.unk2C = 0;
    body.unk28 = 0;
    body.unk24 = 0;
    body.unk30 = 1;

    CDKW_V3d position = GetPosition();
    m_unk1FC.SetCenter(position);

    m_unk1FC.m_unk10.x = 2.0f;
    m_unk1FC.m_unk10.y = 2.0f;
    m_unk1FC.m_unk10.z = 2.0f;

    CDKW_Quat quat;
    RtQuatRotate(&quat, &CDKW_V3d::YAXIS, 45.0f, 0);
    m_unk1FC.SetQuat(quat);
}

BOOL CEntityNPC::Save(void* a1) {
    return CEntity::Save(a1) != FALSE;
}

BOOL CEntityNPC::Restore(void* a1) {
    if (!CEntity::Restore(a1)) {
        return FALSE;
    }

    if (m_flags & (1 << 6) || !(m_flags & (1 << 2))) {
        for (int i = 0; i < (int)m_unk24.size(); i++) {
            unsigned long volume = AS_ULONG_VECTOR_HACK(m_unk24).at(i);
            if (reinterpret_cast<DkPh::BVolume*>(volume) != NULL) {
                m_entity_manager->m_unk1C->GetBodyRef(reinterpret_cast<DkPh::BVolume*>(volume)).unk2C = 0;
            }
        }
    }

    return TRUE;
}

void CEntityNPC::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest1, dest2;

    CEntityMesh::Parse(iter);

    DkPh::Collider::Body& body = m_entity_manager->GetUnkCollider()->GetBodyRef(m_unk90.unk0);
    body.unk34 = ~(1 << 16);

    if (iter.GetFirstChildChunk(dest1) == TRUE) {
        do {
            strcpy(buf, dest1.GetName());

            if (strcmp(buf, "NPCCone") == 0) {
                m_unkF4 |= (1 << 6);

                if (dest1.GetFirstChildChunk(dest2)) {
                    do {
                        strcpy(buf, dest1.GetName());

                        if (strcmp(buf, "ConeAngle") == 0) {
                            m_cone_angle = dest2.GetFloatValue();
                            m_cone_angle *= (3.1415927f / 180.0f); // Convert to radians
                            m_unk104 = cos(m_cone_angle);
                            m_unk104 *= m_unk104;
                        } else if (strcmp(buf, "ConeSize") == 0) {
                            m_cone_distance = dest2.GetFloatValue();
                        } else if (strcmp(buf, "ConeOffset") == 0) {
                            ParseXYZ(dest2, &m_unk108.x, &m_unk108.y, &m_unk108.z);
                        }
                    } while (dest2.GetNextSiblingChunk(dest2));
                }
            }

            if (strcmp(buf, "ConeAngle") == 0) {
                m_unkF4 |= (1 << 6);
                m_cone_angle = 3.1415927f * (dest1.GetFloatValue() / 180.0f);
                m_unk104 = cos(m_cone_angle);
                m_unk104 *= m_unk104;
            } else if (strcmp(buf, "WalkSpeed") == 0) {
                m_unk13C = dest1.GetFloatValue();
            } else if (strcmp(buf, "RunSpeed") == 0) {
                m_unk140 = dest1.GetFloatValue();
            } else if (strcmp(buf, "TurnAnim") == 0) {
                m_unkF4 |= (1 << 4);
            } else if (strcmp(buf, "LookForAnim") == 0) {
                m_unkF4 |= (1 << 5);
            } else if (strcmp(buf, "Spline") == 0) {
                strcpy(buf, dest1.GetStringValue());
                m_unk100 = buf;
                m_unkFC = NULL;
                m_unk1A8 = 1;
                m_unk1A9 = 0;

                if (m_entity_manager->GetGame()->GetIngameGamePart() != NULL) {
                    m_unkFC = m_entity_manager->GetGame()->GetCurrentRoomSplineBank()->GetSplineByName(buf);
                }
            } else if (strcmp(buf, "Life") == 0) {
                m_unkF4 &= ~((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0));

                switch (dest1.GetS32Value()) {
                    case 0:
                        m_unkF4 |= (1 << 3);
                        break;
                    case 1:
                        m_unkF4 |= (1 << 2);
                        break;
                    case 2:
                        m_unkF4 |= (1 << 1);
                        break;
                    case 3:
                        m_unkF4 |= (1 << 0);
                        break;
                }
            } else if (strcmp(buf, "TurnTime") == 0) {
                m_unk150 = dest1.GetFloatValue();
            } else if (strcmp(buf, "AnimationStar") == 0 || strcmp(buf, "AnimationStarFile") == 0) {
                m_unkF4 |= (1 << 4);
            } else if (strcmp(buf, "SoundDetectDist") == 0) {
                m_unk114 = dest1.GetFloatValue();
                m_unk114 *= m_unk114;
            }
        } while (dest1.GetNextSiblingChunk(dest1) == TRUE);
    }

    SetBehavior(m_unk10);

    m_unk118 = m_clump->GetPosition();
    m_unk16C = m_unk118;
    m_unk124 = m_clump->GetAt();
    m_unk178 = m_unk124;

    m_animation_star_controller->Play("WAIT", 1, 1);
}

void CEntityNPC::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    char buf[128];
    DkXmd::CChunkIterator dest;
    int len;

    CEntityMesh::ParseBehavior(iter, behavior);

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "TurnAnim") == 0) {
                m_unkF4 |= (1 << 4);
            } else if (strcmp(buf, "LookForAnim") == 0) {
                m_unkF4 |= (1 << 5);
            } else if (strcmp(buf, "Spline") == 0) {
                strcpy(buf, dest.GetStringValue());

                // Fakematch?
                // m_unk100 = buf;
                len = strlen(buf);
                m_unk100.assign(buf, len);

                m_unkFC = NULL;
                m_unk1A8 = 1;
                m_unk1A9 = 0;

                if (m_entity_manager->GetGame()->GetIngameGamePart() != NULL) {
                    m_unkFC = m_entity_manager->GetGame()->GetCurrentRoomSplineBank()->GetSplineByName(buf);
                }
            } else if (strcmp(buf, "TurnTime") == 0) {
                m_unk150 = dest.GetFloatValue();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }

    m_unk124 = m_clump->GetAt();
}

void CEntityNPC::SetGenericBehaviour(ENPCGenericBehaviour generic_behaviour) {
    m_generic_behaviour = generic_behaviour;
    switch (m_generic_behaviour) {
        case GENERIC_BEHAVIOUR_0:
            m_unk19C = 0.0f;
            m_unkF4 &= ~(1 << 15);
            break;
        case GENERIC_BEHAVIOUR_2:
            m_unk19C = 0.0f;
            m_unk190 = GetPosition();
            m_unk1A8 = 0;
            break;
        case GENERIC_BEHAVIOUR_1: {
            m_unk90.unk8 = CDKW_V3d::ZERO;
            m_unk144 = CDKW_V3d::ZERO;
            CDKW_V3d at = m_clump->GetAt();
            CDKW_V3d delta = GetPosDelta();
            F32 unk_x = delta.x * at.x + delta.z * at.z;
            F32 unk_y = delta.x * at.z - delta.z * at.x;
            if ((F32)fabs(unk_y) < 0.001f) {
                m_rotation_angle = 0.0f;
            }
            m_rotation_angle = atan2(unk_y, unk_x);
            m_rotation_angle = (180.0f * m_rotation_angle) / 3.1415927f; // Convert to degrees
            if (m_rotation_angle < 0.0f) {
                if (m_rotation_angle < -180.0f) {
                    m_rotation_angle = 360.0f + m_rotation_angle;
                }
            } else if (m_rotation_angle > 180.0f) {
                m_rotation_angle = -360.0f + m_rotation_angle;
            }

            m_unk1A8 = 0;
            break;
        }
        case GENERIC_BEHAVIOUR_4:
            m_unk1A8 = 101;
            break;
        case GENERIC_BEHAVIOUR_5:
            m_unk1A8 = 101;
            break;
        case GENERIC_BEHAVIOUR_6:
            m_unk1A8 = 0;
            break;
        case GENERIC_BEHAVIOUR_7:
            m_unk1A8 = 0;
            break;
        case GENERIC_BEHAVIOUR_8:
            m_unk1A8 = 101;
            break;
    }
}

ENPCGenericBehaviour CEntityNPC::GetGenericBehaviour() {
    return m_generic_behaviour;
}

void CEntityNPC::UpdateDetectionBehaviour(F32 dt) {
    SetGenericBehaviour(GENERIC_BEHAVIOUR_2);
}

void CEntityNPC::UpdateFightBehaviour(F32 dt) {

}

void CEntityNPC::UpdateGrimaceBehaviour(F32 dt) {

}

void CEntityNPC::UpdateReturnBaseBehaviour(F32 dt) {
    if (m_unkF4 & (1 << 17)) {
        SetGenericBehaviour(GENERIC_BEHAVIOUR_2);
    } else {
        SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
        m_unk1A8 = m_unk1AC;
        m_unk1A9 = m_unk1AD;
        if (m_unk1A8 == 3) {
            m_animation_star_controller->Play("WAIT");
        }
    }
}

void CEntityNPC::UpdateSpecialBehaviour(F32 dt) {

}

F32 CEntityNPC::GetCurrentVelocity() {
    if (m_unkF4 & (1 << 13)) {
        return m_unk140;
    } else {
        return m_unk13C;
    }
}

void CEntityNPC::PlayWalkAnim(BOOL a1) {
    if (a1) {
        if (m_unkF4 & (1 << 0)) {
            m_animation_star_controller->Play("WALK_NORMAL", 1, 1);
        } else if (m_unkF4 & (1 << 1)) {
            m_animation_star_controller->Play("WALK_WORRIED", 1, 1);
        } else if (m_unkF4 & (1 << 2)) {
            m_animation_star_controller->Play("WALK_FRIGHTENED", 1, 1);
        }
    } else {
        if (m_unkF4 & (1 << 0)) {
            m_animation_star_controller->Play("WALK_NORMAL");
        } else if (m_unkF4 & (1 << 1)) {
            m_animation_star_controller->Play("WALK_WORRIED");
        } else if (m_unkF4 & (1 << 2)) {
            m_animation_star_controller->Play("WALK_FRIGHTENED");
        }
    }
}

void CEntityNPC::PlayRunAnim(BOOL a1) {
    if (a1) {
        if (m_unkF4 & (1 << 0)) {
            m_animation_star_controller->Play("RUN_NORMAL", 1, 1);
        } else if (m_unkF4 & (1 << 1)) {
            m_animation_star_controller->Play("RUN_WORRIED", 1, 1);
        } else if (m_unkF4 & (1 << 2)) {
            m_animation_star_controller->Play("RUN_FRIGHTENED", 1, 1);
        }
    } else {
        if (m_unkF4 & (1 << 0)) {
            m_animation_star_controller->Play("RUN_NORMAL");
        } else if (m_unkF4 & (1 << 1)) {
            m_animation_star_controller->Play("RUN_WORRIED");
        } else if (m_unkF4 & (1 << 2)) {
            m_animation_star_controller->Play("RUN_FRIGHTENED");
        }
    }
}

void CEntityNPC::MakeNPCLaughing() {
    m_unk1AA = 3;
    m_unk1AB = 4;
    SetGenericBehaviour(GENERIC_BEHAVIOUR_2);
    m_unk1A8 = 5;
    m_unkF4 |= (1 << 15);
    m_unk19C = 3.0f;
}

void CEntityNPC::MakeOtherNPCLaughing() {
    if (m_unk1A8 != 6 && m_unk1A8 != 5 && m_unk1A8 != 10) {
        m_unk1AA = m_unk1A8;
        m_unk1AB = m_unk1A9;
    }
    SetGenericBehaviour(GENERIC_BEHAVIOUR_2);
    m_unk1A8 = 5;
}

void CEntityNPC::MakeNPCSatisfaction() {
    m_animation_star_controller->Play("FRIGHTEN_PIGLET");
    SetGenericBehaviour(GENERIC_BEHAVIOUR_2);
    m_unk1A8 = 6;
}

void CEntityNPC::SaveBaseBehaviour() {
    m_unk16C = GetPosition();
    m_unk178 = GetOrientation();
    m_unk1AC = m_unk1A8;
    m_unk1AD = m_unk1A9;
}

CPigSpline* CEntityNPC::GetPathFinderSpline() {
    return m_path_finder_spline;
}

void CEntityNPC::MakeFritten() {
    if (m_unkF4 & (1 << 0)) {
        m_unkF4 &= ~(1 << 0);
        m_unkF4 |= (1 << 1);
    } else if (m_unkF4 & (1 << 1)) {
        m_unkF4 &= ~(1 << 1);
        m_unkF4 |= (1 << 2);
    } else if (m_unkF4 & (1 << 2)) {
        m_unkF4 &= ~(1 << 2);
        m_unkF4 |= (1 << 3);
    }
}

void CEntityNPC::SetDeadInSecondPass() {
    DkPh::Collider* collider = m_entity_manager->m_unk1C;
    for (std::vector<DkPh::BVolume*>::iterator iter = m_unk24.begin(); iter < m_unk24.end(); iter++) {
        collider->GetBodyRef(*iter).unk2C = 0;
    }

    DelFlag(ENTITY_FLAG_ACTIVE);
    DelFlag(ENTITY_FLAG_VISIBLE);
    DelFlag(ENTITY_FLAG_HAS_COLLISION);
    AddFlag(ENTITY_FLAG_UNK6);
}

void CEntityNPC::SaveCurrentAnim() {
    m_current_anim = m_animation_star_controller->GetPlayingAnimationName();
}

void CEntityNPC::RestoreSavedAnim() {
    if (m_current_anim != NULL) {
        m_animation_star_controller->Play(*m_current_anim);
    }
}

void CEntityNPC::Idle() {
    if (m_unkF4 & (1 << 10)) {
        return;
    }

    SaveCurrentAnim();
    if (m_current_anim != NULL && strcmp(m_current_anim->c_str(), "SLEEP") != 0) {
        m_animation_star_controller->Play("WAIT");
    }
    m_unkF4 |= (1 << 10);
}

void CEntityNPC::UnIdle(BOOL play_saved_anim) {
    if (!(m_unkF4 & (1 << 10))) {
        return;
    }

    if (m_current_anim != NULL && play_saved_anim) {
        m_animation_star_controller->Play(*m_current_anim);
    }

    m_unkF4 &= ~(1 << 10);
    m_unk90.unk8 = CDKW_V3d::ZERO;
    m_unk144 = CDKW_V3d::ZERO;
}
