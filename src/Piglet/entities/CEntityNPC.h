#pragma once on

#include "entities/CEntityMesh.h"

enum ENPCGenericBehaviour {
    GENERIC_BEHAVIOUR_0 = 0,
    GENERIC_BEHAVIOUR_4 = 4,
};

class CEntityNPC : public CEntityMesh {
public:
    CEntityNPC(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPC();

    virtual u32 GetType();
    virtual void Reset();
    virtual void Update(f32 dt_maybe);
    virtual void Render(f32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void Save(void*);
    virtual void Restore(void*);
    virtual void ResolveContact(); // FIXME: params
    virtual u32 Create(std::string);
    virtual void UpdateDetectionBehaviour(f32);
    virtual void UpdatePursuitBehaviour(f32);
    virtual void UpdateReturnBaseBehaviour(f32);
    virtual void UpdateFightBehaviour(f32);
    virtual void UpdateGrimaceBehaviour(f32);
    virtual void UpdateSpecialBehaviour(f32);
    virtual void MakeNPCLaughing();
    virtual void MakeNPCSatisfaction();
    virtual void PlayWalkAnim(s32);
    virtual void PlayRunAnim(s32);
    virtual void Idle();
    virtual void UnIdle();

    void SetGenericBehaviour(ENPCGenericBehaviour behaviour);
};
