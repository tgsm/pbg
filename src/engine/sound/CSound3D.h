#ifndef ENGINE_SOUND_CSOUND3D_H
#define ENGINE_SOUND_CSOUND3D_H

#include <rwa/rwastream.h>
#include <rwa/rwavirtualvoice.h>
#include "engine/sound/ISound3D.h"
#include "engine/sound/CSoundEmitter.h"

#define SOUND3D_UPDATE_FLAG_VOLUME    (1 << 0)
#define SOUND3D_UPDATE_FLAG_PITCH     (1 << 1)
#define SOUND3D_UPDATE_FLAG_PLAYING   (1 << 2)
#define SOUND3D_UPDATE_FLAG_RESTART   (1 << 3)
#define SOUND3D_UPDATE_FLAG_PAUSED    (1 << 4)
#define SOUND3D_UPDATE_FLAG_UNK5      (1 << 5)
#define SOUND3D_UPDATE_FLAG_LOOP_MODE (1 << 6)

namespace DKSND {

class CSound3D : public ISound3D {
public:
    U8 m_update_flags;
    F32 m_volume;
    U32 m_layer;
    F32 m_pitch;
    CSample* m_sample;
    BOOL m_playing;
    U32 m_unk1C;
    BOOL m_paused;
    U32 m_unk24;
    int m_loop_mode;
    RwaVirtualVoice* m_virtual_voice;
    RwaStream* m_stream;
    U8 m_unk34[0x3C - 0x34];
    BOOL m_finished;
    CSoundEmitter* m_sound_emitter;
    CDKW_V3d* m_position;
    F32 m_min_distance;
    U8 m_unk4C[4];

public:
    explicit CSound3D(ISample* sample);
    explicit CSound3D(std::string* name);
    virtual ~CSound3D();

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
    virtual void SetLoopMode(int loop_mode);
    virtual int GetLoopMode();
    virtual BOOL IsLooping();
    virtual BOOL IsFinished();
    virtual BOOL IsPlaying();
    virtual U32 GetType() { return 1; }
    virtual void SetMinDistance(F32 min_distance);
    virtual F32 GetMinDistance();
    virtual void SetPosition(CDKW_V3d* position);
    virtual CSoundEmitter* GetSoundEmitter();

    void Clear();
    void Clear3D();
    void DestroySound3D();
    void SetSoundEmitter(ISoundEmitter* sound_emitter);
    void DefaultConstructor(std::string* name);
    void DefaultConstructor(ISample* sample);
    void ClearGeneric();
    void Destroy();
    void PlayGeneric(U32, U32);
    void Restart();
    BOOL CreateSoundSpecific();
    BOOL ReleaseSpecific();
    BOOL GlobalUpdateSpecific();
    RwaVirtualVoice* RWAGetVirtualVoice();

    BOOL CreateSound3DSpecific();
    void UpdateSound3DSpecific();
    void ReleaseSound3DSpecific();
};
REQUIRE_SIZE(CSound3D, 0x50);

}

#endif
