#pragma once on

#include "engine/xmd/CChunkIterator.h"

// TODO
class CGuiManager {
public:
    BOOL LoadGui(DkXmd::CChunkIterator*, int);
    void UnLoadLevel(int);
};
