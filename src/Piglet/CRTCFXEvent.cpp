#include "CRTCFXEvent.h"
#include <cstring>
#include <iostream>

CRTCFXEvent::CRTCFXEvent(CRTCPlayerEntity* player_entity) : CRTCEvent(player_entity) {
    m_type = EVENT_FX;
    m_name = "undefined";
    m_position_x = 0.0f;
    m_position_y = 0.0f;
    m_position_z = 0.0f;
}

CRTCFXEvent::~CRTCFXEvent() {

}

void CRTCFXEvent::Activate() {
    CRTCSeqKey::Activate();
}

BOOL CRTCFXEvent::ParseParam(DkXmd::CChunkIterator* iter) {
    DkXmd::CChunkIterator dest1, dest2;

    if (iter->GetFirstChildChunk(dest1)) {
        do {
            if (strcmp(dest1.GetName(), "Name") == 0) {
                m_name = dest1.GetStringValue();
            } else if (strcmp(dest1.GetName(), "Position") == 0) {
                if (dest1.GetFirstChildChunk(dest2)) {
                    do {
                        if (strcmp(dest2.GetName(), "X") == 0) {
                            m_position_x = dest2.GetFloatValue();
                        } else if (strcmp(dest2.GetName(), "Y") == 0) {
                            m_position_y = dest2.GetFloatValue();
                        } else if (strcmp(dest2.GetName(), "Z") == 0) {
                            m_position_z = dest2.GetFloatValue();
                        }
                    } while (dest2.GetNextSiblingChunk(dest2));
                }
            }
        } while (dest1.GetNextSiblingChunk(dest1));

        return TRUE;
    } else {
        return FALSE;
    }
}
