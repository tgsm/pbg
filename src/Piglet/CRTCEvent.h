#pragma once on

#include "engine/xmd/CChunkIterator.h"
#include "CRTCSequencer.h"

class CRTCPlayerEntity;

class CRTCEvent : public CRTCSeqKey {
protected:
    CRTCPlayerEntity* m_player_entity;
    u32 m_unk14;

public:
    CRTCEvent(CRTCPlayerEntity* player_entity);
    virtual ~CRTCEvent();

    virtual BOOL ParseParam(DkXmd::CChunkIterator* iter);

    BOOL CreateFromFile(DkXmd::CChunkIterator* iter);
    static char* ReadType(DkXmd::CChunkIterator* iter);
};
REQUIRE_SIZE(CRTCEvent, 0x18);
