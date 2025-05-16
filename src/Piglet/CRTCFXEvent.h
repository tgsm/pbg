#pragma once on

#include <string>
#include "CRTCEvent.h"

class CRTCFXEvent : public CRTCEvent {
private:
    std::string m_name;
    f32 m_position_x;
    f32 m_position_y;
    f32 m_position_z;

public:
    CRTCFXEvent(CRTCPlayerEntity* player_entity);
    virtual ~CRTCFXEvent();

    virtual void Activate();
    virtual BOOL ParseParam(DkXmd::CChunkIterator* iter);
};
REQUIRE_SIZE(CRTCFXEvent, 0x28);
