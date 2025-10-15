#ifndef PIGLET_ENTITIES_CENTITYNPCTENNISWOOZLE_H
#define PIGLET_ENTITIES_CENTITYNPCTENNISWOOZLE_H

#include "entities/CEntityNPC.h"

class CEntityNPCTennisWoozle : public CEntityNPC {
public:
    F32 m_animation_speed;

public:
    CEntityNPCTennisWoozle(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPCTennisWoozle();

    virtual U32 GetType() { return ENTITY_NPC_TENNIS_WOOZLE; }
    virtual void Reset();
    virtual void UpdateDetectionBehaviour(F32);
    virtual void UpdateFightBehaviour(F32);
    virtual void UpdateGrimaceBehaviour(F32);
    virtual void UpdateDeathBehaviour(F32);
    virtual void Render(F32);
    virtual void Restore(void*);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
};
REQUIRE_SIZE(CEntityNPCTennisWoozle, 0x2D8);

#endif
