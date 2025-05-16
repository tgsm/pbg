#ifndef PIGLET_CRTCGRIMACEEVENT_H
#define PIGLET_CRTCGRIMACEEVENT_H

#include "engine/wrap/DKW_V3d.h"
#include "CRTCEvent.h"

class CRTCGrimaceEvent : public CRTCEvent {
private:
    CDKW_V3d m_position;
    CDKW_V3d m_orientation;
    f32 m_duration;
    f32 m_delay;

public:
    CRTCGrimaceEvent(CRTCPlayerEntity* player_entity);
    virtual ~CRTCGrimaceEvent();

    virtual void Activate();
    virtual BOOL ParseParam(DkXmd::CChunkIterator* iter);
};
REQUIRE_SIZE(CRTCGrimaceEvent, 0x38);

#endif
