#ifndef ENGINE_VIDEO_DKVIDEO_H
#define ENGINE_VIDEO_DKVIDEO_H

#include "engine/video/CVideoEngineGCN.h"

extern DKVIDEO::CVideoEngineGCN* gs_TheDkVideoEngine;

#ifdef __cplusplus
extern "C" {
#endif

DKVIDEO::CVideoEngineGCN* DkVideoGetEngine();
void DkVideoRelease();

#ifdef __cplusplus
}
#endif

#endif
