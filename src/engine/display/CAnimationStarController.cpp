#include "engine/display/CAnimationStarController.h"
#include "display/CAnimationStarEntry.h"
#include "engine/display/CEventAnimation.h"
#include <cstdlib>

namespace DKDSP {

CAnimationStarController::CAnimationStarController() : m_name("") {
    m_animation_star = NULL;
    m_controller = NULL;
    m_playing_animation = NULL;
    m_target_animation = NULL;
    m_search_depth = 5;
    m_unk1C = m_search_depth;
    m_unk20 = 1.0f;
    m_unk24 = 0;
    m_unk28 = 0;
    m_unk2C = NULL;
    m_unk30 = 0;
    m_next_time = 0.0f;
    m_playing_animation_speed = 1.0f;
}

CAnimationStarController::~CAnimationStarController() {
    Release();
}

void CAnimationStarController::Release() {
    m_animation_star = NULL;
    m_controller = NULL;
    m_playing_animation = NULL;
    m_target_animation = NULL;
    m_search_depth = 10;
    m_unk1C = m_search_depth;
    m_unk20 = 1.0f;
    m_unk24 = 0;
    m_unk28 = 0;
    m_unk30 = 0;

    if (m_unk2C != NULL) {
        free(m_unk2C);
    }
    m_unk2C = NULL;

    m_next_time = 0.0f;
    m_playing_animation_speed = 1.0f;
}

U32 CAnimationStarController::StartAnimation(CAnimationStarEntry* entry, SAnimationStarValues* star_values, STargetAnimationValues* target_values) {
    F32 fVar1 = star_values->unk10;
    F32 fVar2 = star_values->unk14;
    if (target_values != NULL) {
        fVar1 = target_values->unk0;
        fVar2 = target_values->unk4;
    }

    F32 dVar10 = fVar1;
    F32 dVar9 = fVar2;

    if (m_playing_animation != NULL) {
        for (int i = 0; i < m_playing_animation->GetNumberOfAnimations(); i++) {
            CAnimation* anim = m_playing_animation->GetAnimation(i);
            EANIMATIONTYPE type = anim->GetType();
            int uid = GetUnk2CElem(i);
            if (uid >= 0) {
                int anim_id = m_controller->FindAnimationFromUID(type, uid);
                if (anim_id >= 0) {
                    m_controller->SetAnimationDelayOut(type, anim_id, dVar9 * m_unk20);
                }
            }
        }
    }

    if (entry == NULL) {
        m_controller->PlayAnimation(m_animation_star->GetDefaultAnimation(), -1, 0.0f, star_values->unkC, dVar10 * m_unk20, star_values->unk14);
    } else {
        EANIMATIONLOOP loop_mode = star_values->unk4;

        dVar9 = entry->GetAnimationDuration(ANIMATION_TYPE_ANM);
        if (dVar9 < 0.0f) {
            dVar9 = entry->GetAnimationDuration(ANIMATION_TYPE_DMA);
        }

        m_playing_animation_speed = 1.1754944e-38f;
        for (int i = 0; i < entry->GetNumberOfAnimations(); i++) {
            CEventAnimation* anim = (CEventAnimation*)entry->GetAnimation(i);
            if (anim->GetType() == ANIMATION_TYPE_EVN && dVar9 > 0.0f && entry->GetAnimationSyncFlag(i) == 0) {
                anim->SetDuration(dVar9);
            }
            int node = entry->GetAnimationNode(i);
            int play_ret = m_controller->PlayAnimation(anim, node, 0.0f, star_values->unkC, dVar10 * m_unk20, star_values->unk14);
            if (play_ret >= 0) {
                int uid = m_controller->FindAnimationFromUID(anim->GetType(), play_ret);
                if (uid >= 0) {
                    m_controller->SetAnimationSpeed(anim->GetType(), uid, star_values->speed);
                    m_controller->SetAnimationTargetWeight(anim->GetType(), uid, star_values->target_weight);
                    m_controller->SetAnimationLoopMode(anim->GetType(), uid, loop_mode, 0);
                    m_controller->SetAnimationTime(anim->GetType(), uid, m_next_time);

                    m_playing_animation = entry;
                    if (i >= 0 && i < m_unk30) {
                        m_unk2C[i] = play_ret;
                    }
                }
            }
        }
    }

    m_next_time = 0.0f;

    return 0;
}

void CAnimationStarController::SetController(IController* controller) {
    m_controller = (CController*)controller;
    if (m_animation_star != NULL) {
        m_controller->Play();
        m_controller->StopAllAnimations(ANIMATION_TYPE_4, 1);
        StartAnimation(NULL, m_animation_star->GetDefaultAnimationValues(), NULL);
    }
}

CController* CAnimationStarController::GetController() {
    return m_controller;
}

void CAnimationStarController::SetAnimationStar(IAnimationStar* animation_star) {
    m_animation_star = (CAnimationStar*)animation_star;

    int max_nb_anims = 0;
    for (int i = 0; i < m_animation_star->GetNumberOfAnimations(); i++) {
        CAnimationStarEntry* entry = m_animation_star->GetAnimationEntry(i);
        if (entry != NULL && max_nb_anims < entry->GetNumberOfAnimations()) {
            max_nb_anims = entry->GetNumberOfAnimations();
        }
    }

    m_unk30 = max_nb_anims * 2;
    if (m_unk2C != NULL) {
        free(m_unk2C);
    }
    m_unk2C = NULL;
    m_unk2C = (int*)malloc(m_unk30 * 4);

    if (m_controller != NULL) {
        m_controller->Play();
        m_controller->StopAllAnimations(ANIMATION_TYPE_4, 1);
        StartAnimation(NULL, m_animation_star->GetDefaultAnimationValues(), NULL);
    }
}

CAnimationStar* CAnimationStarController::GetAnimationStar() {
    return m_animation_star;
}

void CAnimationStarController::SetSearchDepth(int depth) {
    if (depth > 0) {
        m_search_depth = depth;
    }
}

void CAnimationStarController::Pause() {
    if (m_controller != NULL) {
        m_controller->Pause();
    }
}

void CAnimationStarController::Resume() {
    if (m_controller != NULL) {
        m_controller->Resume();
    }
}

void CAnimationStarController::Reset() {
    if (m_controller != NULL) {
        m_controller->StopAllAnimations(ANIMATION_TYPE_4, 1);
        m_playing_animation = NULL;
        StartAnimation(NULL, m_animation_star->GetDefaultAnimationValues(), NULL);
    }
}

BOOL CAnimationStarController::Play(std::string a1) {
    return Play((char*)a1.c_str(), 0.0f, 0, 0);
}

BOOL CAnimationStarController::Play(std::string a1, F32 a2) {
    return Play((char*)a1.c_str(), a2, 0, 0);
}

BOOL CAnimationStarController::Play(std::string a1, int a2) {
    return Play((char*)a1.c_str(), 0.0f, a2, 0);
}

BOOL CAnimationStarController::Play(std::string a1, F32 a2, int a3) {
    return Play((char*)a1.c_str(), a2, a3, 0);
}

BOOL CAnimationStarController::Play(std::string a1, int a2, int a3) {
    return Play((char*)a1.c_str(), 0.0f, a2, a3);
}

BOOL CAnimationStarController::Play(std::string a1, F32 a2, int a3, int a4) {
    return Play((char*)a1.c_str(), a2, a3, a4);
}

BOOL CAnimationStarController::Play(char* a1) {
    return Play(a1, 0.0f, 0, 0);
}

BOOL CAnimationStarController::Play(char* a1, F32 a2) {
    return Play(a1, a2, 0, 0);
}

BOOL CAnimationStarController::Play(char* a1, int a2) {
    return Play(a1, 0.0f, a2, 0);
}

BOOL CAnimationStarController::Play(char* a1, F32 a2, int a3) {
    return Play(a1, a2, a3, 0);
}

BOOL CAnimationStarController::Play(char* a1, int a2, int a3) {
    return Play(a1, 0.0f, a2, a3);
}

BOOL CAnimationStarController::Play(char* a1, F32 a2, int a3, int a4) {
    if (m_controller == NULL || m_animation_star == NULL) {
        return FALSE;
    }

    if (a1 == NULL) {
        return FALSE;
    }

    if (a3 == 0) {
        a4 = 0;
    }
    m_unk24 = a4;

    CAnimationStarEntry* entry = m_animation_star->GetAnimationEntry(a1);
    CAnimation* anim = NULL;
    if (entry != NULL) {
        if (entry->GetNumberOfAnimations() <= 0) {
            return FALSE;
        }
        anim = entry->GetAnimation(0);
    }

    if (anim == NULL) {
        return FALSE;
    }

    if (entry == m_target_animation) {
        return FALSE;
    }

    m_target_animation = entry;
    m_unk20 = 1.0f;

    SAnimationPath path;
    path.unk0 = 0.0f;
    path.unk4 = 0;
    path.unk8 = 0;
    path.unkC = 0;
    path.unk10 = 0;

    if (m_playing_animation != NULL) {
        if (a3 == 1) {
            if (a2 > 0.0f && a2 < m_target_animation->GetAnimationValues()->unk10 && m_target_animation->GetAnimationValues()->unk10 > 0.0f) {
                m_unk20 = a2 / m_target_animation->GetAnimationValues()->unk10;
            }
            m_unk28 = TRUE;
        } else {
            if (m_animation_star->FindNextAnimationInPath(m_playing_animation, m_target_animation, m_search_depth, &path) == TRUE) {
                m_unk20 = 1.0f;
                if (a2 > 0.0f && a2 < path.unk0 && path.unk0 > 0.0f) {
                    m_unk20 = a2 / path.unk0;
                }
            }
            m_unk28 = FALSE;
        }
    }

    if (m_unk20 > 1.0f) {
        m_unk20 = 1.0f;
    }

    m_unk1C = m_search_depth - 1;

    return TRUE;
}

BOOL CAnimationStarController::IsPlayingTargetAnimation() {
    return m_playing_animation == m_target_animation;
}

BOOL CAnimationStarController::IsFullyPlayingTargetAnimation() {
    if (m_playing_animation == m_target_animation) {
        if (m_playing_animation == NULL) {
            return TRUE;
        }

        CAnimation* animation = m_playing_animation->GetAnimation(0);
        EANIMATIONTYPE type = animation->GetType();

        int uid = GetUnk2CElem(0);
        if (uid < 0) {
            return FALSE;
        }

        int anim_id = m_controller->FindAnimationFromUID(type, uid);
        if (anim_id < 0) {
            return FALSE;
        }

        if (m_controller->GetAnimationStatus(type, anim_id) == 3) {
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CAnimationStarController::IsFullyPlayingCurrentAnimation() {
    if (m_playing_animation == NULL) {
        return TRUE;
    }

    CAnimation* animation = m_playing_animation->GetAnimation(0);
    EANIMATIONTYPE type = animation->GetType();

    int uid = GetUnk2CElem(0);
    if (uid < 0) {
        return FALSE;
    }

    int anim_id = m_controller->FindAnimationFromUID(type, uid);
    if (anim_id < 0) {
        return FALSE;
    }

    if (m_controller->GetAnimationStatus(type, anim_id) == 3) {
        return TRUE;
    }

    return FALSE;
}

BOOL CAnimationStarController::IsPlayingDefaultAnimation() {
    return m_playing_animation == NULL;
}

BOOL CAnimationStarController::IsTargetDefaultAnimation() {
    return m_target_animation == NULL;
}

BOOL CAnimationStarController::IsPlayingAnimation(std::string a1) {
    return IsPlayingAnimation(a1.c_str());
}

BOOL CAnimationStarController::IsPlayingAnimation(char* a1) {
    CAnimationStarEntry* entry = m_animation_star->GetAnimationEntry(a1);
    if (entry == NULL) {
        return FALSE;
    }

    return m_playing_animation == entry;
}

BOOL CAnimationStarController::IsTargetAnimation(std::string a1) {
    return IsTargetAnimation(a1.c_str());
}

BOOL CAnimationStarController::IsTargetAnimation(char* a1) {
    CAnimationStarEntry* entry = m_animation_star->GetAnimationEntry(a1);
    if (entry == NULL) {
        return FALSE;
    }

    return m_target_animation == entry;
}

std::string* CAnimationStarController::GetPlayingAnimationName() {
    if (m_playing_animation != NULL) {
        return m_playing_animation->GetName();
    }

    return NULL;
}

std::string* CAnimationStarController::GetTargetAnimationName() {
    if (m_target_animation != NULL) {
        return m_target_animation->GetName();
    }

    return NULL;
}

F32 CAnimationStarController::GetPlayingAnimationTime() {
    if (m_playing_animation != NULL) {
        int uid = GetUnk2CElem(0);
        CAnimation* animation = m_playing_animation->GetAnimation(0);
        int anim_id = m_controller->FindAnimationFromUID(animation->GetType(), uid);
        return m_controller->GetAnimationTime(animation->GetType(), anim_id);
    }

    return 0.0f;
}

F32 CAnimationStarController::GetPlayingAnimationAbsoluteTime() {
    if (m_playing_animation != NULL) {
        int uid = GetUnk2CElem(0);
        CAnimation* animation = m_playing_animation->GetAnimation(0);
        int anim_id = m_controller->FindAnimationFromUID(animation->GetType(), uid);
        return m_controller->GetAnimationAbsoluteTime(animation->GetType(), anim_id);
    }

    return 0.0f;
}

BOOL CAnimationStarController::IsPlayingAnimationLooped() {
    if (m_playing_animation != NULL) {
        int uid = GetUnk2CElem(0);
        CAnimation* animation = m_playing_animation->GetAnimation(0);
        int anim_id = m_controller->FindAnimationFromUID(animation->GetType(), uid);
        return m_controller->IsAnimationLooped(animation->GetType(), anim_id);
    }

    return FALSE;
}

F32 CAnimationStarController::GetPlayingAnimationLoopTime() {
    if (m_playing_animation != NULL) {
        int uid = GetUnk2CElem(0);
        if (uid < 0) {
            return 0.0f;
        }

        CAnimation* animation = m_playing_animation->GetAnimation(0);
        int anim_id = m_controller->FindAnimationFromUID(animation->GetType(), uid);
        return m_controller->GetAnimationLoopDelta(animation->GetType(), anim_id);
    }

    return 0.0f;
}

F32 CAnimationStarController::GetPlayingAnimationDuration() {
    if (m_playing_animation == NULL) {
        return 0.0f;
    }

    for (int i = 0; i < m_playing_animation->GetNumberOfAnimations(); i++) {
        int uid = GetUnk2CElem(i);

        if (uid >= 0) {
            CAnimation* animation = m_playing_animation->GetAnimation(i);
            int anim_id = m_controller->FindAnimationFromUID(animation->GetType(), uid);
            if (anim_id >= 0) {
                return m_controller->GetAnimationDuration(animation->GetType(), anim_id);
            }
        }
    }

    return 0.0f;
}

F32 CAnimationStarController::GetPlayingAnimationSpeed() {
    if (m_playing_animation == NULL) {
        return 1.0f;
    }

    for (int i = 0; i < m_playing_animation->GetNumberOfAnimations(); i++) {
        int uid = GetUnk2CElem(i);

        if (uid >= 0) {
            CAnimation* animation = m_playing_animation->GetAnimation(i);
            int anim_id = m_controller->FindAnimationFromUID(animation->GetType(), uid);
            if (anim_id >= 0) {
                return m_controller->GetAnimationSpeed(animation->GetType(), anim_id);
            }
        }
    }

    return 0.0f;
}

void CAnimationStarController::SetPlayingAnimationSpeed(F32 speed) {
    if (m_playing_animation == NULL) {
        return;
    }

    if (m_playing_animation_speed == speed) {
        return;
    }

    m_playing_animation_speed = speed;

    for (int i = 0; i < m_playing_animation->GetNumberOfAnimations(); i++) {
        int uid = GetUnk2CElem(i);

        if (uid >= 0) {
            CAnimation* animation = m_playing_animation->GetAnimation(i);
            int anim_id = m_controller->FindAnimationFromUID(animation->GetType(), uid);
            if (anim_id >= 0) {
                m_controller->SetAnimationSpeed(animation->GetType(), anim_id, speed);
            }
        }
    }

    return;
}

F32 CAnimationStarController::GetPlayingAnimationTime(F32) {
    if (m_playing_animation == NULL) {
        return 0.0f;
    }

    for (int i = 0; i < m_playing_animation->GetNumberOfAnimations(); i++) {
        int uid = GetUnk2CElem(i);

        if (uid >= 0) {
            CAnimation* animation = m_playing_animation->GetAnimation(i);
            int anim_id = m_controller->FindAnimationFromUID(animation->GetType(), uid);
            if (anim_id >= 0) {
                return m_controller->GetAnimationTime(animation->GetType(), anim_id);
            }
        }
    }

    return 0.0f;
}

void CAnimationStarController::SetPlayingAnimationTime(F32 time) {
    if (m_playing_animation == NULL) {
        return;
    }

    for (int i = 0; i < m_playing_animation->GetNumberOfAnimations(); i++) {
        int uid = GetUnk2CElem(i);

        if (uid >= 0) {
            CAnimation* animation = m_playing_animation->GetAnimation(i);
            int anim_id = m_controller->FindAnimationFromUID(animation->GetType(), uid);
            if (anim_id >= 0) {
                m_controller->SetAnimationTime(animation->GetType(), anim_id, time);
            }
        }
    }

    return;
}

void CAnimationStarController::SetNextAnimationTime(F32 time) {
    m_next_time = time;
}

// This might be a fakematch
void CAnimationStarController::Update(F32 dt) {
    int status;
    int iVar5;
    int iVar3;
    int uid;
    CAnimation* anim;
    int anim_id;
    CAnimationStarEntry* entry;
    int iVar6;

    if (m_target_animation == m_playing_animation) {
        return;
    }

    status = 4;
    anim = NULL;
    anim_id = -1;
    if (m_playing_animation != NULL) {
        anim = m_playing_animation->GetAnimation(0);
        // FIXME: Potentially a fakematch, should use GetUnk2CElem
        uid = (m_unk30 <= 0) ? -1 : m_unk2C[0];
        anim_id = m_controller->FindAnimationFromUID(anim->GetType(), uid);
        status = m_controller->GetAnimationStatus(anim->GetType(), anim_id);
    }

    if (status == 3 || status == 4 || status == 0 || m_unk28 == TRUE) {
        entry = m_target_animation;
        iVar6 = m_unk24;
        iVar5 = 0;
        SAnimationPath path;
        path.unk0 = 0.0f;
        path.unk4 = 0;
        path.unk8 = 0;
        path.unkC = 0;
        path.unk10 = 0;
        if (m_unk28 == TRUE) {
            if (m_animation_star->FindNextAnimationInPath(m_playing_animation, entry, m_unk1C, &path) == TRUE) {
                if (path.unk8 > 1) {
                    path.unk10 = NULL;
                }
            }
            iVar5 = 1;
            entry = m_target_animation;
        } else if (m_playing_animation != NULL) {
            if (m_animation_star->FindNextAnimationInPath(m_playing_animation, entry, m_unk1C, &path) == TRUE) {
                entry = path.unkC;
                if (path.unk4 == 1) {
                    iVar5 = 1;
                }
            } else {
                iVar6 = 0;
                iVar5 = m_playing_animation->GetAnimationValues()->unk1C;
            }
        }
        if (anim != NULL && iVar6 == 0 && iVar5 == 0) {
            if (m_controller->GetAnimationLoopMode(anim->GetType(), anim_id) == ANIMATION_LOOP_3 && status == 3) {
                if (!m_controller->IsAnimationLooped(anim->GetType(), anim_id)) {
                    return;
                }
            }
        }

        m_unk28 = m_unk24 = iVar3 = 0;
        m_unk1C--;

        // It's on the stack, I don't see how this could be NULL
        if (&path != NULL && iVar5 == 0) {
            iVar3 = path.unk4;
        }
        if (iVar6 == 1) {
            iVar3 = iVar6;
        }

        StopAllAnimations(iVar3);
        StartAnimation(entry, entry->GetAnimationValues(), path.unk10);
    }
}

U32 CAnimationStarController::GetSize() {
    return sizeof(CAnimationStarController) + m_unk30 * sizeof(int);
}

void CAnimationStarController::SetName(std::string name) {
    m_name.assign(name, 0);
}

std::string* CAnimationStarController::GetName() {
    return &m_name;
}

}
