#ifndef PIGLET_CRTCMESSAGEEVENT_H
#define PIGLET_CRTCMESSAGEEVENT_H

#include <string>
#include "CRTCEvent.h"

class CRTCMessageEvent : public CRTCEvent {
private:
    std::string m_emitter;
    std::string m_receiver;
    std::string m_message_type;
    U32 m_param;

public:
    CRTCMessageEvent(CRTCPlayerEntity* player_entity);
    virtual ~CRTCMessageEvent();

    virtual void Activate();
    virtual BOOL ParseParam(DkXmd::CChunkIterator* iter);
};
REQUIRE_SIZE(CRTCMessageEvent, 0x28);

#endif
