#include "CRTCEvent.h"
#include <cstring>
#include <iostream>

CRTCEvent::CRTCEvent(CRTCPlayerEntity* player_entity) {
    m_type = EVENT_NONE;
    m_player_entity = player_entity;
}

CRTCEvent::~CRTCEvent() {

}

BOOL CRTCEvent::CreateFromFile(DkXmd::CChunkIterator* iter) {
    DkXmd::CChunkIterator dest;

    if (iter->GetFirstChildChunk(dest)) {
        do {
            if (strcmp(dest.GetName(), "Time") == 0) {
                m_unk0 = dest.GetS32Value() / 1000.0f;
            } else if (strcmp(dest.GetName(), "EventType") == 0) {
                // Left empty
            } else if (strcmp(dest.GetName(), "EventData") == 0) {
                ParseParam(&dest);
            }
        } while (dest.GetNextSiblingChunk(dest));
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL CRTCEvent::ParseParam(DkXmd::CChunkIterator* iter) {
    return TRUE;
}

char* CRTCEvent::ReadType(DkXmd::CChunkIterator* iter) {
    DkXmd::CChunkIterator dest;

    if (iter->GetFirstChildChunk(dest)) {
        do {
            if (strcmp(dest.GetName(), "EventType") == 0) {
                return dest.GetStringValue();
            }
        } while (dest.GetNextSiblingChunk(dest));
        return "";
    } else {
        return "";
    }
}
