#ifndef ENGINE_DISPLAY_CCONTROLLERHANIM_H
#define ENGINE_DISPLAY_CCONTROLLERHANIM_H

#include <vector>
#include "engine/display/ISubController.h"

namespace DKDSP {

class CControllerAnimEntry {
public:
    U8 m_unk0[0x28 - 0x0];
    F32 m_unk28;
    U8 m_unk2C[0x48 - 0x2C];
    int m_unk48;
    U8 m_unk4C[0x54 - 0x4C];
    CHierarchy* m_unk54;
    U8 m_unk58[0x60 - 0x58];
};
REQUIRE_SIZE(CControllerAnimEntry, 0x60);

class CControllerHAnim : public ISubController {
public:
    U8 m_unk4[0xC - 0x4];
    std::vector<CControllerAnimEntry> m_anim_entries;
    U8 m_unk18[0x1C - 0x18];

public:
    ~CControllerHAnim();

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

    CHierarchy* GetHierarchyRealIndex(int);
    int ConvertIndex(int);
    int GetStartNode(int);
    F32* GetWeightAccumulationEntry(int);
};
REQUIRE_SIZE(CControllerHAnim, 0x1C);

}

#endif
