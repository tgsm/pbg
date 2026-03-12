#ifndef ENGINE_WRAP_DKW_ATOMIC_H
#define ENGINE_WRAP_DKW_ATOMIC_H

#include <rwsdk/world/baclump.h>
#include "engine/wrap/DKW_Node3d.h"
#include "types.h"

class CDKW_Atomic : public CDKW_Node3d {
public:
    U8 m_unk4[8];
    RpAtomic* m_rw_atomic;
};
// Unknown size

#endif
