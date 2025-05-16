#ifndef PIGLET_CCAKEY_H
#define PIGLET_CCAKEY_H

#include "engine/xmd/CChunkIterator.h"
#include "engine/wrap/DKW_V3d.h"

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
REQUIRE_SIZE(CCAKey, 0x30);

#endif
