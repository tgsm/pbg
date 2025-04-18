#pragma once on

#include "engine/CChunkIterator.h"
#include "engine/DKW_V3d.h"

class CCAKey {
private:
    char* m_name;
    CDKW_V3d m_unk4;
    CDKW_V3d m_unk10;
    CDKW_V3d m_unk1C;
    f32 m_fov;
    f32 m_roll;

public:
    CCAKey(DkXmd::CChunkIterator* iter, char* name);
    ~CCAKey();

    char* GetName();
};
