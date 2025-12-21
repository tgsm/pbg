#ifndef ENGINE_DISPLAY_CCONTROLLERDMORPH_H
#define ENGINE_DISPLAY_CCONTROLLERDMORPH_H

#include "engine/display/ISubController.h"
#include "engine/display/CClump.h"

namespace DKDSP {

class CControllerDMorph : public ISubController {
public:
    U8 m_unk4[0x1C - 0x4];

public:
    ~CControllerDMorph();

    virtual void Release();
    virtual void Create(IHierarchy* hierarchy, int);
    virtual void Update(F32 dt);
    virtual int GetSize();
    virtual int GetNumberOfAnimations();
    virtual CAnimation* GetAnimation(int);
    virtual CHierarchy* GetHierarchy(int);
    virtual void StopAllAnimations(int);
    virtual int FindAnimation(IGenericAnimation* animation);
    virtual int FindAnimationFromUID(int uid);
    virtual int AddAnimation(IGenericAnimation* animation, int, F32, F32, F32, F32);
    virtual void StopAnimation(int, int);
    virtual BOOL IsAnimationPlaying(int);
    virtual void PauseAnimation(int);
    virtual void ResumeAnimation(int);
    virtual F32 GetAnimationTime(int);
    virtual F32 GetAnimationTimeBeforePlay(int);
    virtual void SetAnimationTime(int, F32 time);
    virtual void SetAnimationSpeed(int, F32 speed);
    virtual F32 GetAnimationSpeed(int);
    virtual F32 GetAnimationDuration(int);
    virtual void SetAnimationLoopMode(int, EANIMATIONLOOP loop, int);
    virtual EANIMATIONLOOP GetAnimationLoopMode(int);
    virtual F32 GetAnimationWeight(int);
    virtual F32 GetAnimationNormalizedWeight(int);
    virtual void SetAnimationWeight(int, F32 weight);
    virtual void SetAnimationTargetWeight(int, F32 weight);
    virtual int GetAnimationStatus(int);
    virtual int PredictAnimationStatus(int, F32);
    virtual void SetAnimationDelayOut(int, F32 delay_out);
    virtual void SetAnimationDelayIn(int, F32 delay_in);
    virtual F32 GetAnimationAbsoluteTime(int);
    virtual BOOL IsAnimationLooped(int);
    virtual F32 GetAnimationLoopDelta(int);

    void UpdateClump(CClump* clump);
};
REQUIRE_SIZE(CControllerDMorph, 0x1C);

}

#endif
