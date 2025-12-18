#ifndef ENGINE_WRAP_DKW_NODE3D_H
#define ENGINE_WRAP_DKW_NODE3D_H

#include <rwsdk/baframe.h>
#include "engine/wrap/DKW_ObjPlg.h"

class CDKW_Node3d : CDKW_ObjPlg {
public:
    virtual ~CDKW_Node3d() {}
    virtual RwFrame* GetRwFrame() = 0;
};

#endif
