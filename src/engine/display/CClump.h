#ifndef ENGINE_DISPLAY_CCLUMP_H
#define ENGINE_DISPLAY_CCLUMP_H

#include "engine/display/IClump.h"

namespace DKDSP {

class CClump : public IClump {
public:
    virtual CDKW_Frame* GetFrame();
    virtual void SetController(IController*);
    virtual void Update(F32 dt_maybe);
    virtual void SetAnimation(IAnimation*);
    // TODO
};

}

#endif
