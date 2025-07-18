#ifndef ENGINE_VIDEO_IVIDEOENGINE_H
#define ENGINE_VIDEO_IVIDEOENGINE_H

#include "types.h"

namespace DKVIDEO {

typedef void(*VideoEngineCallback)(void);

class IVideoEngine {
public:
    virtual ~IVideoEngine() {}

    virtual void Initialize(void*) = 0;
    virtual void Play(char* filename) = 0;
    virtual void Stop() = 0;
    virtual void SetCallBack(VideoEngineCallback callback) = 0;
    virtual F32 GetVolume() = 0;
    virtual void SetVolume(F32 volume) = 0;
};

}

#endif
