#ifndef ENGINE_WRAP_DKW_WORLD_H
#define ENGINE_WRAP_DKW_WORLD_H

#include <rwsdk/plcore/bamemory.h>
#include "engine/wrap/DKW_Clump.h"
#include "engine/wrap/DKW_Camera.h"
#include "engine/wrap/DKW_ObjPlg.h"

class CDKW_Light;

struct RpLight;

class CDKW_World : public CDKW_ObjPlg {
public:
    U8 m_unk4[8];
    RpLight* m_rw_light;

    static RwFreeList* ms_pMemEntry;

public:
    CDKW_World(F32);

    virtual ~CDKW_World();

    void AddCamera(CDKW_Camera*);
    void AddLight(CDKW_Light*);
    void AddClump(CDKW_Clump*);
};

#endif
