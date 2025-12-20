#ifndef ENGINE_DISPLAY_ISUBCONTROLLER_H
#define ENGINE_DISPLAY_ISUBCONTROLLER_H

#include "engine/display/IController.h"

namespace DKDSP {

class CHierarchy;
class IHierarchy;

class ISubController {
public:
    ISubController();

    virtual void Release() = 0;
    virtual void Create(IHierarchy* hierarchy, int) = 0;
    virtual void Update(F32 dt) = 0;
    virtual int GetSize() = 0;
    virtual int GetNumberOfAnimations() = 0;
    virtual CAnimation* GetAnimation(int) = 0;
    virtual CHierarchy* GetHierarchy(int) = 0;
    virtual void StopAllAnimations(int) = 0;
    virtual int FindAnimation(IGenericAnimation* animation) = 0;
    virtual int FindAnimationFromUID(int uid) = 0;
    virtual int AddAnimation(IGenericAnimation* animation, int, F32, F32, F32, F32) = 0;
    virtual void StopAnimation(int, int) = 0;
    virtual BOOL IsAnimationPlaying(int) = 0;
    virtual void PauseAnimation(int) = 0;
    virtual void ResumeAnimation(int) = 0;
    virtual F32 GetAnimationTime(int) = 0;
    virtual F32 GetAnimationTimeBeforePlay(int) = 0;
    virtual void SetAnimationTime(int, F32 time) = 0;
    virtual void SetAnimationSpeed(int, F32 speed) = 0;
    virtual F32 GetAnimationSpeed(int) = 0;
    virtual F32 GetAnimationDuration(int) = 0;
    virtual void SetAnimationLoopMode(int, EANIMATIONLOOP loop, int) = 0;
    virtual EANIMATIONLOOP GetAnimationLoopMode(int) = 0;
    virtual F32 GetAnimationWeight(int) = 0;
    virtual F32 GetAnimationNormalizedWeight(int) = 0;
    virtual void SetAnimationWeight(int, F32 weight) = 0;
    virtual void SetAnimationTargetWeight(int, F32 weight) = 0;
    virtual int GetAnimationStatus(int) = 0;
    virtual int PredictAnimationStatus(int, F32) = 0;
    virtual void SetAnimationDelayOut(int, F32 delay_out) = 0;
    virtual void SetAnimationDelayIn(int, F32 delay_in) = 0;
    virtual F32 GetAnimationAbsoluteTime(int) = 0;
    virtual BOOL IsAnimationLooped(int) = 0;
    virtual F32 GetAnimationLoopDelta(int) = 0;
};

}

#endif
