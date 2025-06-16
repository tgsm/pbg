#ifndef ENGINE_SOUND_ISOUND2D_H
#define ENGINE_SOUND_ISOUND2D_H

#include "engine/sound/ISound.h"

namespace DKSND {

class ISound2D : public ISound {
public:
    virtual ~ISound2D() {}

    virtual void SetVolume(F32 volume) = 0;
    virtual F32 GetVolume() = 0;
    virtual void SetPitch(F32 pitch) = 0;
    virtual F32 GetPitch() = 0;
    virtual CSample* GetSample() = 0;
    virtual void SetLayer(U32 layer) = 0;
    virtual U32 GetLayer() = 0;
    virtual void Play(U32, U32) = 0;
    virtual void Stop() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
    virtual void SetLoopMode(int mode) = 0;
    virtual int GetLoopMode() = 0;
    virtual BOOL IsLooping() = 0;
    virtual BOOL IsFinished() = 0;
    virtual BOOL IsPlaying() = 0;
    virtual U32 GetType() = 0;
    virtual void SetPanning(F32) = 0;
    virtual F32 GetPanning() = 0;
};

}

#endif
