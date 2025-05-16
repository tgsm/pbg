#ifndef PIGLET_ENTITIES_CENTITYNPC_H
#define PIGLET_ENTITIES_CENTITYNPC_H

#include "entities/CEntityMesh.h"

enum ENPCGenericBehaviour {
    GENERIC_BEHAVIOUR_0 = 0,
    GENERIC_BEHAVIOUR_2 = 2,
    GENERIC_BEHAVIOUR_4 = 4,
    GENERIC_BEHAVIOUR_6 = 6,
    GENERIC_BEHAVIOUR_9 = 9,
};

class CEntityNPC : public CEntityMesh {
protected:
    u32 m_unkF4;
    u8 m_unkF8[0x15C - 0xF8];
    u8 m_unk15C;
    u8 m_unk15D[0x1A0 - 0x15D];
    f32 m_rotation_angle;
    u8 m_unk1A4[0x1A8 - 0x1A4];
    u8 m_unk1A8;
    u8 m_unk1A9[0x2C4 - 0x1A9];
    std::string* m_unk2C4;
    u8 m_unk2c8[0x2D4 - 0x2C8];

public:
    CEntityNPC(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPC();

    virtual u32 GetType();
    virtual void Init();
    virtual void Reset();
    virtual void Update(f32 dt_maybe);
    virtual void Render(f32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void Save(void*);
    virtual void Restore(void*);
    virtual void ResolveContact(const DkPh::Collider::Body&, int, int);
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
REQUIRE_SIZE(CEntityNPC, 0x2D4);

#endif
