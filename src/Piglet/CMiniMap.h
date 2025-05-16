#ifndef PIGLET_CMINIMAP_H
#define PIGLET_CMINIMAP_H

#include "engine/xmd/CChunkIterator.h"

// TODO
class CMiniMap {
public:
    BOOL Load(DkXmd::CChunkIterator*);
    BOOL Unload();
};

#endif
