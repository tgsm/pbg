#include "engine/gui/DkGUI.h"

DKGUI::CGUIEngine* gs_TheDkGUIEngine;

void DkGUIRelease() {
    if (gs_TheDkGUIEngine != NULL) {
        delete gs_TheDkGUIEngine;
    }
    gs_TheDkGUIEngine = NULL;
}

DKGUI::CGUIEngine* DkGUIGetEngine() {
    if (gs_TheDkGUIEngine == NULL) {
        gs_TheDkGUIEngine = new DKGUI::CGUIEngine;
    }
    return gs_TheDkGUIEngine;
}
