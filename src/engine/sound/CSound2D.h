#ifndef ENGINE_SOUND_CSOUND2D_H
#define ENGINE_SOUND_CSOUND2D_H

#include "engine/sound/ISound2D.h"

namespace DKSND {

class CSound2D : public ISound2D {
public:
    virtual ~CSound2D();

    virtual void SetVolume(F32 volume);
    virtual F32 GetVolume();
    virtual void SetPitch(F32 pitch);
    virtual F32 GetPitch();
    virtual void* GetSample(); // unknown return type
    virtual void SetLayer(U32 layer);
    virtual U32 GetLayer();
    virtual void Play(U32, U32);
    virtual void Stop();
    virtual void Pause();
    virtual void Resume();
    virtual void SetLoopMode(int mode);
    virtual int GetLoopMode();
    virtual BOOL IsLooping();
    virtual BOOL IsFinished();
    virtual BOOL IsPlaying();
    virtual U32 GetType();
    virtual void SetPanning(F32);
    virtual F32 GetPanning();
};

}

#endif
