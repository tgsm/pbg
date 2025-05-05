#pragma once on

#include "engine/xmd/CChunkIterator.h"

// TODO
class CMiniMap {
public:
    BOOL Load(DkXmd::CChunkIterator*);
    BOOL Unload();
};
