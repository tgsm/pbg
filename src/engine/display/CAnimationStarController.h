#ifndef ENGINE_DISPLAY_CANIMATIONSTARCONTROLLER_H
#define ENGINE_DISPLAY_CANIMATIONSTARCONTROLLER_H

#include "engine/display/CController.h"
#include "engine/display/IAnimationStarController.h"

namespace DKDSP {

class CAnimationStarController : public IAnimationStarController {
private:
    std::string m_name;
    CController* m_controller;
    CAnimationStar* m_animation_star;
    CAnimationStarEntry* m_playing_animation;
    CAnimationStarEntry* m_target_animation;
    int m_search_depth;
    int m_unk1C;
    F32 m_unk20;
    int m_unk24;
    BOOL m_unk28;
    int* m_unk2C;
    int m_unk30;
    F32 m_next_time;
    F32 m_playing_animation_speed;

public:
    CAnimationStarController();
    ~CAnimationStarController();

    virtual void SetController(IController* controller);
    virtual CController* GetController();
    virtual void SetAnimationStar(IAnimationStar* animation_star);
    virtual CAnimationStar* GetAnimationStar();
    virtual void SetSearchDepth(int depth);
    virtual void Pause();
    virtual void Resume();
    virtual void Reset();
    virtual BOOL Play(std::string);
    virtual BOOL Play(std::string, F32);
    virtual BOOL Play(std::string, int);
    virtual BOOL Play(std::string, F32, int);
    virtual BOOL Play(std::string, int, int);
    virtual BOOL Play(std::string, F32, int, int);
    virtual BOOL Play(char*);
    virtual BOOL Play(char*, F32);
    virtual BOOL Play(char*, int);
    virtual BOOL Play(char*, F32, int);
    virtual BOOL Play(char*, int, int);
    virtual BOOL Play(char*, F32, int, int);
    virtual void Update(F32 dt);
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
    virtual std::string* GetName();

    void Release();
    U32 StartAnimation(CAnimationStarEntry*, SAnimationStarValues*, STargetAnimationValues*);
    void SetName(std::string name);

    int GetUnk2CElem(int i) {
        if (i < 0 || i >= m_unk30) {
            return -1;
        }

        return m_unk2C[i];
    }

    void StopAllAnimations(int a1) {
        CAnimation* anim;

        if (m_playing_animation != NULL) {
            for (int i = 0; i < m_playing_animation->GetNumberOfAnimations(); i++) {
                int uid = GetUnk2CElem(i);
                if (uid >= 0) {
                    anim = m_playing_animation->GetAnimation(i);
                    int anim_id = m_controller->FindAnimationFromUID(anim->GetType(), uid);
                    m_controller->StopAnimation(anim->GetType(), anim_id, a1);
                }
            }
        } else {
            m_controller->StopAllAnimations(ANIMATION_TYPE_4, a1);
        }
    }
};
REQUIRE_SIZE(CAnimationStarController, 0x3C);

}

#endif
