#pragma once on

#include "entities/CEntityNPC.h"

class CEntityNPCBasicWoozle : public CEntityNPC {
private:
    u8 m_unk2D4[4];

public:
    CEntityNPCBasicWoozle(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPCBasicWoozle();

    virtual u32 GetType() {
        return ENTITY_NPC_BASIC_WOOZLE;
    }
    virtual void Reset();
    virtual void Render(f32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void Restore(void*);
    virtual void UpdateDetectionBehaviour(f32);
    virtual void UpdateFightBehaviour(f32);
    virtual void UpdateGrimaceBehaviour(f32);
    virtual void MakeNPCLaughing();
    virtual void MakeNPCSatisfaction();
    virtual void PlayWalkAnim(s32);
    virtual void PlayRunAnim(s32);
    virtual void Idle();
    virtual void UnIdle();
    virtual void UpdateDeathBehaviour(f32);
};
REQUIRE_SIZE(CEntityNPCBasicWoozle, 0x2D8);
