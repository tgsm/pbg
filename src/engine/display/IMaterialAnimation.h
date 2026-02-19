#ifndef ENGINE_DISPLAY_IMATERIALANIMATION_H
#define ENGINE_DISPLAY_IMATERIALANIMATION_H

#include "engine/display/IGenericAnimation.h"

namespace DKDSP {

struct SMATERIALANIMKEY {
    U8 unk0[0x18];
};
REQUIRE_SIZE(SMATERIALANIMKEY, 0x18);

enum EMATANIMCOMPONENT {
    // TODO
};

enum EMATANIMSCHEME {
    // TODO
};

class IMaterialAnimation : public IGenericAnimation {
public:
    IMaterialAnimation();

    virtual BOOL Create() = 0;
    virtual void SetKey(U32 keyframe_nb, SMATERIALANIMKEY& key) = 0;
    virtual void GetKey(U32 keyframe_nb, SMATERIALANIMKEY& dest_key) = 0;
    virtual IMaterialAnimation* Copy(IMaterialAnimation*) = 0;
    virtual U32 GetNumberOfKeyframes() = 0;
    virtual void SetInterpolatorType(EMATANIMCOMPONENT component, EMATANIMSCHEME scheme) = 0;
    virtual EMATANIMSCHEME GetInterpolatorType(EMATANIMCOMPONENT component) = 0;
};
REQUIRE_SIZE(IMaterialAnimation, 0x4);

}

#endif
