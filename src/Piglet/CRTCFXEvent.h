#ifndef PIGLET_CRTCFXEVENT_H
#define PIGLET_CRTCFXEVENT_H

#include <string>
#include "CRTCEvent.h"

class CRTCFXEvent : public CRTCEvent {
private:
    std::string m_name;
    F32 m_position_x;
    F32 m_position_y;
    F32 m_position_z;

public:
    CRTCFXEvent(CRTCPlayerEntity* player_entity);
    virtual ~CRTCFXEvent();

    virtual void Activate();
    virtual BOOL ParseParam(DkXmd::CChunkIterator* iter);
};
REQUIRE_SIZE(CRTCFXEvent, 0x28);

#endif
