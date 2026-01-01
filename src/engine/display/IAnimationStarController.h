#ifndef ENGINE_DISPLAY_IANIMATIONSTARCONTROLLER_H
#define ENGINE_DISPLAY_IANIMATIONSTARCONTROLLER_H

#include <string>
#include "types.h"

namespace DKDSP {

class IController;
class IAnimationStar;

class IAnimationStarController {
public:
    IAnimationStarController();

    virtual void SetController(IController* controller) = 0;
    virtual IController* GetController() = 0;
    virtual void SetAnimationStar(IAnimationStar* animation_star) = 0;
    virtual IAnimationStar* GetAnimationStar() = 0;
    virtual void SetSearchDepth(int depth) = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
    virtual void Reset() = 0;
    virtual void Play(std::string) = 0;
    virtual void Play(std::string, F32) = 0;
    virtual void Play(std::string, int) = 0;
    virtual void Play(std::string, F32, int) = 0;
    virtual void Play(std::string, int, int) = 0;
    virtual void Play(std::string, F32, int, int) = 0;
    virtual void Play(char*) = 0;
    virtual void Play(char*, F32) = 0;
    virtual void Play(char*, int) = 0;
    virtual void Play(char*, F32, int) = 0;
    virtual void Play(char*, int, int) = 0;
    virtual void Play(char*, F32, int, int) = 0;
    virtual void Update(F32) = 0;
    virtual BOOL IsPlayingTargetAnimation() = 0;
    virtual BOOL IsFullyPlayingTargetAnimation() = 0;
    virtual BOOL IsFullyPlayingCurrentAnimation() = 0;
    virtual BOOL IsPlayingDefaultAnimation() = 0;
    virtual BOOL IsTargetDefaultAnimation() = 0;
    virtual BOOL IsPlayingAnimation(std::string) = 0;
    virtual BOOL IsPlayingAnimation(char*) = 0;
    virtual BOOL IsTargetAnimation(std::string) = 0;
    virtual BOOL IsTargetAnimation(char*) = 0;
    virtual std::string* GetPlayingAnimationName() = 0;
    virtual std::string* GetTargetAnimationName() = 0;
    virtual F32 GetPlayingAnimationTime() = 0;
    virtual F32 GetPlayingAnimationAbsoluteTime() = 0;
    virtual BOOL IsPlayingAnimationLooped() = 0;
    virtual F32 GetPlayingAnimationLoopTime() = 0;
    virtual F32 GetPlayingAnimationDuration() = 0;
    virtual F32 GetPlayingAnimationSpeed() = 0;
    virtual void SetPlayingAnimationSpeed(F32 speed) = 0;
    virtual F32 GetPlayingAnimationTime(F32) = 0;
    virtual void SetPlayingAnimationTime(F32 time) = 0;
    virtual void SetNextAnimationTime(F32 time) = 0;
    virtual U32 GetSize() = 0;
    virtual std::string GetName() = 0;
};

}

#endif
