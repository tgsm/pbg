#ifndef ENGINE_DISPLAY_CCONTROLLER_H
#define ENGINE_DISPLAY_CCONTROLLER_H

#include "engine/display/CClump.h"
#include "engine/display/CControllerDMorph.h"
#include "engine/display/CControllerEvent.h"
#include "engine/display/CControllerHAnim.h"
#include "engine/display/CControllerMaterial.h"
#include "engine/display/CHierarchy.h"
#include "engine/display/CMaterialAnimationInterpolator.h"
#include "engine/display/IController.h"

namespace DKDSP {

class CController : public IController {
public:
    std::string m_name;
    F32 m_time;
    F32 m_unkC;
    F32 m_speed;
    BOOL m_playing;
    BOOL m_paused;
    U32 m_unk1C;
    BOOL m_hierarchy_is_safe;
    int m_unk24;
    U32 m_unk28;
    CControllerHAnim m_hanim_controller;
    CControllerDMorph m_dmorph_controller;
    CControllerMaterial m_material_controller;
    CMaterialAnimationInterpolator m_material_animation_interpolator;
    CControllerEvent m_event_controller;
    CHierarchy* m_hierarchy;
    CClump* m_clump;
    void* m_user_data;

public:
    CController() {
        m_time = 0.0f;
        m_unkC = 0.0f;
        m_speed = 1.0f;
        m_unk1C = 0;
        m_hierarchy = NULL;
        m_clump = NULL;
        m_playing = FALSE;
        m_paused = FALSE;
        m_hierarchy_is_safe = FALSE;
        m_user_data = NULL;
        m_unk24 = 0;
        m_unk28 = 0;
    }

    ~CController();

    virtual void SetEventUserData(void* data);
    virtual void Play(F32);
    virtual void Play() { Play(0.0f); }
    virtual void Stop();
    virtual BOOL IsPlaying();
    virtual void Pause();
    virtual void Resume();
    virtual BOOL IsPaused();
    virtual F32 GetTime();
    virtual void SetTime(F32 time);
    virtual F32 GetSpeed();
    virtual void SetSpeed(F32 speed);
    virtual int GetNumberOfNodes();
    virtual std::string& GetName();
    virtual ISubController* GetSubController(EANIMATIONTYPE type);
    virtual void Update(F32 dt);
    virtual BOOL IsHierarchySafe();
    virtual int GetSize();
    virtual int GetNumberOfAnimations(EANIMATIONTYPE type);
    virtual CAnimation* GetAnimation(EANIMATIONTYPE type, int id);
    virtual int PlayAnimation(IGenericAnimation* animation, int, F32, F32, F32, F32);
    virtual int PlayAnimation(IGenericAnimation* animation, int a2, F32 a3) { return PlayAnimation(animation, a2, a3, 0.0f, 0.0f, 0.0f); }
    virtual int PlayAnimation(IGenericAnimation* animation, int a2) { return PlayAnimation(animation, a2, 0.0f, 0.0f, 0.0f, 0.0f); }
    virtual void StopAnimation(IGenericAnimation* animation, int);
    virtual void StopAllAnimations(EANIMATIONTYPE, int);
    virtual BOOL IsAnimationPlaying(IGenericAnimation* animation);
    virtual void PauseAnimation(IGenericAnimation* animation);
    virtual void ResumeAnimation(IGenericAnimation* animation);
    virtual F32 GetAnimationTime(IGenericAnimation* animation);
    virtual F32 GetAnimationAbsoluteTime(IGenericAnimation* animation);
    virtual F32 GetAnimationTimeBeforePlay(IGenericAnimation* animation);
    virtual void SetAnimationTime(IGenericAnimation* animation, F32 time);
    virtual void SetAnimationSpeed(IGenericAnimation* animation, F32 speed);
    virtual F32 GetAnimationSpeed(IGenericAnimation* animation);
    virtual F32 GetAnimationDuration(IGenericAnimation* animation);
    virtual void SetAnimationLoopMode(IGenericAnimation* animation, EANIMATIONLOOP loop, int);
    virtual EANIMATIONLOOP GetAnimationLoopMode(IGenericAnimation* animation);
    virtual F32 GetAnimationWeight(IGenericAnimation* animation);
    virtual F32 GetAnimationNormalizedWeight(IGenericAnimation* animation);
    virtual void SetAnimationWeight(IGenericAnimation* animation, F32 weight);
    virtual void SetAnimationTargetWeight(IGenericAnimation* animation, F32 target_weight);
    virtual int GetAnimationStatus(IGenericAnimation* animation);
    virtual void SetAnimationDelayOut(IGenericAnimation* animation, F32 delay_out);
    virtual void SetAnimationDelayIn(IGenericAnimation* animation, F32 delay_in);
    virtual BOOL IsAnimationLooped(IGenericAnimation* animation);
    virtual F32 GetAnimationLoopDelta(IGenericAnimation* animation);
    virtual int FindAnimation(IGenericAnimation* animation);
    virtual int FindAnimationFromUID(EANIMATIONTYPE type, int uid);
    virtual void StopAnimation(EANIMATIONTYPE type, int id, int);
    virtual BOOL IsAnimationPlaying(EANIMATIONTYPE type, int id);
    virtual void PauseAnimation(EANIMATIONTYPE type, int id);
    virtual void ResumeAnimation(EANIMATIONTYPE type, int id);
    virtual F32 GetAnimationTime(EANIMATIONTYPE type, int id);
    virtual F32 GetAnimationAbsoluteTime(EANIMATIONTYPE type, int id);
    virtual void SetAnimationTime(EANIMATIONTYPE type, int id, F32 time);
    virtual void SetAnimationSpeed(EANIMATIONTYPE type, int id, F32 speed);
    virtual F32 GetAnimationSpeed(EANIMATIONTYPE type, int id);
    virtual F32 GetAnimationDuration(EANIMATIONTYPE type, int id);
    virtual void SetAnimationLoopMode(EANIMATIONTYPE type, int id, EANIMATIONLOOP, int);
    virtual EANIMATIONLOOP GetAnimationLoopMode(EANIMATIONTYPE type, int id);
    virtual void SetAnimationWeight(EANIMATIONTYPE type, int id, F32 weight);
    virtual void SetAnimationTargetWeight(EANIMATIONTYPE type, int id, F32 target_weight);
    virtual int GetAnimationStatus(EANIMATIONTYPE type, int id);
    virtual void SetAnimationDelayOut(EANIMATIONTYPE type, int id, F32 delay_out);
    virtual void SetAnimationDelayIn(EANIMATIONTYPE type, int id, F32 delay_in);
    virtual BOOL IsAnimationLooped(EANIMATIONTYPE type, int id);
    virtual F32 GetAnimationLoopDelta(EANIMATIONTYPE type, int id);

    void Release();
    void Create(IClump* clump, IHierarchy* hierarchy, int);
    void SetName(std::string name);
    void UpdateClump(IClump* clump);
};
REQUIRE_SIZE(CController, 0xD0);

}

#endif
