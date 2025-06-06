#ifndef ENGINE_GUI_DKGUI_H
#define ENGINE_GUI_DKGUI_H

#include "engine/gui/CGUIEngine.h"

extern DKGUI::CGUIEngine* gs_TheDkGUIEngine;

#ifdef __cplusplus
extern "C" {
#endif

void DkGUIRelease(void);
DKGUI::CGUIEngine* DkGUIGetEngine(void);

#ifdef __cplusplus
}
#endif

#endif
