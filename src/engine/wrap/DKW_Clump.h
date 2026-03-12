#ifndef ENGINE_WRAP_DKW_CLUMP_H
#define ENGINE_WRAP_DKW_CLUMP_H

#include <rwsdk/plcore/bamemory.h>
#include <rwsdk/world/baclump.h>
#include "engine/wrap/DKW_Atomic.h"
#include "engine/wrap/DKW_Node3d.h"

// TODO
class CDKW_Clump : public CDKW_Node3d {
public:
    RpClump* m_rw_clump;

    static int ms_NbObject;
    static RwFreeList* ms_pMemEntry;

public:
    CDKW_Clump();

    virtual RwFrame* GetRwFrame() {
        return (RwFrame*)m_rw_clump->object.parent;
    }

    CDKW_Atomic* GetFirstAtomic();
    CDKW_Atomic* GetNextAtomic();
};
// Unknown size

#endif
