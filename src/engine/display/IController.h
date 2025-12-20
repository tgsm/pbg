#ifndef ENGINE_DISPLAY_ICONTROLLER_H
#define ENGINE_DISPLAY_ICONTROLLER_H

#include <string>
#include "types.h"

namespace DKDSP {

enum EANIMATIONTYPE {
    ANIMATION_TYPE_ANM,
    ANIMATION_TYPE_DMA,
    ANIMATION_TYPE_TAN,
    ANIMATION_TYPE_EVN,
    ANIMATION_TYPE_4,
};

enum EANIMATIONLOOP {
    ANIMATION_LOOP_3 = 3,
};

class IGenericAnimation;
class ISubController;
class CAnimation;

// TODO
class IController {
public:
    IController();

    virtual void SetEventUserData(void* data) = 0;
    virtual void Play(F32) = 0;
    virtual void Play() = 0;
    virtual void Stop() = 0;
    virtual BOOL IsPlaying() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
    virtual BOOL IsPaused() = 0;
    virtual F32 GetTime() = 0;
    virtual void SetTime(F32 time) = 0;
    virtual F32 GetSpeed() = 0;
    virtual void SetSpeed(F32 speed) = 0;
    virtual int GetNumberOfNodes() = 0;
    virtual std::string& GetName() = 0;
    virtual ISubController* GetSubController(EANIMATIONTYPE type) = 0;
    virtual void Update(F32 dt) = 0;
    virtual BOOL IsHierarchySafe() = 0;
    virtual int GetSize() = 0;
    virtual int GetNumberOfAnimations(EANIMATIONTYPE type) = 0;
    virtual CAnimation* GetAnimation(EANIMATIONTYPE type, int) = 0;
    virtual int PlayAnimation(IGenericAnimation* animation, int, F32, F32, F32, F32) = 0;
    virtual int PlayAnimation(IGenericAnimation* animation, int, F32) = 0;
    virtual int PlayAnimation(IGenericAnimation* animation, int) = 0;
    virtual void StopAnimation(IGenericAnimation* animation, int) = 0;
    virtual void StopAllAnimations(EANIMATIONTYPE, int) = 0;
    virtual BOOL IsAnimationPlaying(IGenericAnimation* animation) = 0;
    virtual void PauseAnimation(IGenericAnimation* animation) = 0;
    virtual void ResumeAnimation(IGenericAnimation* animation) = 0;
    virtual F32 GetAnimationTime(IGenericAnimation* animation) = 0;
    virtual F32 GetAnimationAbsoluteTime(IGenericAnimation* animation) = 0;
    virtual F32 GetAnimationTimeBeforePlay(IGenericAnimation* animation) = 0;
    virtual void SetAnimationTime(F32 time) = 0;
    virtual void SetAnimationSpeed(F32 speed) = 0;
    virtual F32 GetAnimationSpeed(IGenericAnimation* animation) = 0;
    virtual F32 GetAnimationDuration(IGenericAnimation* animation) = 0;
    virtual void SetAnimationLoopMode(IGenericAnimation* animation, EANIMATIONLOOP loop, int) = 0;
    virtual EANIMATIONLOOP GetAnimationLoopMode(IGenericAnimation* animation) = 0;
    virtual F32 GetAnimationWeight(IGenericAnimation* animation) = 0;
    virtual F32 GetAnimationNormalizedWeight(IGenericAnimation* animation) = 0;
    virtual void SetAnimationWeight(IGenericAnimation* animation, F32 weight) = 0;
    virtual void SetAnimationTargetWeight(IGenericAnimation* animation, F32 target_weight) = 0;
    virtual int GetAnimationStatus(IGenericAnimation* animation) = 0;
    virtual void SetAnimationDelayOut(IGenericAnimation* animation, F32 delay_out) = 0;
    virtual void SetAnimationDelayIn(IGenericAnimation* animation, F32 delay_in) = 0;
    virtual BOOL IsAnimationLooped(IGenericAnimation* animation) = 0;
    virtual F32 GetAnimationLoopDelta(IGenericAnimation* animation) = 0;
    virtual int FindAnimation(IGenericAnimation* animation) = 0;
    virtual int FindAnimationFromUID(EANIMATIONTYPE type, int uid) = 0;
    virtual int StopAnimation(EANIMATIONTYPE type, int, int) = 0;
    virtual BOOL IsAnimationPlaying(EANIMATIONTYPE type, int) = 0;
    virtual void PauseAnimation(EANIMATIONTYPE type, int) = 0;
    virtual void ResumeAnimation(EANIMATIONTYPE type, int) = 0;
    virtual F32 GetAnimationTime(EANIMATIONTYPE type, int) = 0;
    virtual F32 GetAnimationAbsoluteTime(EANIMATIONTYPE type, int) = 0;
    virtual void SetAnimationTime(EANIMATIONTYPE type, int, F32 time) = 0;
    virtual void SetAnimationSpeed(EANIMATIONTYPE type, int, F32 speed) = 0;
    virtual F32 GetAnimationSpeed(EANIMATIONTYPE type, int) = 0;
    virtual F32 GetAnimationDuration(EANIMATIONTYPE type, int) = 0;
    virtual void SetAnimationLoopMode(EANIMATIONTYPE type, int, EANIMATIONLOOP, int) = 0;
    virtual EANIMATIONLOOP GetAnimationLoopMode(EANIMATIONTYPE type, int) = 0;
    virtual void SetAnimationWeight(EANIMATIONTYPE type, int, F32 weight) = 0;
    virtual void SetAnimationTargetWeight(EANIMATIONTYPE type, int, F32 target_weight) = 0;
    virtual int GetAnimationStatus(EANIMATIONTYPE type, int) = 0;
    virtual void SetAnimationDelayOut(EANIMATIONTYPE type, int, F32 delay_out) = 0;
    virtual void SetAnimationDelayIn(EANIMATIONTYPE type, int, F32 delay_in) = 0;
    virtual BOOL IsAnimationLooped(EANIMATIONTYPE type, int) = 0;
    virtual F32 GetAnimationLoopDelta(EANIMATIONTYPE type, int) = 0;
};

}

#endif
