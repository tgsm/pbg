#ifndef ENGINE_DISPLAY_IMATERIALANIMATION_H
#define ENGINE_DISPLAY_IMATERIALANIMATION_H

#include "engine/display/IGenericAnimation.h"

namespace DKDSP {

struct SMATERIALANIMKEY {
    F32 unk0;
    F32 unk4;
    F32 unk8;
    F32 unkC;
    F32 unk10;
    F32 unk14;

    SMATERIALANIMKEY() {
        unk0 = 0.0f;
        unk4 = 0.0f;
        unk8 = 1.0f;
        unkC = 1.0f;
        unk10 = 0.0f;
        unk14 = 0.0f;
    }
};
REQUIRE_SIZE(SMATERIALANIMKEY, 0x18);

enum EMATANIMCOMPONENT {
    // TODO
};

enum EMATANIMSCHEME {
    ANIM_SCHEME_0,
};

class IMaterialAnimation : public IGenericAnimation {
public:
    IMaterialAnimation();

    virtual BOOL Create(int nb_keyframes, F32, int) = 0;
    virtual void SetKey(int keyframe_nb, SMATERIALANIMKEY& key) = 0;
    virtual void GetKey(int keyframe_nb, SMATERIALANIMKEY& dest_key) = 0;
    virtual IMaterialAnimation* Copy(IMaterialAnimation*) = 0;
    virtual U32 GetNumberOfKeyframes() = 0;
    virtual void SetInterpolatorType(EMATANIMCOMPONENT component, EMATANIMSCHEME scheme) = 0;
    virtual EMATANIMSCHEME GetInterpolatorType(EMATANIMCOMPONENT component) = 0;
};
REQUIRE_SIZE(IMaterialAnimation, 0x4);

}

#endif
