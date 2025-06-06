#include "entities/CEntityTimer.h"
#include "CGamePartIngame.h"
#include <cstring>
#include <iostream>

CEntityTimer::CEntityTimer(CEntityManager* entity_manager, std::string name) : CEntity(entity_manager, name) {
    m_elapsed = 0.0f;

    DelFlag(1 << 0);
    DelFlag(1 << 1);
}

CEntityTimer::~CEntityTimer() {

}

void CEntityTimer::Update(F32 dt_maybe) {
    if (IsFlagged(1 << 7)) {
        return;
    }

    CEntity::Update(dt_maybe);

    if (!IsFlagged(1 << 0)) {
        return;
    }

    if (((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->IsPlayingRTC()) {
        return;
    }

    m_elapsed += dt_maybe;
    if (m_elapsed > m_duration) {
        CMailBox* mailbox = m_entity_manager->GetGame()->GetMailbox();
        mailbox->SendMessage(m_unk0, m_unk0, "TIMEROVER", 0);
        if (IsFlagged(1 << 0)) {
            DelFlag(1 << 0);
            DelFlag(1 << 1);
            m_elapsed = 0.0f;
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->StopTimer();
        }
    } else {
        ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->UpdateTimer(dt_maybe);
        if (IsFlagged(1 << 1)) {
            if (((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->m_unk44 == 0) {
                CMailBox* mailbox = m_entity_manager->GetGame()->GetMailbox();
                mailbox->SendMessage(m_unk0, m_unk0, "TIMEROVER", 0);
                if (IsFlagged(1 << 0)) {
                    DelFlag(1 << 0);
                    DelFlag(1 << 1);
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
        if (IsFlagged(1 << 0)) {
            DelFlag(1 << 0);
            DelFlag(1 << 1);
            m_elapsed = 0.0f;

            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->StopTimer();
        }
    } else if (strcmp(message.unk20, "START") == 0) {
        if (!IsFlagged(1 << 0)) {
            AddFlag(1 << 0);
            AddFlag(1 << 1);
            m_elapsed = 0.0f;

            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->InitTimer(m_duration);
        }
    } else if (strcmp(message.unk20, "SET_BEHAVIOR") == 0) {
        m_unk14 = message.unk30;
    }
}

void CEntityTimer::Save(void* unk) {
    DelFlag(1 << 0);
    DelFlag(1 << 1);

    CEntity::Save(unk);
}
