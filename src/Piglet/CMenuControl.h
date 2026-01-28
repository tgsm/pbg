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
    CDKW_V2d m_uv1;
    CDKW_V2d m_uv2;
    CDKW_V2d m_uv3;
    CDKW_V2d m_uv4;
    CDKW_2dPath* m_path;
    CDKW_2dBrush* m_brush;
    CDKW_RGBA m_active_color;
    CDKW_RGBA m_inactive_color;

public:
    CMenuControl(CDebugMenu* debug_menu);
    ~CMenuControl();

    virtual BOOL Init(DkXmd::CChunkIterator iter);

    void DestroyValueList();
    void SetRange(int min, int max);
    void NextValue();
    void PrevValue();
    void Render(BOOL active);

    void SetValue(int val) { m_value = val; }
    void SetValueMinus1(int val) { m_value = val - 1; }
};
REQUIRE_SIZE(CMenuControl, 0x54);

#endif
