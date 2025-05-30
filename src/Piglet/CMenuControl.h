#ifndef PIGLET_CMENUCONTROL_H
#define PIGLET_CMENUCONTROL_H

#include <vector>
#include "engine/wrap/DKW_RGBA.h"
#include "engine/wrap/DKW_V2d.h"
#include "engine/xmd/CChunkIterator.h"
#include "CControlValue.h"

class CDebugMenu;

class CMenuControl {
public:
    CDebugMenu* m_debug_menu;
    u8 m_unk4[0x8];
    u32 m_unkC;
    std::vector<size_t> m_control_values;
    u32 m_unk1C;
    CDKW_V2d m_unk20;
    CDKW_V2d m_unk28;
    CDKW_V2d m_unk30;
    CDKW_V2d m_unk38;
    u8 m_unk40[8];
    CDKW_RGBA m_unk48;
    CDKW_RGBA m_unk4C;

public:
    virtual void Init(DkXmd::CChunkIterator iter);

    void DestroyValueList();
    void SetRange(int min, int max);

    void SetUnk1C(int val) { m_unk1C = val; }
    void SetUnk1CMinus1(int val) { m_unk1C = val - 1; }
};
REQUIRE_SIZE(CMenuControl, 0x54);

#endif
