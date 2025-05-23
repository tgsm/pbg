#include "CRTCFadeEvent.h"
#include <cstring>
#include <iostream>

CRTCFadeEvent::CRTCFadeEvent(CRTCPlayerEntity* player_entity) : CRTCEvent(player_entity) {
    m_type = EVENT_FADE;
}

CRTCFadeEvent::~CRTCFadeEvent() {

}

void CRTCFadeEvent::Activate() {
    CRTCSeqKey::Activate();

    if (!m_player_entity->IsInFade()) {
        m_player_entity->m_entity_manager->GetGame()->FadeInit(m_fade.m_duration / 1000.0f, CGame::FADE_TYPE_0, 0, 0, 0, 0.0f);
        m_player_entity->StartFade(m_fade.m_type);
    }
}

BOOL CRTCFadeEvent::ParseParam(DkXmd::CChunkIterator* iter) {
    DkXmd::CChunkIterator dest1, unused;

    if (iter->GetFirstChildChunk(dest1)) {
        do {
            if (strcmp(dest1.GetName(), "Type") == 0) {
                if (strcmp(dest1.GetStringValue(), "In") == 0 ||
                    strcmp(dest1.GetStringValue(), "IN") == 0 ||
                    strcmp(dest1.GetStringValue(), "in") == 0) {
                    m_fade.m_type = 0;
                } else if (strcmp(dest1.GetStringValue(), "Out") == 0 ||
                           strcmp(dest1.GetStringValue(), "OUT") == 0 ||
                           strcmp(dest1.GetStringValue(), "out") == 0) {
                    m_fade.m_type = 1;
                } else {
                    m_fade.m_type = 2;
                }
            } else if (strcmp(dest1.GetName(), "Duration") == 0) {
                m_fade.m_duration = dest1.GetS32Value();
            } else if (strcmp(dest1.GetName(), "Color") == 0) {
                if (strcmp(dest1.GetStringValue(), "white") == 0) {
                    m_fade.m_color_maybe = 0;
                } else {
                    m_fade.m_color_maybe = 1;
                }
            }
        } while (dest1.GetNextSiblingChunk(dest1));

        return TRUE;
    }

    return FALSE;
}
