#ifndef PIGLET_ENTITIES_CENTITYNPC_H
#define PIGLET_ENTITIES_CENTITYNPC_H

#include "engine/physics/DkPh_Dynamics.h"
#include "entities/CEntityHero.h"
#include "entities/CEntityMesh.h"
#include "CPSplines.h"
#include "entities/CEntityPhial.h"

enum ENPCGenericBehaviour {
    GENERIC_BEHAVIOUR_0 = 0,
    GENERIC_BEHAVIOUR_1 = 1,
    GENERIC_BEHAVIOUR_2 = 2,
    GENERIC_BEHAVIOUR_4 = 4,
    GENERIC_BEHAVIOUR_5 = 5,
    GENERIC_BEHAVIOUR_6 = 6,
    GENERIC_BEHAVIOUR_7 = 7,
    GENERIC_BEHAVIOUR_8 = 8,
    GENERIC_BEHAVIOUR_9 = 9,
};

// TODO
struct PathPoints {

};

class CEntityNPC : public CEntityMesh {
public:
    U32 m_unkF4;
    ENPCGenericBehaviour m_generic_behaviour;
    CPigSpline* m_unkFC;
    std::string m_unk100;
    F32 m_unk104;
    CDKW_V3d m_unk108;
    F32 m_unk114;
    CDKW_V3d m_unk118;
    CDKW_V3d m_unk124;
    CDKW_V3d m_unk130;
    F32 m_unk13C;
    F32 m_unk140;
    CDKW_V3d m_unk144;
    F32 m_unk150;
    U32 m_unk154;
    F32 m_unk158;
    U8 m_unk15C;
    U8 m_unk15D[0x16C - 0x15D];
    CDKW_V3d m_unk16C;
    CDKW_V3d m_unk178;
    U8 m_unk184[0x190 - 0x184];
    CDKW_V3d m_unk190;
    F32 m_unk19C;
    F32 m_rotation_angle;
    F32 m_unk1A4;
    U8 m_unk1A8;
    U8 m_unk1A9;
    U8 m_unk1AA;
    U8 m_unk1AB;
    U8 m_unk1AC;
    U8 m_unk1AD;
    U8 m_unk1AE[2];
    U32 m_unk1B0;
    F32 m_unk1B4;
    U8 m_unk1B8[0x1C0 - 0x1B8];
    std::vector<PathPoints> m_unk1C0;
    int m_unk1CC;
    U8 m_unk1D0[0x1E0 - 0x1D0];
    CDKW_V3d m_unk1E0;
    U8 m_unk1EC[0x1F8 - 0x1EC];
    U32 m_unk1F8;
    DkPh::BVolumeOBB m_unk1FC;
    CEntityPhial* m_phial;
    DkPh::Dynamics m_dynamics;
    U8 m_unk2B0[0x2C4 - 0x2B0];
    std::string* m_current_anim;
    CPigSpline* m_path_finder_spline;
    DKDSP::CIm3DBatch* m_batch2CC;
    U32 m_unk2D0;

public:
    CEntityNPC(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPC();

    virtual U32 GetType() { return ENTITY_NPC23; }
    virtual void Init();
    virtual void Reset();
    virtual void Update(F32 dt_maybe);
    virtual void Render(F32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual BOOL Save(void*);
    virtual BOOL Restore(void*);
    virtual void ResolveContact(const DkPh::Collider::Body&, int, int);
    virtual void UpdateDetectionBehaviour(F32);
    virtual void UpdatePursuitBehaviour(F32);
    virtual void UpdateReturnBaseBehaviour(F32);
    virtual void UpdateFightBehaviour(F32);
    virtual void UpdateGrimaceBehaviour(F32);
    virtual void UpdateSpecialBehaviour(F32);
    virtual void MakeNPCLaughing();
    virtual void MakeNPCSatisfaction();
    virtual void PlayWalkAnim(BOOL);
    virtual void PlayRunAnim(BOOL);
    virtual void Idle();
    virtual void UnIdle(BOOL play_saved_anim);

    void SetGenericBehaviour(ENPCGenericBehaviour behaviour);
    ENPCGenericBehaviour GetGenericBehaviour();
    BOOL FollowSplinePath(F32, F32, int);
    void SaveBaseBehaviour();
    void ComputeRotationAngle(CDKW_V3d&, CDKW_V3d&);
    BOOL RotateAccordingToGarbageVar(F32);
    void DisplayVisionSector();
    void CreatePathFinder(CDKW_V3d, int, int);
    void SetDeadInSecondPass();
    void SaveCurrentAnim();
    void RestoreSavedAnim();
    F32 GetCurrentVelocity();
    void MakeOtherNPCLaughing();
    CPigSpline* GetPathFinderSpline();
    void MakeFritten();

    CDKW_V3d GetPosDelta() {
        const CDKW_V3d& npc_position = GetPosition();
        const CDKW_V3d& hero_position = m_entity_manager->GetHero()->GetPosition();

        return hero_position - npc_position;
    }
    // FIXME: Probably not correct?
    CDKW_V3d& GetPosDeltaRef() {
        const CDKW_V3d hero_position = m_entity_manager->GetHero()->GetPosition();
        const CDKW_V3d& npc_position = GetPosition();

        return hero_position - npc_position;
    }
};
REQUIRE_SIZE(CEntityNPC, 0x2D4);

#endif
