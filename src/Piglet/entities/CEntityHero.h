#pragma once

#include "entities/CEntityMesh.h"

// TODO
class CEntityHero : public CEntityMesh {
protected:
    u8 m_unkF4[0x124 - 0xF4];
    u32 m_unk124;
    u32 m_state;
    u8 m_unk12C[0x13C - 0x12C];
    int m_unk13C;
    u8 m_unk140[0x1CC - 0x140];
    CDKW_V3d m_unk1CC;
    u8 m_unk1D8[4];
    f32 m_unk1DC;
    u8 m_unk1E0[0x244 - 0x1E0];
    f32 m_unk244;
    f32 m_unk248;
    f32 m_unk24C;
    f32 m_unk250;
    f32 m_unk254;
    f32 m_unk258;
    u8 m_unk25C[4];

public:
    struct HeroMoveParams {

    };

    CEntityHero(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityHero();

    virtual void AddFlag(u32 flag);
    virtual void DelFlag(u32 flag);
    virtual u32 GetType() { return ENTITY_HERO; }
    virtual void Reset();
    virtual void Update(f32 dt_maybe);
    virtual void Render(f32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual u32 GetSaveSize() { return 0; }
    virtual void ManageMessage(SDkMessage&);
    virtual void ResolveContact(const DkPh::Collider::Body&, int, int);
    virtual void UpdateAnimations(f32);
    virtual void SetDirection(CDKW_V3d& direction);
    virtual CDKW_V3d GetDirection();
    virtual u32 GetMode();
    virtual u32 GetState() { return m_state; }
    virtual f32 GetSpeed();
    virtual f32 GetMaxSpeed();
    virtual void SetMode(u32 mode) = 0;
    virtual void DicreaseLife(int) = 0;
    virtual CDKW_V3d GetPadDirection();
    virtual u8 GetActions();
    virtual void UpdateActions();
    virtual void UpdateAnimations_Wait(f32, int);
    virtual void UpdateAnimations_Idle(f32);
    virtual void UpdateAnimations_AdventureMode(f32);
    virtual void Move(HeroMoveParams& params, f32);
    virtual void UpdateMoving(f32) = 0;
    virtual void ConvertPadToDirection(CDKW_V3d, CDKW_V3d*, f32*);
    virtual void ResetPadTimer(f32);
    virtual void SetToGround();
};
REQUIRE_SIZE(CEntityHero, 0x260);
