#ifndef ENGINE_WRAP_DKW_GEOMETRY_H
#define ENGINE_WRAP_DKW_GEOMETRY_H

#include <rwsdk/world/bageomet.h>
#include "engine/wrap/DKW_ObjPlg.h"
#include "types.h"

// TODO
class CDKW_Geometry : public CDKW_ObjPlg {
public:
    BOOL Lock() {
        BOOL ret;
        if (!(rw_geometry->flags & (1 << 24)) && RpGeometryLock(rw_geometry, 0xFFF)) {
            ret = TRUE;
        } else {
            ret = FALSE;
        }
        return ret;
    }


public:
    RpGeometry* rw_geometry;
};

#endif
