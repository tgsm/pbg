#ifndef ENGINE_SOUND_CSOUND3D_H
#define ENGINE_SOUND_CSOUND3D_H

#include "engine/sound/ISound3D.h"
#include "engine/sound/CSoundEmitter.h"
#include "engine/wrap/DKW_V3d.h"

namespace DKSND {

class CSample;

class CSound3D : public ISound3D {
public:
    virtual ~CSound3D();

    virtual void SetVolume(F32 volume);
    virtual F32 GetVolume();
    virtual void SetPitch(F32 pitch);
    virtual F32 GetPitch();
    virtual CSample* GetSample();
    virtual void SetLayer(U32 layer);
    virtual U32 GetLayer();
    virtual void Play(U32, U32);
    virtual void Stop();
    virtual void Pause();
    virtual void Resume();
    virtual void SetLoopMode(int loop_mode);
    virtual int GetLoopMode();
    virtual BOOL IsLooping();
    virtual BOOL IsFinished();
    virtual BOOL IsPlaying();
    virtual U32 GetType();
    virtual void SetMinDistance(F32 min_distance);
    virtual F32 GetMinDistance();
    virtual void SetPosition(CDKW_V3d* position);
    virtual CSoundEmitter* GetSoundEmitter();

    void SetSoundEmitter(ISoundEmitter* sound_emitter);
};

}

#endif
