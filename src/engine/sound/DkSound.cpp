#include "engine/sound/DkSound.h"

DKSND::CSoundEngine* gs_TheDkSoundEngine;

void DkSoundRelease() {
    if (gs_TheDkSoundEngine != NULL) {
        gs_TheDkSoundEngine->Release();
        delete gs_TheDkSoundEngine;
    }
    gs_TheDkSoundEngine = NULL;
}

DKSND::CSoundEngine* DkSoundGetEngine() {
    if (gs_TheDkSoundEngine == NULL) {
        gs_TheDkSoundEngine = new DKSND::CSoundEngine;
    }
    return gs_TheDkSoundEngine;
}
