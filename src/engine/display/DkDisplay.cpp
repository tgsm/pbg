#include "engine/display/DkDisplay.h"

DKDSP::CEngine* gs_TheDkDisplayEngine;

DKDSP::CEngine* DkDisplayGetEngine() {
    if (gs_TheDkDisplayEngine == NULL) {
        CDKW_GCNEngine::GCNInit();
        gs_TheDkDisplayEngine = new DKDSP::CEngine;
    }
    return gs_TheDkDisplayEngine;
}

void DkDisplayRelease() {
    if (gs_TheDkDisplayEngine != NULL) {
        gs_TheDkDisplayEngine->Release();
        delete gs_TheDkDisplayEngine;
    }
    gs_TheDkDisplayEngine = NULL;
}
