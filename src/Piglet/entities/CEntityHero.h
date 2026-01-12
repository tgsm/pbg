#ifndef PIGLET_ENTITIES_CENTITYHERO_H
#define PIGLET_ENTITIES_CENTITYHERO_H

#include "engine/input/CInput.h"
#include "engine/physics/DkPh_Dynamics.h"
#include "entities/CEntityMesh.h"
#include "CGame.h"

// TODO
class CEntityHero : public CEntityMesh {
protected:
    U32 m_mode;
    F32 m_delta_time;
    DKI::CInput* m_axe_h_input;
    DKI::CInput* m_axe_v_input;
    DKI::CInput* m_action_input;
    DKI::CInput* m_object_a_input;
    DKI::CInput* m_object_b_input;
    DKI::CInput* m_object_c_input;
    DKI::CInput* m_pad_r1_input;
    DKI::CInput* m_pad_r2_input;
    DKI::CInput* m_pad_l1_input;
    DKI::CInput* m_pad_l2_input;
    U32 m_unk124;
    U32 m_state;
    F32 m_unk12C;
    F32 m_pad_timer;
    U32 m_unk134;
    F32 m_unk138;
    int m_unk13C;
    DkPh::Dynamics m_dynamics;
    U8 m_unk1B8[0x1BC - 0x1B8];
    CDKW_V3d m_unk1BC;
    F32 m_unk1C8;
    CDKW_V3d m_unk1CC;
    F32 m_acceleration;
    F32 m_friction;
    F32 m_friction_turn_back;
    F32 m_stop_speed;
    F32 m_speed_rotate;
    F32 m_speed_rotate_turn_back;
    F32 m_gravity;
    F32 m_pad_threshold_run;
    F32 m_pad_threshold_walk;
    F32 m_pad_threshold_slow;
    F32 m_rotate_angle;
    F32 m_turn_back_angle;
    F32 m_turn_back_speed;
    F32 m_unk20C;
    F32 m_speed_slow;
    F32 m_speed_walk;
    F32 m_speed_run;
    F32 m_speed_slow_worried;
    F32 m_speed_walk_worried;
    F32 m_speed_run_panic;
    F32 m_anim_threshold_slow;
    F32 m_anim_threshold_walk;
    F32 m_anim_threshold_run;
    F32 m_anim_threshold_slow_worried;
    F32 m_anim_threshold_walk_worried;
    F32 m_anim_threshold_run_panic;
    F32 m_anim_threshold_idle;
    F32 m_unk244;
    F32 m_unk248;
    F32 m_unk24C;
    F32 m_unk250;
    F32 m_unk254;
    F32 m_unk258;
    int m_unk25C;

public:
    struct HeroMoveParams {
        F32 unk0;
        F32 unk4;
        F32 unk8;
        F32 unkC;
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
    virtual void SetDirection(CDKW_V3d& direction) {
        CDKW_V3d normalized = direction;
        NormalizeInline(normalized);
        m_unk1BC = CDKW_V3d(normalized);
    }
    virtual CDKW_V3d GetDirection();
    virtual U32 GetMode() { return m_mode; }
    virtual U32 GetState() { return m_state; }
    virtual F32 GetSpeed();
    virtual F32 GetMaxSpeed() { return (m_speed_walk >= m_speed_run) ? m_speed_walk : m_speed_run; }
    virtual void SetMode(U32 mode) = 0;
    virtual void DicreaseLife(int) = 0;
    virtual CDKW_V3d GetPadDirection();
    virtual U32 GetActions();
    virtual void UpdateActions();
    virtual void UpdateAnimations_Wait(F32, int);
    virtual void UpdateAnimations_Idle(F32);
    virtual void UpdateAnimations_AdventureMode(F32);
    virtual void Move(HeroMoveParams& params, F32);
    virtual void UpdateMoving(F32) = 0;
    virtual void ConvertPadToDirection(CDKW_V3d, CDKW_V3d*, F32*);
    virtual void ResetPadTimer(F32);
    virtual void SetToGround();

    void NormalizeInline(CDKW_V3d& vec) {
        if (!((vec.x - CDKW_V3d::ZERO.x) * (vec.x - CDKW_V3d::ZERO.x) +
            (vec.y - CDKW_V3d::ZERO.y) * (vec.y - CDKW_V3d::ZERO.y) +
            (vec.z - CDKW_V3d::ZERO.z) * (vec.z - CDKW_V3d::ZERO.z) <= CDKW_V3d::sm_Epsilon * CDKW_V3d::sm_Epsilon)) {
            RwV3dNormalize(&vec, &vec);
        }
    }
};
REQUIRE_SIZE(CEntityHero, 0x260);

#endif
