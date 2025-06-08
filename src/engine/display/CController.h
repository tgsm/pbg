#ifndef ENGINE_DISPLAY_CCONTROLLER_H
#define ENGINE_DISPLAY_CCONTROLLER_H

#include "engine/display/IController.h"

namespace DKDSP {

class CController : public IController {
public:
    virtual void Update(F32 dt);
    virtual int PlayAnimation(IGenericAnimation* animation, int, F32, F32, F32, F32);
    virtual void StopAllAnimations(EANIMATIONTYPE, int);
    virtual int FindAnimationFromUID(EANIMATIONTYPE type, int uid);
    virtual void SetAnimationLoopMode(EANIMATIONTYPE type, int, EANIMATIONLOOP, int);
};

}

#endif
