#pragma once on

#include "engine/display/IAnimationStarController.h"

namespace DKDSP {

class CAnimationStarController : public IAnimationStarController {
private:
    u8 m_unk4[0x38];

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
    virtual void Play(std::string, f32);
    virtual void Play(std::string, int);
    virtual void Play(std::string, f32, int);
    virtual void Play(std::string, int, int);
    virtual void Play(std::string, f32, int, int);
    virtual void Play(char*);
    virtual void Play(char*, f32);
    virtual void Play(char*, int);
    virtual void Play(char*, f32, int);
    virtual void Play(char*, int, int);
    virtual void Play(char*, f32, int, int);
    virtual void Update(f32);
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
    virtual char* GetTargetAnimationName();
    virtual f32 GetPlayingAnimationTime();
    virtual f32 GetPlayingAnimationAbsoluteTime();
    virtual BOOL IsPlayingAnimationLooped();
    virtual f32 GetPlayingAnimationLoopTime();
    virtual f32 GetPlayingAnimationDuration();
    virtual f32 GetPlayingAnimationSpeed();
    virtual void SetPlayingAnimationSpeed(f32 speed);
    virtual f32 GetPlayingAnimationTime(f32);
    virtual void SetPlayingAnimationTime(f32 time);
    virtual void SetNextAnimationTime(f32 time);
    virtual u32 GetSize();
    virtual std::string GetName();
};
REQUIRE_SIZE(CAnimationStarController, 0x3C);

}
