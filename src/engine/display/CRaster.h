#ifndef ENGINE_DISPLAY_CRASTER_H
#define ENGINE_DISPLAY_CRASTER_H

#include "engine/display/IRaster.h"

namespace DKDSP {

class CRaster : public IRaster {
public:
    virtual int GetWidth();
    virtual int GetHeight();
};

}

#endif
