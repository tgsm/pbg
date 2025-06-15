#ifndef ENGINE_GUI_DKSOUND_H
#define ENGINE_GUI_DKSOUND_H

#include "engine/sound/CSoundEngine.h"

extern DKSND::CSoundEngine* gs_TheDkSoundEngine;

#ifdef __cplusplus
extern "C" {
#endif

void DkSoundRelease(void);
DKSND::CSoundEngine* DkSoundGetEngine(void);

#ifdef __cplusplus
}
#endif

#endif
