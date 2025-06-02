#ifndef ENGINE_SOUND_CSOUND2D_H
#define ENGINE_SOUND_CSOUND2D_H

#include "engine/sound/ISound2D.h"

namespace DKSND {

class CSound2D : public ISound2D {
public:
    virtual ~CSound2D();

    virtual void SetVolume(f32 volume);
    virtual f32 GetVolume();
    virtual void SetPitch(f32 pitch);
    virtual f32 GetPitch();
    virtual void* GetSample(); // unknown return type
    virtual void SetLayer(u32 layer);
    virtual u32 GetLayer();
    virtual void Play(u32, u32);
    virtual void Stop();
    virtual void Pause();
    virtual void Resume();
    virtual void SetLoopMode(int mode);
    virtual int GetLoopMode();
    virtual BOOL IsLooping();
    virtual BOOL IsFinished();
    virtual BOOL IsPlaying();
    virtual u32 GetType();
    virtual void SetPanning(f32);
    virtual f32 GetPanning();
};

}

#endif
