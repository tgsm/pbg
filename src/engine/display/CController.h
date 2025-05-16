#ifndef ENGINE_DISPLAY_CCONTROLLER_H
#define ENGINE_DISPLAY_CCONTROLLER_H

#include "engine/display/IController.h"

namespace DKDSP {

class CController : public IController {
public:
    virtual void StopAllAnimations(EANIMATIONTYPE, int);
};

}

#endif
