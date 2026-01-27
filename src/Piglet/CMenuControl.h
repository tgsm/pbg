#ifndef PIGLET_CMENUCONTROL_H
#define PIGLET_CMENUCONTROL_H

#include <vector>
#include "engine/wrap/DKW_2dTools.h"
#include "engine/wrap/DKW_RGBA.h"
#include "engine/wrap/DKW_V2d.h"
#include "engine/xmd/CChunkIterator.h"
#include "CControlValue.h"

class CDebugMenu;

class CMenuControl {
public:
    CDebugMenu* m_debug_menu;
    char* m_id;
    int m_min;
    int m_max;
    std::vector<CControlValue*> m_control_values;
    int m_value;
    CDKW_V2d m_unk20;
    CDKW_V2d m_unk28;
    CDKW_V2d m_unk30;
    CDKW_V2d m_unk38;
    CDKW_2dPath* m_path;
    CDKW_2dBrush* m_brush;
    CDKW_RGBA m_unk48;
    CDKW_RGBA m_unk4C;

public:
    CMenuControl(CDebugMenu* debug_menu);
    ~CMenuControl();

    virtual BOOL Init(DkXmd::CChunkIterator iter);

    void DestroyValueList();
    void SetRange(int min, int max);
    void NextValue();
    void PrevValue();
    void Render(BOOL);

    void SetValue(int val) { m_value = val; }
    void SetValueMinus1(int val) { m_value = val - 1; }
};
REQUIRE_SIZE(CMenuControl, 0x54);

#endif
