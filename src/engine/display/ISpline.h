#ifndef ENGINE_DISPLAY_ISPLINE_H
#define ENGINE_DISPLAY_ISPLINE_H

#include "engine/wrap/DKW_V3d.h"

namespace DKDSP {

// TODO
class ISpline {
public:
    virtual int GetNumberOfControlPoints() = 0;
    virtual CDKW_V3d* GetControlPoint(int, CDKW_V3d*) = 0;
};

}

#endif
