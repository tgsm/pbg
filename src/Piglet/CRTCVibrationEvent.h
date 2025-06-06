#ifndef PIGLET_CRTCVIBRATIONEVENT_H
#define PIGLET_CRTCVIBRATIONEVENT_H

#include "CRTCEvent.h"

class CRTCVibrationEvent : public CRTCEvent {
private:
    U32 m_unk18;

public:
    CRTCVibrationEvent(CRTCPlayerEntity* player_entity);
    virtual ~CRTCVibrationEvent();

    virtual void Activate();
    virtual BOOL ParseParam(DkXmd::CChunkIterator* iter);
};
REQUIRE_SIZE(CRTCVibrationEvent, 0x1C);

#endif
