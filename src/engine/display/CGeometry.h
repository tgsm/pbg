#ifndef ENGINE_DISPLAY_CGEOMETRY_H
#define ENGINE_DISPLAY_CGEOMETRY_H

#include "engine/display/IGeometry.h"
#include "engine/wrap/DKW_Geometry.h"

namespace DKDSP {

// TODO
class CGeometry : public IGeometry {
public:
    CDKW_Geometry* wrap_geometry;
};

}

#endif
