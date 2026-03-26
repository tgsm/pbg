#ifndef ENGINE_WRAP_DKW_WORLD_H
#define ENGINE_WRAP_DKW_WORLD_H

#include <rwsdk/plcore/bamemory.h>
#include "engine/wrap/DKW_Camera.h"
#include "engine/wrap/DKW_ObjPlg.h"

class CDKW_Light;

class CDKW_World : public CDKW_ObjPlg {
public:
    static RwFreeList* ms_pMemEntry;

public:
    CDKW_World(F32);

    virtual ~CDKW_World();

    void AddCamera(CDKW_Camera*);
    void AddLight(CDKW_Light*);
};

#endif
