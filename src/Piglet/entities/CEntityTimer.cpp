#include "entities/CEntityTimer.h"
#include "CGame.h"
#include "CGamePartIngame.h"
#include <cstring>
#include <iostream>

CEntityTimer::CEntityTimer(CEntityManager* entity_manager, std::string name) : CEntity(entity_manager, name) {
    m_elapsed = 0.0f;

    DelFlag(ENTITY_FLAG_ACTIVE);
    DelFlag(ENTITY_FLAG_VISIBLE);
}

CEntityTimer::~CEntityTimer() {

}

void CEntityTimer::Update(F32 dt_maybe) {
    if (IsFlagged(ENTITY_FLAG_UNK7)) {
        return;
    }

    CEntity::Update(dt_maybe);

    if (!IsFlagged(ENTITY_FLAG_ACTIVE)) {
        return;
    }

    if (((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->IsPlayingRTC()) {
        return;
    }

    m_elapsed += dt_maybe;
    if (m_elapsed > m_duration) {
        CMailBox* mailbox = m_entity_manager->GetGame()->GetMailbox();
        mailbox->SendMessage(m_unk0, m_unk0, "TIMEROVER", 0);
        if (IsFlagged(ENTITY_FLAG_ACTIVE)) {
            DelFlag(ENTITY_FLAG_ACTIVE);
            DelFlag(ENTITY_FLAG_VISIBLE);
            m_elapsed = 0.0f;
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->StopTimer();
        }
    } else {
        ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->UpdateTimer(dt_maybe);
        if (IsFlagged(ENTITY_FLAG_VISIBLE)) {
            if (((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->m_unk44 == 0) {
                CMailBox* mailbox = m_entity_manager->GetGame()->GetMailbox();
                mailbox->SendMessage(m_unk0, m_unk0, "TIMEROVER", 0);
                if (IsFlagged(ENTITY_FLAG_ACTIVE)) {
                    DelFlag(ENTITY_FLAG_ACTIVE);
                    DelFlag(ENTITY_FLAG_VISIBLE);
                    m_elapsed = 0.0f;
                    ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->StopTimer();
                }
            } else {
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->DisplayTimer();
            }
        }
    }
}

void CEntityTimer::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest;
    DkXmd::CChunkIterator unused;

    CEntity::Parse(iter);

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "TimerDuration") == 0) {
                m_duration = dest.GetFloatValue();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CEntityTimer::ManageMessage(SDkMessage& message) {
    if (strcmp(message.unk20, "STOP") == 0) {
        if (IsFlagged(ENTITY_FLAG_ACTIVE)) {
            DelFlag(ENTITY_FLAG_ACTIVE);
            DelFlag(ENTITY_FLAG_VISIBLE);
            m_elapsed = 0.0f;

            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->StopTimer();
        }
    } else if (strcmp(message.unk20, "START") == 0) {
        if (!IsFlagged(ENTITY_FLAG_ACTIVE)) {
            AddFlag(ENTITY_FLAG_ACTIVE);
            AddFlag(ENTITY_FLAG_VISIBLE);
            m_elapsed = 0.0f;

            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->InitTimer(m_duration);
        }
    } else if (strcmp(message.unk20, "SET_BEHAVIOR") == 0) {
        m_unk14 = message.unk30;
    }
}

void CEntityTimer::Save(void* unk) {
    DelFlag(ENTITY_FLAG_ACTIVE);
    DelFlag(ENTITY_FLAG_VISIBLE);

    CEntity::Save(unk);
}
