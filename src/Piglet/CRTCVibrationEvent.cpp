#include "engine/input/IInputEngine.h"
#include "CRTCVibrationEvent.h"
#include <iostream>

CRTCVibrationEvent::CRTCVibrationEvent(CRTCPlayerEntity* player_entity) : CRTCEvent(player_entity) {
    m_unk18 = 0;
    m_unk14 = 6;
}

CRTCVibrationEvent::~CRTCVibrationEvent() {

}

void CRTCVibrationEvent::Activate() {
    CRTCSeqKey::Activate();
    DKI::IInputEngine::GetDevice(0)->SendVibration(m_unk18);
}

BOOL CRTCVibrationEvent::ParseParam(DkXmd::CChunkIterator* iter) {
    DkXmd::CChunkIterator dest1, unused;

    if (iter->GetFirstChildChunk(dest1)) {
        do {
            if (strcmp(dest1.GetName(), "Value") == 0) {
                m_unk18 = dest1.GetS32Value();
            }
        } while (dest1.GetNextSiblingChunk(dest1));

        return TRUE;
    } else {
        return FALSE;
    }
}
