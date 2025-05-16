#include "CRTCFOVEvent.h"
#include <cstring>
#include <iostream>

CRTCFOVEvent::CRTCFOVEvent(CRTCPlayerEntity* player_entity) : CRTCEvent(player_entity) {
    m_unk14 = 4;
    m_fov = 45.0f;
}

CRTCFOVEvent::~CRTCFOVEvent() {

}

void CRTCFOVEvent::Activate() {
    CRTCSeqKey::Activate();
}

BOOL CRTCFOVEvent::ParseParam(DkXmd::CChunkIterator* iter) {
    DkXmd::CChunkIterator dest;

    if (iter->GetFirstChildChunk(dest)) {
        do {
            if (strcmp(dest.GetName(), "Value") == 0) {
                m_fov = dest.GetFloatValue();
            }
        } while (dest.GetNextSiblingChunk(dest));

        return TRUE;
    } else {
        return FALSE;
    }
}
