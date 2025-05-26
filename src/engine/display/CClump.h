#ifndef ENGINE_DISPLAY_CCLUMP_H
#define ENGINE_DISPLAY_CCLUMP_H

#include "engine/display/IClump.h"

namespace DKDSP {

class CClump : public IClump {
public:
    virtual CDKW_Frame* GetFrame();
    // TODO
};

}

#endif
