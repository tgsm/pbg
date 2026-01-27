#ifndef PIGLET_CCONTROLVALUE_H
#define PIGLET_CCONTROLVALUE_H

#include "engine/display/CTexture.h"
#include "engine/xmd/CChunkIterator.h"

enum EControlValueType {
    CONTROL_VALUE_TYPE_INT,
    CONTROL_VALUE_TYPE_FLOAT,
    CONTROL_VALUE_TYPE_STRING,
};

class CControlValue {
public:
    int m_type;
    U8 m_value[4];
    DKDSP::CTexture* m_texture;

public:
    CControlValue();
    ~CControlValue();

    int GetS32Value();
    virtual BOOL Init(DkXmd::CChunkIterator iter);
    void SetS32Value(int value);
    void SetTexture(char* name);
};
REQUIRE_SIZE(CControlValue, 0x10);

#endif
