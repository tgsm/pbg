#include "engine/video/DkVideo.h"

DKVIDEO::CVideoEngineGCN* gs_TheDkVideoEngine;

DKVIDEO::CVideoEngineGCN* DkVideoGetEngine() {
    if (gs_TheDkVideoEngine == NULL) {
        gs_TheDkVideoEngine = new DKVIDEO::CVideoEngineGCN;
    }
    return gs_TheDkVideoEngine;
}

void DkVideoRelease() {
    if (gs_TheDkVideoEngine != NULL) {
        delete gs_TheDkVideoEngine;
    }
    gs_TheDkVideoEngine = NULL;
}
