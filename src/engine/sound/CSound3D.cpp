#include "engine/sound/CSound3D.h"
#include "engine/sound/DkSound.h"

namespace DKSND {

CSound3D::CSound3D(ISample* sample) {
    DefaultConstructor(sample);
    Clear3D();

    if (m_sample == NULL) {
        return;
    }

    if (!CreateSound3DSpecific()) {
        if (m_sample->m_unk0 > 0) {
            m_sample->m_unk0--;
        }
        m_sample = NULL;
    }
}

CSound3D::CSound3D(std::string* name) {
    DefaultConstructor(name);
    Clear3D();

    if (m_sample == NULL) {
        return;
    }

    if (!m_sample->Is3D()) {
        if (m_sample->m_unk0 > 0) {
            m_sample->m_unk0--;
        }
        m_sample = NULL;
        return;
    }

    if (!CreateSound3DSpecific()) {
        if (m_sample->m_unk0 > 0) {
            m_sample->m_unk0--;
        }
        m_sample = NULL;
    }
}

void CSound3D::Clear() {
    DONT_INLINE_HACK();
    ClearGeneric();
    Clear3D();
}

void CSound3D::Clear3D() {
    m_sound_emitter = NULL;
    m_position = NULL;
    SetMinDistance(1.0f);
}

void CSound3D::DestroySound3D() {
    DONT_INLINE_HACK();
    ReleaseSound3DSpecific();
    Destroy();
}

void CSound3D::SetMinDistance(F32 min_distance) {
    m_min_distance = min_distance;
    UpdateSound3DSpecific();
}

F32 CSound3D::GetMinDistance() {
    return m_min_distance;
}

void CSound3D::SetPosition(CDKW_V3d* position) {
    m_position = position;
    UpdateSound3DSpecific();
}

void CSound3D::SetSoundEmitter(ISoundEmitter* sound_emitter_) {
    CSoundEmitter* sound_emitter = (CSoundEmitter*)sound_emitter_;
    if (m_sound_emitter != NULL) {
        m_sound_emitter->RemoveSound(this, 0);
    }
    m_sound_emitter = sound_emitter;
}

CSoundEmitter* CSound3D::GetSoundEmitter() {
    return m_sound_emitter;
}

void CSound3D::DefaultConstructor(std::string* name) {
    Clear();

    m_sample = DkSoundGetEngine()->GetSampleDictionary()->FindSample(name, 1);
    if (m_sample != NULL) {
        m_sample->m_unk0++;
    }

    if (!CreateSoundSpecific()) {
        if (m_sample != NULL && m_sample->m_unk0 > 0) {
            m_sample->m_unk0--;
        }
        m_sample = NULL;
    }
}

void CSound3D::DefaultConstructor(ISample* sample) {
    ClearGeneric();
    m_sample = (CSample*)sample;

    if (m_sample != NULL) {
        m_sample->m_unk0++;
    }

    if (!CreateSoundSpecific()) {
        if (m_sample != NULL && m_sample->m_unk0 > 0) {
            m_sample->m_unk0--;
        }
        m_sample = NULL;
    }
}

void CSound3D::ClearGeneric() {
    m_volume = 1.0f;
    m_pitch = 1.0f;
    m_sample = NULL;
    m_playing = FALSE;
    m_paused = FALSE;
    m_layer = 0;
    m_unk24 = 0;
    m_update_flags = 0;
    m_loop_mode = 0;
    m_unk1C = 0;
    m_virtual_voice = NULL;
}

void CSound3D::Destroy() {
    if (m_sample != NULL && m_sample->m_unk0 > 0) {
        m_sample->m_unk0--;
    }
    ReleaseSpecific();
}

void CSound3D::PlayGeneric(U32 a1, U32 a2) {
    DONT_INLINE_HACK();

    if (!m_playing) {
        m_playing = TRUE;
        m_update_flags |= SOUND3D_UPDATE_FLAG_PLAYING;
    } else if (a1 == 1) {
        Restart();
    }

    if (m_paused && a2 != 0) {
        m_paused = FALSE;
        m_update_flags |= SOUND3D_UPDATE_FLAG_PAUSED;
        if (a2 == 1) {
            Restart();
        }
    }

    GlobalUpdateSpecific();
    m_unk1C = 0;
}

void CSound3D::Stop() {
    if (m_playing) {
        m_playing = FALSE;
        m_update_flags |= SOUND3D_UPDATE_FLAG_PLAYING;
    }

    if (m_paused) {
        m_paused = FALSE;
        m_update_flags |= SOUND3D_UPDATE_FLAG_PAUSED;
    }

    GlobalUpdateSpecific();
}

void CSound3D::Resume() {
    if (m_paused) {
        m_paused = FALSE;
        m_update_flags |= SOUND3D_UPDATE_FLAG_PAUSED;
        GlobalUpdateSpecific();
    }
}

void CSound3D::Pause() {
    if (!m_paused) {
        m_paused = TRUE;
        m_update_flags |= SOUND3D_UPDATE_FLAG_PAUSED;
        GlobalUpdateSpecific();
    }
}

void CSound3D::SetLayer(U32 layer) {
    m_layer = layer;
    DkSoundGetEngine()->AddLayer(layer);
}

void CSound3D::SetVolume(F32 volume) {
    m_volume = volume;
    m_update_flags |= SOUND3D_UPDATE_FLAG_VOLUME;
    GlobalUpdateSpecific();
}

void CSound3D::SetPitch(F32 pitch) {
    m_pitch = pitch;
    m_update_flags |= SOUND3D_UPDATE_FLAG_PITCH;
    GlobalUpdateSpecific();
}

void CSound3D::SetLoopMode(int loop_mode) {
    m_loop_mode = loop_mode;
    m_update_flags |= SOUND3D_UPDATE_FLAG_LOOP_MODE;
    GlobalUpdateSpecific();
}

U32 CSound3D::GetLayer() {
    return m_layer;
}

F32 CSound3D::GetVolume() {
    return m_volume;
}

F32 CSound3D::GetPitch() {
    return m_pitch;
}

CSample* CSound3D::GetSample() {
    return m_sample;
}

int CSound3D::GetLoopMode() {
    return m_loop_mode;
}

void CSound3D::Restart() {
    m_unk1C = 1;
    m_update_flags |= SOUND3D_UPDATE_FLAG_RESTART;
}

BOOL CSound3D::IsPlaying() {
    return m_playing;
}

BOOL CSound3D::CreateSoundSpecific() {
    m_finished = FALSE;
    m_stream = NULL;

    CSample* sample = GetSample();
    if (sample == NULL) {
        return FALSE;
    }

    if (!sample->IsStreamed()) {
        m_virtual_voice = NULL;
        m_virtual_voice = DkSoundGetEngine()->RWAGetFreeVirtualVoice();
        if (m_virtual_voice == NULL) {
            return FALSE;
        }
        _rwaVirtualVoiceSetWave(m_virtual_voice, sample->RWAGetOSWave());
        m_virtual_voice->unk7C = 1.0f;
        m_update_flags = 0;
        m_update_flags = SOUND3D_UPDATE_FLAG_LOOP_MODE | SOUND3D_UPDATE_FLAG_PLAYING | SOUND3D_UPDATE_FLAG_PITCH | SOUND3D_UPDATE_FLAG_VOLUME;
        GlobalUpdateSpecific();
    }

    return TRUE;
}

BOOL CSound3D::ReleaseSpecific() {
    CSample* sample = GetSample();
    if (sample != NULL && !sample->IsStreamed()) {
        if (m_virtual_voice != NULL) {
            _rwaVirtualVoiceSetTrigger(m_virtual_voice, 0);
            DkSoundGetEngine()->RWAFreeVirtualVoice(m_virtual_voice);
        }
        m_virtual_voice = NULL;
    }

    return TRUE;
}

RwaVirtualVoice* CSound3D::RWAGetVirtualVoice() {
    return m_virtual_voice;
}

BOOL CSound3D::IsFinished() {
    BOOL finished = FALSE;

    if (m_finished) {
        return TRUE;
    }

    if (m_sample->IsStreamed()) {
        if (m_stream == NULL) {
            return TRUE;
        }

        if (m_playing && !m_paused) {
            int status = _rwaStreamGetStatus(m_stream);
            if ((int)m_position != NULL) { // what? (this has to be a signed comparison)
                if (status == 6) {
                    m_position = NULL;
                }
                return FALSE;
            }

            finished = (status != 6 && status != 2 && status != 7);
        }
    } else {
        if (m_virtual_voice == NULL || !m_playing) {
            finished = TRUE;
        } else if (m_playing && !m_paused && m_loop_mode == 0) {
            finished = m_virtual_voice->object.unk3C == 0;
        }
    }

    if (finished) {
        m_playing = FALSE;
        m_finished = TRUE;
    }

    return finished;
}

CSound3D::~CSound3D() {
    DestroySound3D();
}

}
