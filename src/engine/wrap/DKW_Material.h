#ifndef ENGINE_WRAP_DKW_MATERIAL_H
#define ENGINE_WRAP_DKW_MATERIAL_H

#include <rwsdk/world/bamateri.h>
#include "engine/wrap/DKW_ObjPlg.h"

// TODO
class CDKW_Material : public CDKW_ObjPlg {
public:
    static CDKW_Material* GetInstance(RpMaterial* rw_material);

public:
    RpMaterial* rw_material;
};

#endif
