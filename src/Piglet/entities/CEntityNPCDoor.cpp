#include "entities/CEntityNPCDoor.h"
#include "entities/CEntityPiglet.h"
#include "CGamePartIngame.h"
#include <cstring>
#include <iostream>

CEntityNPCDoor::CEntityNPCDoor(CEntityManager* entity_manager, std::string name) : CEntityNPC(entity_manager, name) {
    m_unk2D4 = "";
    m_unk2D8 = "";
    m_unk2DC = 0;
    m_unkF4 |= (1 << 14);
    SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
    m_unk1A8 = 3;
}

CEntityNPCDoor::~CEntityNPCDoor() {

}

void CEntityNPCDoor::Reset() {
    CEntityNPC::Reset();
}

void CEntityNPCDoor::UpdateDetectionBehaviour(F32) {
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
                PlayWalkAnim(0);
                UpdateAnimations(0.001f);
                SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
                m_unk1A8 = 3;
            }
            break;
    }
}

void CEntityNPCDoor::UpdateSpecialBehaviour(F32) {
    std::string* anim_name = m_animation_star_controller->GetPlayingAnimationName();
    if (strcmp(anim_name->c_str(), "LAUGH") == 0) {
        if (m_animation_star_controller->IsPlayingAnimationLooped()) {
            m_unkF4 &= ~(1 << 20);
            m_animation_star_controller->Play("WAIT");
            UpdateAnimations(0.001f);
            SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
            m_unk1A8 = 3;
        }
    } else {
        m_animation_star_controller->Play("LAUGH", 1, 1);
        UpdateAnimations(0.001f);
    }
}

void CEntityNPCDoor::UpdateFightBehaviour(F32) {
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
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                CEntityHero* hero = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->GetCurrentHero();
                m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, hero->m_unk0, "FRITTEN_PIGLET", 0);

                if (m_unkF4 & (1 << 8)) {
                    m_unkF4 |= (1 << 20);
                    m_animation_star_controller->Play("LAUGH", 1, 1);
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

// Equivalent: stack offsets
void CEntityNPCDoor::UpdateGrimaceBehaviour(F32) {
    switch (m_unk1A8) {
        case 101:
            m_animation_star_controller->Play("IS_FRIGHTENED", 1, 1);
            UpdateAnimations(0.001f);
            m_unk1A8 = 102;
            break;
        case 102:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                SetGenericBehaviour(GENERIC_BEHAVIOUR_6);
                m_unkF4 &= ~((1 << 2) | (1 << 1) | (1 << 0));
                m_unkF4 |= (1 << 3);
                m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, m_unk2D8, "START", 0);
                m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, m_unk0, "DIE", 0);
            }
            break;
    }
}

void CEntityNPCDoor::UpdateDeathBehaviour(F32) {

}

void CEntityNPCDoor::Render(F32 dt_maybe) {
    if (IsFlagged(1 << 1) == TRUE) {
        CEntityNPC::Render(dt_maybe);
    }
}

void CEntityNPCDoor::Restore(void* unk) {
    CEntityNPC::Restore(unk);
}

void CEntityNPCDoor::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest;
    DkXmd::CChunkIterator unused;

    CEntityNPC::Parse(iter);

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "NPCDoor_RTC_BadGrimace") == 0) {
                m_unk2D4 = dest.GetStringValue();
            } else if (strcmp(buf, "NPCDoor_RTC_Victory") == 0) {
                m_unk2D8 = dest.GetStringValue();
            } else if (strcmp(buf, "NPCDoor_NeededGrimace") == 0) {
                m_unk2DC = dest.GetS32Value();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CEntityNPCDoor::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityNPC::ParseBehavior(iter, behavior);
}

// Equivalent: stack offsets
BOOL CEntityNPCDoor::CanLaunchFightMode() {
    if (m_entity_manager->GetHero()->GetType() == ENTITY_PIGLET) {
        CEntityPiglet* piglet = static_cast<CEntityPiglet*>(m_entity_manager->GetHero());
        if (piglet->m_grimace_level >= m_unk2DC) {
            return TRUE;
        }

        m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, m_unk2D4, "START", 0);
        SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
        m_unk1A8 = 3;
        return FALSE;
    } else {
        return FALSE;
    }
}
