#ifndef PIGLET_ENTITIES_CENTITYNPC_H
#define PIGLET_ENTITIES_CENTITYNPC_H

#include "entities/CEntityHero.h"
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
    U32 m_unkF4;
    U8 m_unkF8[0x15C - 0xF8];
    U8 m_unk15C;
    U8 m_unk15D[0x1A0 - 0x15D];
    F32 m_rotation_angle;
    U8 m_unk1A4[0x1A8 - 0x1A4];
    U8 m_unk1A8;
    U8 m_unk1A9[0x2C4 - 0x1A9];
    std::string* m_unk2C4;
    U8 m_unk2c8[0x2D4 - 0x2C8];

public:
    CEntityNPC(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPC();

    virtual U32 GetType();
    virtual void Init();
    virtual void Reset();
    virtual void Update(F32 dt_maybe);
    virtual void Render(F32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void Save(void*);
    virtual void Restore(void*);
    virtual void ResolveContact(const DkPh::Collider::Body&, int, int);
    virtual U32 Create(std::string);
    virtual void UpdateDetectionBehaviour(F32);
    virtual void UpdatePursuitBehaviour(F32);
    virtual void UpdateReturnBaseBehaviour(F32);
    virtual void UpdateFightBehaviour(F32);
    virtual void UpdateGrimaceBehaviour(F32);
    virtual void UpdateSpecialBehaviour(F32);
    virtual void MakeNPCLaughing();
    virtual void MakeNPCSatisfaction();
    virtual void PlayWalkAnim(S32);
    virtual void PlayRunAnim(S32);
    virtual void Idle();
    virtual void UnIdle();

    void SetGenericBehaviour(ENPCGenericBehaviour behaviour);
    BOOL FollowSplinePath(F32, F32, int);
    void ComputeRotationAngle(CDKW_V3d&, CDKW_V3d&);
    BOOL RotateAccordingToGarbageVar(F32);

    CDKW_V3d GetPosDelta() {
        const CDKW_V3d& npc_position = GetPosition();
        const CDKW_V3d& hero_position = m_entity_manager->GetHero()->GetPosition();

        CDKW_V3d pos_delta;
        pos_delta.m_x = hero_position.m_x - npc_position.m_x;
        volatile F32 x = pos_delta.m_x;
        pos_delta.m_y = hero_position.m_y - npc_position.m_y;
        volatile F32 y = pos_delta.m_y;
        pos_delta.m_z = hero_position.m_z - npc_position.m_z;
        volatile F32 z = pos_delta.m_z;
        // volatile CDKW_V3d unused = pos_delta;
        return pos_delta;
    }
};
REQUIRE_SIZE(CEntityNPC, 0x2D4);

#endif
