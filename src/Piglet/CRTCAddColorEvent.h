#ifndef PIGLET_CRTCADDCOLOREVENT_H
#define PIGLET_CRTCADDCOLOREVENT_H

#include "CRTCEvent.h"

class CRTCAddColorEvent : public CRTCEvent {
private:
    enum {
        COLOR_RED,
        COLOR_YELLOW,
        COLOR_BLUE,
    } m_color;

public:
    CRTCAddColorEvent(CRTCPlayerEntity* player_entity);
    virtual ~CRTCAddColorEvent() {}

    virtual void Activate();
    virtual BOOL ParseParam(DkXmd::CChunkIterator* iter);
};
REQUIRE_SIZE(CRTCAddColorEvent, 0x1C);

#endif
