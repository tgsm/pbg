#ifndef ENGINE_DISPLAY_IEVENTANIMATION_H
#define ENGINE_DISPLAY_IEVENTANIMATION_H

#include "engine/display/IGenericAnimation.h"

namespace DKDSP {

// FIXME: Should this be here?
struct SEVENTANIMKEY {
    F32 unk0;
    int unk4;

    SEVENTANIMKEY() {
        unk0 = 0.0f;
        unk4 = -1;
    }
};

class IEventAnimation : public IGenericAnimation {

};

}

#endif
