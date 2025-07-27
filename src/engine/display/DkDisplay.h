#ifndef ENGINE_DISPLAY_DKDISPLAY_H
#define ENGINE_DISPLAY_DKDISPLAY_H

#include "engine/display/CEngine.h"

extern "C" DKDSP::CEngine* DkDisplayGetEngine();
extern "C" void DkDisplayRelease();

#endif
