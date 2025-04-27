#include "entities/CEntityNPCTree.h"
#include "CGamePartIngame.h"
#include <cstring>
#include <iostream>

CEntityNPCTree::CEntityNPCTree(CEntityManager* entity_manager, std::string name) : CEntityNPC(entity_manager, name) {
    SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
    m_unk1A8 = 3;
    m_unkF4 &= ~(1 << 12);
    m_unkF4 |= (1 << 14);
}

CEntityNPCTree::~CEntityNPCTree() {

}

void CEntityNPCTree::Reset() {
    CEntityNPC::Reset();
}

void CEntityNPCTree::UpdateDetectionBehaviour(f32) {
    switch (m_unk1A8) {
        case 0:
            if (m_unk15C == 2) {
                m_animation_star_controller->Play("DETECT_SOUND");
                UpdateAnimations(0.001f);
            } else {
                m_animation_star_controller->Play("DETECT_VISUAL");
                UpdateAnimations(0.001f);
            }
            m_unk1A8 = 1;
            break;
        case 1:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                m_animation_star_controller->Play("WAIT");
                UpdateAnimations(0.001f);
                SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
                m_unk1A8 = 3;
            }
            break;
    }
}

void CEntityNPCTree::UpdateSpecialBehaviour(f32) {
    if (m_animation_star_controller->IsPlayingAnimationLooped()) {
        m_animation_star_controller->Play("WAIT");
        UpdateAnimations(0.001f);
        SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
        m_unk1A8 = 3;
    }
}

void CEntityNPCTree::UpdateFightBehaviour(f32) {
    switch (m_unk1A8) {
        case 101:
            m_animation_star_controller->Play("WAIT");
            UpdateAnimations(0.001f);
            m_unk1A8 = 102;
            break;
        case 102:
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->UnblockFightMode();

            SDkMessage message;
            if (m_entity_manager->GetGame()->GetMailbox()->GetMessage(&message, m_unk0, 1)) {
                if (strcmp(message.unk20, "START_FIGHT") == 0) {
                    m_animation_star_controller->Play("WAIT");
                    UpdateAnimations(0.001f);
                    m_unk1A8 = 103;
                }
            }

            break;
        case 103:
            if (m_unkF4 & (1 << 8)) {
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
                m_unk1A8 = 104;
            }
            break;
        case 105: {
            if (funny_check()) {
                CEntityHero* hero = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->GetCurrentHero();
                m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, hero->m_unk0, "FRITTEN_PIGLET", 0);

                if (m_unkF4 & (1 << 8)) {
                    m_animation_star_controller->Play("LAUGH");
                    UpdateAnimations(0.001f);
                    SetGenericBehaviour(GENERIC_BEHAVIOUR_9);
                }
            }
            break;
        case 100:
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
            m_animation_star_controller->Play("FRIGHTEN_PIGLET");
            UpdateAnimations(0.001f);
            m_unk1A8 = 105;
            break;
        }
    }
}

void CEntityNPCTree::UpdateGrimaceBehaviour(f32) {
    switch (m_unk1A8) {
        case 101:
            m_animation_star_controller->Play("IS_FRIGHTENED", 1, 1);
            UpdateAnimations(0.001f);
            m_unk1A8 = 102;
            break;
        case 102:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                SetGenericBehaviour(GENERIC_BEHAVIOUR_6);
                m_unk1A8 = 103;
                m_unkF4 &= ~((1 << 2) | (1 << 1) | (1 << 0));
                m_unkF4 |= (1 << 3);
            }
    }
}

void CEntityNPCTree::UpdateDeathBehaviour(f32) {

}

void CEntityNPCTree::Render(f32 dt_maybe) {
    if (IsFlagged(1 << 1) == TRUE) {
        CEntityNPC::Render(dt_maybe);
    }
}

void CEntityNPCTree::Restore(void* unk) {
    CEntityNPC::Restore(unk);
}

void CEntityNPCTree::Parse(DkXmd::CChunkIterator iter) {
    CEntityNPC::Parse(iter);
}

void CEntityNPCTree::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityNPC::ParseBehavior(iter, behavior);
}

void CEntityNPCTree::PlayWalkAnim(int) {
    m_animation_star_controller->Play("WAIT");
    UpdateAnimations(0.001f);
}
