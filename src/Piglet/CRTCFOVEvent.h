#ifndef PIGLET_CRTCFOVEVENT_H
#define PIGLET_CRTCFOVEVENT_H

#include "CRTCEvent.h"

class CRTCFOVEvent : public CRTCEvent {
private:
    F32 m_fov;

public:
    CRTCFOVEvent(CRTCPlayerEntity* player_entity);
    virtual ~CRTCFOVEvent();

    virtual void Activate();
    virtual BOOL ParseParam(DkXmd::CChunkIterator* iter);
};
REQUIRE_SIZE(CRTCFOVEvent, 0x1C);

#endif
