#include "CRTCAddColorEvent.h"
#include <cstring>
#include <iostream>

CRTCAddColorEvent::CRTCAddColorEvent(CRTCPlayerEntity* player_entity) : CRTCEvent(player_entity) {
    m_type = EVENT_ADD_COLOR;
}

void CRTCAddColorEvent::Activate() {
    CRTCSeqKey::Activate();
}

BOOL CRTCAddColorEvent::ParseParam(DkXmd::CChunkIterator* iter) {
    DkXmd::CChunkIterator dest, unused;

    if (iter->GetFirstChildChunk(dest)) {
        do {
            if (strcmp(dest.GetName(), "Color") == 0) {
                if (strcmp(dest.GetStringValue(), "red") == 0 ||
                    strcmp(dest.GetStringValue(), "Red") == 0 ||
                    strcmp(dest.GetStringValue(), "RED") == 0) {
                    m_color = COLOR_RED;
                } else if (strcmp(dest.GetStringValue(), "yellow") == 0 ||
                           strcmp(dest.GetStringValue(), "Yellow") == 0 ||
                           strcmp(dest.GetStringValue(), "YELLOW") == 0) {
                    m_color = COLOR_YELLOW;
                } else if (strcmp(dest.GetStringValue(), "blue") == 0 ||
                           strcmp(dest.GetStringValue(), "Blue") == 0 ||
                           strcmp(dest.GetStringValue(), "BLUE") == 0) {
                    m_color = COLOR_BLUE;
                } else {
                    m_color = COLOR_RED;
                    return FALSE;
                }
            }
        } while (dest.GetNextSiblingChunk(dest));

        return TRUE;
    }

    return FALSE;
}
