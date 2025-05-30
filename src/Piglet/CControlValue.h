#ifndef PIGLET_CCONTROLVALUE_H
#define PIGLET_CCONTROLVALUE_H

#include "engine/xmd/CChunkIterator.h"

// TODO
class CControlValue {
public:
    u8 m_unk0[0x10];

public:
    CControlValue();
    ~CControlValue();

    int GetS32Value();
    BOOL Init(DkXmd::CChunkIterator iter);
    void SetS32Value(int value);
    void SetTexture(char* name);
};
REQUIRE_SIZE(CControlValue, 0x10);

#endif
