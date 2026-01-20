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
        if (!(m_rw_geometry->flags & (1 << 24)) && RpGeometryLock(m_rw_geometry, 0xFFF)) {
            ret = TRUE;
        } else {
            ret = FALSE;
        }
        return ret;
    }

    RpGeometry* GetRwGeometry() { return m_rw_geometry; }
    int GetNumberOfMaterials() { return m_rw_geometry->matList.numMaterials; }

    RpMaterial* GetMaterial(int id) const { return m_rw_geometry->matList.materials[id]; }
public:
    RpGeometry* m_rw_geometry;
};

#endif
