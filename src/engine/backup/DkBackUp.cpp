#include "engine/backup/DkBackUp.h"

DKBAK::CGCNBAKEngine* gs_TheEngine;

DKBAK::CGCNBAKEngine* DkBakUpGetEngine() {
    if (gs_TheEngine == NULL) {
        gs_TheEngine = new DKBAK::CGCNBAKEngine;
    }
    return gs_TheEngine;
}

void DkBakUpRelease() {
    if (gs_TheEngine != NULL) {
        delete (DKBAK::IBAKEngine*)gs_TheEngine;
    }
    gs_TheEngine = NULL;
}
