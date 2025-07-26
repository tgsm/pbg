#ifndef ENGINE_DISPLAY_IEVENTANIMATIONCALLBACK_H
#define ENGINE_DISPLAY_IEVENTANIMATIONCALLBACK_H

#include "engine/xmd/CChunkIterator.h"

namespace DKDSP {

class IEventAnimationCallback {
public:
    IEventAnimationCallback();

    virtual void HandleEvent(F32, DkXmd::CChunkIterator*, void*) {}
    virtual void Start(void*) {}
    virtual void Stop(void*) {}
    virtual void Pause(void*) {}
    virtual void Resume(void*) {}
};

}

#endif
