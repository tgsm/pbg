#ifndef ENGINE_SOUND_CSOUND2D_H
#define ENGINE_SOUND_CSOUND2D_H

#include "engine/sound/ISound2D.h"

namespace DKSND {

class CSound2D : public ISound2D {
public:
    F32 m_panning;
    U8 m_update_flags;
    F32 m_volume;
    U32 m_layer;
    F32 m_pitch;
    CSample* m_sample;
    BOOL m_playing;
    U32 m_unk20;
    BOOL m_paused;
    U32 m_unk28;
    int m_loop_mode;
    U32 m_unk30;
    U8 m_unk34[0x44 - 0x34];

public:
    CSound2D(ISample* sample);
    CSound2D(std::string*);
    virtual ~CSound2D() {
        DestroySound2D();
    }

    virtual void SetVolume(F32 volume);
    virtual F32 GetVolume();
    virtual void SetPitch(F32 pitch);
    virtual F32 GetPitch();
    virtual CSample* GetSample();
    virtual void SetLayer(U32 layer);
    virtual U32 GetLayer();
    virtual void Play(U32 a1, U32 a2) { PlayGeneric(a1, a2); }
    virtual void Stop();
    virtual void Pause();
    virtual void Resume();
    virtual void SetLoopMode(int mode);
    virtual int GetLoopMode();
    virtual BOOL IsLooping();
    virtual BOOL IsFinished();
    virtual BOOL IsPlaying();
    virtual U32 GetType() { return 0; }
    virtual void SetPanning(F32);
    virtual F32 GetPanning();

    void DefaultConstructor(std::string*);
    void DefaultConstructor(ISample* sample);

    void Clear();
    void DestroySound2D();
    void ClearGeneric();
    void Destroy();
    void UpdateSpecific();
    void Restart();
    void ReleaseSpecific();
    void PlayGeneric(U32, U32);

    BOOL CreateSoundSpecific();
    void CreateSound2DSpecific();
};
REQUIRE_SIZE(CSound2D, 0x44);

}

#endif
