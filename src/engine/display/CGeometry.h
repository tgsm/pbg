#ifndef ENGINE_DISPLAY_CGEOMETRY_H
#define ENGINE_DISPLAY_CGEOMETRY_H

#include "engine/display/IGeometry.h"
#include "engine/wrap/DKW_Geometry.h"

namespace DKDSP {

// TODO
class CGeometry : public IGeometry {
public:
    CDKW_Geometry* GetDkWrapGeometry() { return m_wrap_geometry; }

public:
    CDKW_Geometry* m_wrap_geometry;
};

}

#endif
