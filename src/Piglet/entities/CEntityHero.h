#ifndef PIGLET_ENTITIES_CENTITYHERO_H
#define PIGLET_ENTITIES_CENTITYHERO_H

#include "entities/CEntityMesh.h"

// TODO
class CEntityHero : public CEntityMesh {
protected:
    U8 m_unkF4[0x124 - 0xF4];
    U32 m_unk124;
    U32 m_state;
    U8 m_unk12C[0x13C - 0x12C];
    int m_unk13C;
    U8 m_unk140[0x1CC - 0x140];
    CDKW_V3d m_unk1CC;
    U8 m_unk1D8[4];
    F32 m_unk1DC;
    U8 m_unk1E0[0x244 - 0x1E0];
    F32 m_unk244;
    F32 m_unk248;
    F32 m_unk24C;
    F32 m_unk250;
    F32 m_unk254;
    F32 m_unk258;
    U8 m_unk25C[4];

public:
    struct HeroMoveParams {

    };

    CEntityHero(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityHero();

    virtual void AddFlag(U32 flag);
    virtual void DelFlag(U32 flag);
    virtual U32 GetType() { return ENTITY_HERO; }
    virtual void Reset();
    virtual void Update(F32 dt_maybe);
    virtual void Render(F32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual U32 GetSaveSize() { return 0; }
    virtual void ManageMessage(SDkMessage&);
    virtual void ResolveContact(const DkPh::Collider::Body&, int, int);
    virtual void UpdateAnimations(F32);
    virtual void SetDirection(CDKW_V3d& direction);
    virtual CDKW_V3d GetDirection();
    virtual U32 GetMode();
    virtual U32 GetState() { return m_state; }
    virtual F32 GetSpeed();
    virtual F32 GetMaxSpeed();
    virtual void SetMode(U32 mode) = 0;
    virtual void DicreaseLife(int) = 0;
    virtual CDKW_V3d GetPadDirection();
    virtual U8 GetActions();
    virtual void UpdateActions();
    virtual void UpdateAnimations_Wait(F32, int);
    virtual void UpdateAnimations_Idle(F32);
    virtual void UpdateAnimations_AdventureMode(F32);
    virtual void Move(HeroMoveParams& params, F32);
    virtual void UpdateMoving(F32) = 0;
    virtual void ConvertPadToDirection(CDKW_V3d, CDKW_V3d*, F32*);
    virtual void ResetPadTimer(F32);
    virtual void SetToGround();
};
REQUIRE_SIZE(CEntityHero, 0x260);

#endif
