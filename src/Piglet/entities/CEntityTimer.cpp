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

void CEntityTimer::Update(F32 dt) {
    if (IsFlagged(ENTITY_FLAG_UNK7)) {
        return;
    }

    CEntity::Update(dt);

    if (!IsFlagged(ENTITY_FLAG_ACTIVE)) {
        return;
    }

    if (m_entity_manager->GetGame()->GetIngameGamePart()->GetGameRoomManager()->IsPlayingRTC()) {
        return;
    }

    m_elapsed += dt;
    if (m_elapsed > m_duration) {
        CMailBox* mailbox = m_entity_manager->GetGame()->GetMailbox();
        mailbox->SendMessage(m_name, m_name, "TIMEROVER", 0);
        if (IsFlagged(ENTITY_FLAG_ACTIVE)) {
            DelFlag(ENTITY_FLAG_ACTIVE);
            DelFlag(ENTITY_FLAG_VISIBLE);
            m_elapsed = 0.0f;
            m_entity_manager->GetGame()->GetIngameGamePart()->GetGameRoomManager()->StopTimer();
        }
    } else {
        m_entity_manager->GetGame()->GetIngameGamePart()->GetGameRoomManager()->UpdateTimer(dt);
        if (IsFlagged(ENTITY_FLAG_VISIBLE)) {
            if (m_entity_manager->GetGame()->GetIngameGamePart()->GetGameRoomManager()->m_batch44 == NULL) {
                CMailBox* mailbox = m_entity_manager->GetGame()->GetMailbox();
                mailbox->SendMessage(m_name, m_name, "TIMEROVER", 0);
                if (IsFlagged(ENTITY_FLAG_ACTIVE)) {
                    DelFlag(ENTITY_FLAG_ACTIVE);
                    DelFlag(ENTITY_FLAG_VISIBLE);
                    m_elapsed = 0.0f;
                    m_entity_manager->GetGame()->GetIngameGamePart()->GetGameRoomManager()->StopTimer();
                }
            } else {
                m_entity_manager->GetGame()->GetIngameGamePart()->GetGameRoomManager()->DisplayTimer();
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
    if (strcmp(message.type, "STOP") == 0) {
        if (IsFlagged(ENTITY_FLAG_ACTIVE)) {
            DelFlag(ENTITY_FLAG_ACTIVE);
            DelFlag(ENTITY_FLAG_VISIBLE);
            m_elapsed = 0.0f;

            m_entity_manager->GetGame()->GetIngameGamePart()->GetGameRoomManager()->StopTimer();
        }
    } else if (strcmp(message.type, "START") == 0) {
        if (!IsFlagged(ENTITY_FLAG_ACTIVE)) {
            AddFlag(ENTITY_FLAG_ACTIVE);
            AddFlag(ENTITY_FLAG_VISIBLE);
            m_elapsed = 0.0f;

            m_entity_manager->GetGame()->GetIngameGamePart()->GetGameRoomManager()->InitTimer(m_duration);
        }
    } else if (strcmp(message.type, "SET_BEHAVIOR") == 0) {
        m_unk14 = message.param;
    }
}

BOOL CEntityTimer::Save(void* unk) {
    DelFlag(ENTITY_FLAG_ACTIVE);
    DelFlag(ENTITY_FLAG_VISIBLE);

    return CEntity::Save(unk);
}
