#ifndef ENGINE_DISPLAY_CEVENTANIMATION_H
#define ENGINE_DISPLAY_CEVENTANIMATION_H

#include "engine/display/IEventAnimation.h"

namespace DKDSP {

class CEventAnimation : public IEventAnimation {
public:
    virtual EANIMATIONTYPE GetType();

    void SetDuration(F32 duration);
};

}

#endif
