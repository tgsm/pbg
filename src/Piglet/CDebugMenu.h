#ifndef PIGLET_CDEBUGMENU_H
#define PIGLET_CDEBUGMENU_H

#include <vector>
#include "engine/display/CEngine.h"
#include "engine/input/CInput.h"
#include "engine/wrap/DKW_V2d.h"
#include "CMenuControl.h"

class CGame;

// TODO
class CDebugMenu {
public:
    DKI::CInput* m_input_valid;
    DKI::CInput* m_input_cancel;
    DKI::CInput* m_input_ncontrol;
    DKI::CInput* m_input_pcontrol;
    DKI::CInput* m_input_nvalue;
    DKI::CInput* m_input_pvalue;
    std::vector<CMenuControl*> m_controls;
    int m_current_control_no;
    int m_resolution_width;
    int m_resolution_height;
    int m_cam_raster_width;
    int m_cam_raster_height;
    DKDSP::CTexture* m_texture;
    CDKW_2dPath* m_path;
    CDKW_2dBrush* m_brush;
    CDKW_V2d m_uv1;
    CDKW_V2d m_uv2;
    CDKW_V2d m_uv3;
    CDKW_V2d m_uv4;
    CGame* m_game;
    DKDSP::CEngine* m_display_engine;

public:
    CDebugMenu(CGame* game, char* xmd_path);
    ~CDebugMenu();

    CMenuControl* GetControl(char* name);
    void RemoveControl(char* name);
    U32 UpdateAndDisplay();

    int GetResolutionWidth() { return m_resolution_width; }
    int GetResolutionHeight() { return m_resolution_height; }
};
REQUIRE_SIZE(CDebugMenu, 0x6C);

#endif
