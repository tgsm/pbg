#ifndef PIGLET_CRTCFADEEVENT_H
#define PIGLET_CRTCFADEEVENT_H

#include "CRTCEvent.h"
#include "CRTCFade.h"

class CRTCFadeEvent : public CRTCEvent {
private:
    CRTCFade m_fade;
    u8 filler[4]; // unused, not sure if it's in here or CRTCFade

public:
    CRTCFadeEvent(CRTCPlayerEntity* player_entity);
    virtual ~CRTCFadeEvent();

    virtual void Activate();
    virtual BOOL ParseParam(DkXmd::CChunkIterator* iter);
};
REQUIRE_SIZE(CRTCFadeEvent, 0x2C);

#endif
