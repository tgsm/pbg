#ifndef ENGINE_VIDEO_CVIDEOENGINE_H
#define ENGINE_VIDEO_CVIDEOENGINE_H

#include "engine/video/IVideoEngine.h"

namespace DKVIDEO {

class CVideoEngine : public IVideoEngine {
protected:
    VideoEngineCallback m_callback;
    F32 m_volume;

public:
    CVideoEngine();
    virtual ~CVideoEngine();

    virtual void Initialize(void*) {}
    virtual void Play(char* filename) = 0;
    virtual void Stop() = 0;
    virtual void SetCallBack(VideoEngineCallback callback);
    virtual F32 GetVolume() { return m_volume; }
    virtual void SetVolume(F32 volume) { m_volume = volume; }
};

}

#endif
