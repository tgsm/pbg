#include "CRTCMessageEvent.h"
#include <cstring>
#include <iostream>

CRTCMessageEvent::CRTCMessageEvent(CRTCPlayerEntity* player_entity) : CRTCEvent(player_entity) {
    m_emitter = "";
    m_receiver = "";
    m_type = "";
    m_param = 0;
    m_unk14 = 5;
}

CRTCMessageEvent::~CRTCMessageEvent() {

}

void CRTCMessageEvent::Activate() {
    CRTCSeqKey::Activate();
    m_player_entity->m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_emitter, m_receiver, m_type, m_param);
}

// Equivalent: regalloc
BOOL CRTCMessageEvent::ParseParam(DkXmd::CChunkIterator* iter) {
    DkXmd::CChunkIterator dest1, dest2;

    BOOL ret;
    BOOL got_type;
    BOOL got_param;
    BOOL got_emitter;
    BOOL got_receiver;

    got_emitter = FALSE;
    got_receiver = got_emitter;
    got_param = got_emitter;
    got_type = got_emitter;

    if (iter->GetFirstChildChunk(dest1)) {
        do {
            if (strcmp(dest1.GetName(), "Emitter") == 0) {
                m_emitter = dest1.GetStringValue();
                got_emitter = TRUE;
            } else if (strcmp(dest1.GetName(), "Receiver") == 0) {
                m_receiver = dest1.GetStringValue();
                got_receiver = TRUE;
            } else if (strcmp(dest1.GetName(), "Param") == 0) {
                m_param = dest1.GetS32Value();
                got_param = TRUE;
            } else if (strcmp(dest1.GetName(), "Type") == 0) {
                m_type = dest1.GetStringValue();
                got_type = TRUE;
            }
        } while (dest1.GetNextSiblingChunk(dest1));

        ret = FALSE;
        if (got_emitter && got_receiver && got_param && got_type) {
            ret = TRUE;
        }
        return ret;
    }

    return FALSE;
}
