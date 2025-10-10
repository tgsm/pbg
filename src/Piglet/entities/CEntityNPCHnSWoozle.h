#ifndef PIGLET_ENTITIES_CENTITYNPCHNSWOOZLE_H
#define PIGLET_ENTITIES_CENTITYNPCHNSWOOZLE_H

#include "entities/CEntityNPC.h"

class CEntityNPCHnSWoozle : public CEntityNPC {
public:
    F32 m_unk2D4;
    BOOL m_unk2D8;
    F32 m_unk2DC;

public:
    CEntityNPCHnSWoozle(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPCHnSWoozle();

    virtual U32 GetType() { return ENTITY_NPC_HNS_WOOZLE; }
    virtual void Reset();
    virtual void Render(F32 dt);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void Restore(void*);
    virtual void UpdateDetectionBehaviour(F32);
    virtual void UpdateFightBehaviour(F32);
    virtual void UpdateGrimaceBehaviour(F32);
    virtual void UpdateDeathBehaviour(F32);
};
REQUIRE_SIZE(CEntityNPCHnSWoozle, 0x2E0);

#endif
