#include "engine/display/CController.h"
#include "display/IGenericAnimation.h"
#include "display/IHierarchy.h"

namespace DKDSP {

CController::~CController() {
    Release();
}

void CController::Release() {
    m_time = 0.0f;
    m_speed = 1.0f;
    m_unk1C = 0;

    m_hanim_controller.Release();
    m_dmorph_controller.Release();
    m_material_controller.Release();
    m_event_controller.Release();

    if (m_hierarchy != NULL) {
        m_hierarchy->Release();
        delete m_hierarchy;
    }

    m_hierarchy = NULL;
    m_clump = NULL;
    m_hierarchy_is_safe = FALSE;
    m_user_data = NULL;
}

void CController::SetEventUserData(void* data) {
    m_user_data = data;
    m_event_controller.SetUserData(m_user_data);
}

void CController::Create(IClump* clump, IHierarchy* hierarchy, int a3) {
    if (a3 <= 0) {
        a3 = 1;
    }

    if (hierarchy == NULL) {
        return;
    }

    m_clump = (DKDSP::CClump*)clump;
    m_hierarchy = (DKDSP::CHierarchy*)hierarchy->Clone(1 << 1);
    m_hierarchy->func68();
    m_unk1C = a3;

    m_hanim_controller.Create(m_hierarchy, m_unk1C);
    m_dmorph_controller.Create(m_hierarchy, 0);
    m_material_controller.Create(m_hierarchy, clump->GetNumberOfAtomics());
    m_event_controller.Create(m_hierarchy, m_unk1C);
}

void CController::Play(F32 a1) {
    m_time = a1;
    m_unkC = a1;
    m_playing = TRUE;
}

void CController::Stop() {
    m_playing = FALSE;
}

BOOL CController::IsPlaying() {
    return m_playing;
}

void CController::Pause() {
    m_paused = TRUE;
}

void CController::Resume() {
    m_paused = FALSE;
}

BOOL CController::IsPaused() {
    return m_paused;
}

F32 CController::GetTime() {
    return m_time;
}

void CController::SetTime(F32 time) {
    m_time = time;
}

F32 CController::GetSpeed() {
    return m_speed;
}

void CController::SetSpeed(F32 speed) {
    m_speed = speed;
}

int CController::GetNumberOfNodes() {
    return m_hierarchy->GetNumberOfNodes();
}

std::string& CController::GetName() {
    return m_name;
}

ISubController* CController::GetSubController(EANIMATIONTYPE type) {
    switch (type) {
        case ANIMATION_TYPE_ANM:
            return &m_hanim_controller;
        case ANIMATION_TYPE_DMA:
            return &m_dmorph_controller;
        case ANIMATION_TYPE_TAN:
            return &m_material_controller;
        case ANIMATION_TYPE_EVN:
            return &m_event_controller;
        default:
            return NULL;
    }
}

void CController::SetName(std::string name) {
    m_name.assign(name, 0);
}

void CController::Update(F32 dt) {
    CHierarchy* uVar2;
    int i;
    F32 dVar9;

    F32 dt_scaled = 0.0f;
    if (m_playing == TRUE && m_paused == FALSE) {
        dt_scaled = dt * m_speed;
    }

    m_time += dt_scaled;
    m_event_controller.Update(dt_scaled);
    m_material_controller.Update(dt_scaled);
    m_dmorph_controller.Update(dt_scaled);
    m_hanim_controller.Update(dt_scaled);

    int nb_animations = m_hanim_controller.GetNumberOfAnimations();
    if (nb_animations <= 0) {
        return;
    }

    m_unk28 = m_hanim_controller.ConvertIndex(0);

    CHierarchy* hierarchy = m_hanim_controller.GetHierarchyRealIndex(m_unk28);
    if (hierarchy == NULL) {
        m_hierarchy_is_safe = FALSE;
        return;
    }

    m_hierarchy_is_safe = TRUE;
    m_unk24 = 1;

    if (nb_animations <= 1) {
        return;
    }

    m_unk24 = 0;
    hierarchy->Copy(m_hierarchy);

    F32* start_accmulation_entry = m_hanim_controller.GetWeightAccumulationEntry(m_hanim_controller.GetStartNode(0));
    *start_accmulation_entry = m_hanim_controller.GetAnimationWeight(0);

    dt_scaled = 0.0f;

    for (i = 1; i < nb_animations; i++) {
        int iVar6 = m_hanim_controller.ConvertIndex(i);
        CControllerAnimEntry* anim_entry = &m_hanim_controller.m_anim_entries[iVar6];
        dVar9 = anim_entry->m_unk28;
        uVar2 = anim_entry->m_unk54;
        F32* unk48_accumulation_entry = m_hanim_controller.GetWeightAccumulationEntry(anim_entry->m_unk48);
        if (dVar9 > dt_scaled) {
            *unk48_accumulation_entry += dVar9;
            m_hierarchy->BlendSubHierarchyFast(m_hierarchy, uVar2, dVar9 / *unk48_accumulation_entry);
        }
    }
}

int CController::IsHierarchySafe() {
    return m_hierarchy_is_safe;
}

int CController::GetSize() {
    int size = 0;

    if (m_hierarchy != NULL) {
        size += m_hierarchy->GetSize();
    }

    size += m_hanim_controller.GetSize();
    size += m_dmorph_controller.GetSize();
    size += m_material_controller.GetSize();
    size += m_event_controller.GetSize();

    size += sizeof(CController);

    return size;
}

void CController::UpdateClump(IClump* clump) {
    if (clump == NULL) {
        return;
    }

    CHierarchy* clump_hierarchy = clump->GetHierarchy();
    if (clump_hierarchy != NULL) {
        if (m_unk24 == 1) {
            m_hanim_controller.GetHierarchyRealIndex(m_unk28)->Copy(clump_hierarchy);
        } else {
            m_hierarchy->Copy(clump_hierarchy);
        }
        clump_hierarchy->Update();
    }

    m_dmorph_controller.UpdateClump((DKDSP::CClump*)clump);
    m_material_controller.UpdateClump((DKDSP::CClump*)clump);
}

int CController::GetNumberOfAnimations(EANIMATIONTYPE type) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL) {
        return subcontroller->GetNumberOfAnimations();
    } else {
        return 0;
    }
}

CAnimation* CController::GetAnimation(EANIMATIONTYPE type, int id) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL) {
        return subcontroller->GetAnimation(id);
    } else {
        return 0;
    }
}

int CController::PlayAnimation(IGenericAnimation* animation, int a2, F32 a3, F32 a4, F32 a5, F32 a6) {
    if (animation == NULL) {
        return -1;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (animation->GetType() == ANIMATION_TYPE_EVN) {
        a2 = -1;
    }

    int index = 0;
    if (a2 >= 0) {
        index = m_hierarchy->GetIndexFromID(a2);
        if (index < 0) {
            index = 0;
        }
    }

    if (subcontroller != NULL) {
        if (animation->GetType() == ANIMATION_TYPE_TAN) {
            index = m_clump->GetAtomicIndexFromNodeIndex(index);
        }

        return subcontroller->AddAnimation(animation, index, a3, a4, a5, a6);
    } else {
        return -1;
    }
}

void CController::StopAnimation(IGenericAnimation* animation, int a2) {
    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            subcontroller->StopAnimation(index, a2);
        }
    }
}

void CController::StopAllAnimations(EANIMATIONTYPE type, int a2) {
    ISubController* subcontroller = NULL;
    if (type != ANIMATION_TYPE_4) {
        subcontroller = GetSubController(type);
    }

    if (subcontroller != NULL) {
        subcontroller->StopAllAnimations(a2);
    } else if (type == ANIMATION_TYPE_4) {
        GetSubController(ANIMATION_TYPE_ANM)->StopAllAnimations(a2);
        GetSubController(ANIMATION_TYPE_DMA)->StopAllAnimations(a2);
        GetSubController(ANIMATION_TYPE_TAN)->StopAllAnimations(a2);
        GetSubController(ANIMATION_TYPE_EVN)->StopAllAnimations(a2);
    }
}

BOOL CController::IsAnimationPlaying(IGenericAnimation* animation) {
    if (animation == NULL) {
        return FALSE;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int location = subcontroller->FindAnimation(animation);
        if (location >= 0) {
            return subcontroller->IsAnimationPlaying(location);
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

void CController::PauseAnimation(IGenericAnimation* animation) {
    if (animation == NULL) {
        return;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            subcontroller->PauseAnimation(index);
        }
    }
}

void CController::ResumeAnimation(IGenericAnimation* animation) {
    if (animation == NULL) {
        return;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            subcontroller->ResumeAnimation(index);
        }
    }
}

F32 CController::GetAnimationTime(IGenericAnimation* animation) {
    if (animation == NULL) {
        return 0.0f;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            return subcontroller->GetAnimationTime(index);
        } else {
            return 0.0f;
        }
    } else {
        return 0.0f;
    }
}

F32 CController::GetAnimationAbsoluteTime(IGenericAnimation* animation) {
    if (animation == NULL) {
        return 0.0f;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            return subcontroller->GetAnimationAbsoluteTime(index);
        } else {
            return 0.0f;
        }
    } else {
        return 0.0f;
    }
}

F32 CController::GetAnimationTimeBeforePlay(IGenericAnimation* animation) {
    if (animation == NULL) {
        return 0.0f;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            return subcontroller->GetAnimationTimeBeforePlay(index);
        } else {
            return 0.0f;
        }
    } else {
        return 0.0f;
    }
}

void CController::SetAnimationTime(IGenericAnimation* animation, F32 time) {
    if (animation == NULL) {
        return;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            subcontroller->SetAnimationTime(index, time);
        }
    }
}

void CController::SetAnimationSpeed(IGenericAnimation* animation, F32 speed) {
    if (animation == NULL) {
        return;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            subcontroller->SetAnimationSpeed(index, speed);
        }
    }
}

F32 CController::GetAnimationSpeed(IGenericAnimation* animation) {
    if (animation == NULL) {
        return 0.0f;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            return subcontroller->GetAnimationSpeed(index);
        } else {
            return 0.0f;
        }
    } else {
        return 0.0f;
    }
}

F32 CController::GetAnimationDuration(IGenericAnimation* animation) {
    if (animation == NULL) {
        return 0.0f;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            return subcontroller->GetAnimationDuration(index);
        } else {
            return 0.0f;
        }
    } else {
        return 0.0f;
    }
}

void CController::SetAnimationLoopMode(IGenericAnimation* animation, EANIMATIONLOOP loop, int a3) {
    if (animation == NULL) {
        return;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            subcontroller->SetAnimationLoopMode(index, loop, a3);
        }
    }
}

EANIMATIONLOOP CController::GetAnimationLoopMode(IGenericAnimation* animation) {
    if (animation == NULL) {
        return ANIMATION_LOOP_1;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());

    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            return subcontroller->GetAnimationLoopMode(index);
        } else {
            return ANIMATION_LOOP_1;
        }
    } else {
        return ANIMATION_LOOP_1;
    }
}

F32 CController::GetAnimationWeight(IGenericAnimation* animation) {
    if (animation == NULL) {
        return 0.0f;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            return subcontroller->GetAnimationWeight(index);
        } else {
            return 0.0f;
        }
    } else {
        return 0.0f;
    }
}

F32 CController::GetAnimationNormalizedWeight(IGenericAnimation* animation) {
    if (animation == NULL) {
        return 0.0f;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            return subcontroller->GetAnimationNormalizedWeight(index);
        } else {
            return 0.0f;
        }
    } else {
        return 0.0f;
    }
}

void CController::SetAnimationWeight(IGenericAnimation* animation, F32 weight) {
    if (animation == NULL) {
        return;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            subcontroller->SetAnimationWeight(index, weight);
        }
    }
}

void CController::SetAnimationTargetWeight(IGenericAnimation* animation, F32 target_weight) {
    if (animation == NULL) {
        return;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            subcontroller->SetAnimationTargetWeight(index, target_weight);
        }
    }
}

int CController::GetAnimationStatus(IGenericAnimation* animation) {
    if (animation == NULL) {
        return 0;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            return subcontroller->GetAnimationStatus(index);
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

void CController::SetAnimationDelayOut(IGenericAnimation* animation, F32 delay_out) {
    if (animation == NULL) {
        return;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            subcontroller->SetAnimationDelayOut(index, delay_out);
        }
    }
}

void CController::SetAnimationDelayIn(IGenericAnimation* animation, F32 delay_in) {
    if (animation == NULL) {
        return;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            subcontroller->SetAnimationDelayIn(index, delay_in);
        }
    }
}

BOOL CController::IsAnimationLooped(IGenericAnimation* animation) {
    if (animation == NULL) {
        return FALSE;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int location = subcontroller->FindAnimation(animation);
        if (location >= 0) {
            return subcontroller->IsAnimationLooped(location);
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

F32 CController::GetAnimationLoopDelta(IGenericAnimation* animation) {
    if (animation == NULL) {
        return 0.0f;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            return subcontroller->GetAnimationLoopDelta(index);
        } else {
            return 0.0f;
        }
    } else {
        return 0.0f;
    }
}

int CController::FindAnimation(IGenericAnimation* animation) {
    if (animation == NULL) {
        return -1;
    }

    ISubController* subcontroller = GetSubController(animation->GetType());
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimation(animation);
        if (index >= 0) {
            return index;
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

int CController::FindAnimationFromUID(EANIMATIONTYPE type, int uid) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL) {
        int index = subcontroller->FindAnimationFromUID(uid);
        if (index >= 0) {
            return index;
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

void CController::StopAnimation(EANIMATIONTYPE type, int id, int a3) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL && id >= 0) {
        subcontroller->StopAnimation(id, a3);
    }
}

BOOL CController::IsAnimationPlaying(EANIMATIONTYPE type, int id) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL) {
        if (id >= 0) {
            return subcontroller->IsAnimationPlaying(id);
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

void CController::PauseAnimation(EANIMATIONTYPE type, int id) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL && id >= 0) {
        subcontroller->PauseAnimation(id);
    }
}

void CController::ResumeAnimation(EANIMATIONTYPE type, int id) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL && id >= 0) {
        subcontroller->ResumeAnimation(id);
    }
}

F32 CController::GetAnimationTime(EANIMATIONTYPE type, int id) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL) {
        if (id >= 0) {
            return subcontroller->GetAnimationTime(id);
        } else {
            return 0.0f;
        }
    } else {
        return 0.0f;
    }
}

F32 CController::GetAnimationAbsoluteTime(EANIMATIONTYPE type, int id) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL) {
        if (id >= 0) {
            return subcontroller->GetAnimationAbsoluteTime(id);
        } else {
            return 0.0f;
        }
    } else {
        return 0.0f;
    }
}

void CController::SetAnimationTime(EANIMATIONTYPE type, int id, F32 time) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL && id >= 0) {
        subcontroller->SetAnimationTime(id, time);
    }
}

void CController::SetAnimationSpeed(EANIMATIONTYPE type, int id, F32 speed) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL && id >= 0) {
        subcontroller->SetAnimationSpeed(id, speed);
    }
}

F32 CController::GetAnimationSpeed(EANIMATIONTYPE type, int id) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL) {
        if (id >= 0) {
            return subcontroller->GetAnimationSpeed(id);
        } else {
            return 0.0f;
        }
    } else {
        return 0.0f;
    }
}

F32 CController::GetAnimationDuration(EANIMATIONTYPE type, int id) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL) {
        if (id >= 0) {
            return subcontroller->GetAnimationDuration(id);
        } else {
            return 0.0f;
        }
    } else {
        return 0.0f;
    }
}

void CController::SetAnimationLoopMode(EANIMATIONTYPE type, int id, EANIMATIONLOOP loop, int a4) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL && id >= 0) {
        subcontroller->SetAnimationLoopMode(id, loop, a4);
    }
}

EANIMATIONLOOP CController::GetAnimationLoopMode(EANIMATIONTYPE type, int id) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL) {
        if (id >= 0) {
            return subcontroller->GetAnimationLoopMode(id);
        } else {
            return ANIMATION_LOOP_1;
        }
    } else {
        return ANIMATION_LOOP_1;
    }
}

void CController::SetAnimationWeight(EANIMATIONTYPE type, int id, F32 weight) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL && id >= 0) {
        subcontroller->SetAnimationWeight(id, weight);
    }
}

void CController::SetAnimationTargetWeight(EANIMATIONTYPE type, int id, F32 target_weight) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL && id >= 0) {
        subcontroller->SetAnimationTargetWeight(id, target_weight);
    }
}

int CController::GetAnimationStatus(EANIMATIONTYPE type, int id) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL) {
        if (id >= 0) {
            return subcontroller->GetAnimationStatus(id);
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

void CController::SetAnimationDelayOut(EANIMATIONTYPE type, int id, F32 delay_out) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL && id >= 0) {
        subcontroller->SetAnimationDelayOut(id, delay_out);
    }
}

void CController::SetAnimationDelayIn(EANIMATIONTYPE type, int id, F32 delay_in) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL && id >= 0) {
        subcontroller->SetAnimationDelayIn(id, delay_in);
    }
}

BOOL CController::IsAnimationLooped(EANIMATIONTYPE type, int id) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL) {
        if (id >= 0) {
            return subcontroller->IsAnimationLooped(id);
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

F32 CController::GetAnimationLoopDelta(EANIMATIONTYPE type, int id) {
    ISubController* subcontroller = GetSubController(type);
    if (subcontroller != NULL) {
        if (id >= 0) {
            return subcontroller->GetAnimationLoopDelta(id);
        } else {
            return 0.0f;
        }
    } else {
        return 0.0f;
    }
}

}
