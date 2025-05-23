#ifndef PIGLET_CRTCEVENT_H
#define PIGLET_CRTCEVENT_H

#include "entities/CRTCPlayerEntity.h"
#include "engine/xmd/CChunkIterator.h"
#include "CRTCSequencer.h"

class CRTCEvent : public CRTCSeqKey {
protected:
    enum EEventType {
        EVENT_NONE,
        EVENT_FX,
        EVENT_FADE,
        EVENT_ADD_COLOR,
        EVENT_FOV,
        EVENT_MESSAGE,
        EVENT_VIBRATION,
        EVENT_GRIMACE,
    };

    CRTCPlayerEntity* m_player_entity;
    EEventType m_type;

public:
    CRTCEvent(CRTCPlayerEntity* player_entity);
    virtual ~CRTCEvent();

    virtual BOOL ParseParam(DkXmd::CChunkIterator* iter);

    BOOL CreateFromFile(DkXmd::CChunkIterator* iter);
    static char* ReadType(DkXmd::CChunkIterator* iter);
};
REQUIRE_SIZE(CRTCEvent, 0x18);

#endif
