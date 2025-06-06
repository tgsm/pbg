#ifndef ENGINE_DISPLAY_CANIMATIONSTARCONTROLLER_H
#define ENGINE_DISPLAY_CANIMATIONSTARCONTROLLER_H

#include "engine/display/IAnimationStarController.h"

namespace DKDSP {

class CAnimationStarController : public IAnimationStarController {
private:
    U8 m_unk4[0x38];

public:
    CAnimationStarController();
    ~CAnimationStarController();

    virtual void SetController(IController* controller);
    virtual IController* GetController();
    virtual void SetAnimationStar(IAnimationStar* animation_star);
    virtual IAnimationStar* GetAnimationStar();
    virtual void SetSearchDepth(int depth);
    virtual void Pause();
    virtual void Resume();
    virtual void Reset();
    virtual void Play(std::string);
    virtual void Play(std::string, F32);
    virtual void Play(std::string, int);
    virtual void Play(std::string, F32, int);
    virtual void Play(std::string, int, int);
    virtual void Play(std::string, F32, int, int);
    virtual void Play(char*);
    virtual void Play(char*, F32);
    virtual void Play(char*, int);
    virtual void Play(char*, F32, int);
    virtual void Play(char*, int, int);
    virtual void Play(char*, F32, int, int);
    virtual void Update(F32);
    virtual BOOL IsPlayingTargetAnimation();
    virtual BOOL IsFullyPlayingTargetAnimation();
    virtual BOOL IsFullyPlayingCurrentAnimation();
    virtual BOOL IsPlayingDefaultAnimation();
    virtual BOOL IsTargetDefaultAnimation();
    virtual BOOL IsPlayingAnimation(std::string);
    virtual BOOL IsPlayingAnimation(char*);
    virtual BOOL IsTargetAnimation(std::string);
    virtual BOOL IsTargetAnimation(char*);
    virtual std::string* GetPlayingAnimationName();
    virtual std::string* GetTargetAnimationName();
    virtual F32 GetPlayingAnimationTime();
    virtual F32 GetPlayingAnimationAbsoluteTime();
    virtual BOOL IsPlayingAnimationLooped();
    virtual F32 GetPlayingAnimationLoopTime();
    virtual F32 GetPlayingAnimationDuration();
    virtual F32 GetPlayingAnimationSpeed();
    virtual void SetPlayingAnimationSpeed(F32 speed);
    virtual F32 GetPlayingAnimationTime(F32);
    virtual void SetPlayingAnimationTime(F32 time);
    virtual void SetNextAnimationTime(F32 time);
    virtual U32 GetSize();
    virtual std::string GetName();
};
REQUIRE_SIZE(CAnimationStarController, 0x3C);

}

#endif
