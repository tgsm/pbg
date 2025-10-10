#ifndef PIGLET_ENTITIES_CENTITYNPCBASICWOOZLE_H
#define PIGLET_ENTITIES_CENTITYNPCBASICWOOZLE_H

#include "entities/CEntityNPC.h"

class CEntityNPCBasicWoozle : public CEntityNPC {
private:
    F32 m_animation_speed;

public:
    CEntityNPCBasicWoozle(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPCBasicWoozle();

    virtual U32 GetType() {
        return ENTITY_NPC_BASIC_WOOZLE;
    }
    virtual void Reset();
    virtual void Render(F32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void Restore(void*);
    virtual void UpdateDetectionBehaviour(F32);
    virtual void UpdateFightBehaviour(F32);
    virtual void UpdateGrimaceBehaviour(F32);
    virtual void MakeNPCLaughing();
    virtual void MakeNPCSatisfaction();
    virtual void PlayWalkAnim(S32);
    virtual void PlayRunAnim(S32);
    virtual void Idle();
    virtual void UnIdle();
    virtual void UpdateDeathBehaviour(F32);
};
REQUIRE_SIZE(CEntityNPCBasicWoozle, 0x2D8);

#endif
