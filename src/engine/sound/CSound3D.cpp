#include <dolphin/vi.h>
#include <rwa/objects/rwavirtualvoiceobj.h>
#include <rwa/rwastream.h>
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
    m_loop_mode = FALSE;
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

void CSound3D::SetLoopMode(BOOL loop_mode) {
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
        m_virtual_voice->state.Priority = 1.0f;
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
            _rwaVirtualVoiceSetTrigger(m_virtual_voice, FALSE);
            DkSoundGetEngine()->RWAFreeVirtualVoice(m_virtual_voice);
        }
        m_virtual_voice = NULL;
    }

    return TRUE;
}

BOOL CSound3D::GlobalUpdateSpecific() {
    CSample* sample = GetSample();
    if (sample == NULL) {
        return FALSE;
    }

    if (!sample->IsStreamed()) {
        if (m_virtual_voice == NULL) {
            return FALSE;
        }

        if (m_update_flags & SOUND3D_UPDATE_FLAG_VOLUME) {
            m_virtual_voice->state.Gain = m_volume;
            if (m_virtual_voice->state.RealVoice != NULL) {
                RwaObjHandleSetParamReal(m_virtual_voice->state.RealVoice, 5, 1, m_volume);
            }
        }

        if (m_update_flags & SOUND3D_UPDATE_FLAG_PITCH) {
            m_virtual_voice->state.Frequency = sample->GetFrequency() * m_pitch;
            if (m_virtual_voice->state.RealVoice != NULL) {
                RwaObjHandleSetParamReal(m_virtual_voice->state.RealVoice, 4, 1, sample->GetFrequency() * m_pitch);
            }
        }

        if (m_update_flags & SOUND3D_UPDATE_FLAG_PLAYING) {
            if (m_playing && !m_virtual_voice->state.Trigger) {
                m_finished = FALSE;
                _rwaVirtualVoiceSetTrigger(m_virtual_voice, TRUE);
            }
            if (!m_playing && m_virtual_voice->state.Trigger) {
                m_finished = TRUE;
                _rwaVirtualVoiceSetTrigger(m_virtual_voice, FALSE);
            }
        }

        if (m_update_flags & SOUND3D_UPDATE_FLAG_RESTART) {
            m_finished = FALSE;
            _rwaVirtualVoiceSetTrigger(m_virtual_voice, TRUE);
        }

        if (m_update_flags & SOUND3D_UPDATE_FLAG_PAUSED) {
            if (!m_paused) {
                m_finished = FALSE;
                m_virtual_voice->state.WavePosition = m_wave_position;
                if (m_virtual_voice->state.RealVoice != NULL) {
                    RwaObjHandleSetParamUInt32(m_virtual_voice->state.RealVoice, 7, 1, m_wave_position);
                }
                _rwaVirtualVoiceSetTrigger(m_virtual_voice, TRUE);
            }
            if (m_paused) {
                U32 position = (m_virtual_voice->state.RealVoice != NULL && m_virtual_voice->state.Trigger) ? RwaObjHandleGetParamUInt32(m_virtual_voice->state.RealVoice, 7, 2, NULL) : m_virtual_voice->state.WavePosition;
                m_virtual_voice->state.WavePosition = position;
                m_wave_position = position;
                _rwaVirtualVoiceSetTrigger(m_virtual_voice, FALSE);
            }
        }

        if (m_update_flags & SOUND3D_UPDATE_FLAG_LOOP_MODE) {
            if (m_playing) {
                U32 position = (m_virtual_voice->state.RealVoice != NULL && m_virtual_voice->state.Trigger) ? RwaObjHandleGetParamUInt32(m_virtual_voice->state.RealVoice, 7, 2, NULL) : m_virtual_voice->state.WavePosition;
                m_virtual_voice->state.WavePosition = position;
                m_wave_position = position;
                _rwaVirtualVoiceSetTrigger(m_virtual_voice, FALSE);
            }

            m_virtual_voice->state.Loop = m_loop_mode;
            if (m_virtual_voice->state.RealVoice != NULL) {
                RwaObjHandleSetParamInt32(m_virtual_voice->state.RealVoice, 8, 1, m_loop_mode);
            }

            if (m_playing && !m_paused) {
                m_virtual_voice->state.WavePosition = m_wave_position;
                if (m_virtual_voice->state.RealVoice != NULL) {
                    RwaObjHandleSetParamUInt32(m_virtual_voice->state.RealVoice, 7, 1, m_wave_position);
                }
                _rwaVirtualVoiceSetTrigger(m_virtual_voice, TRUE);
            }
        }
    } else {
        if (m_update_flags & SOUND3D_UPDATE_FLAG_VOLUME) {
            RwaStreamGain gain;
            gain.subStream = -1;
            gain.element.gain = m_volume;
            _rwaStreamSetStreamGain(m_stream, &gain);
        }

        if (m_update_flags & SOUND3D_UPDATE_FLAG_PITCH) {
            if (m_stream != NULL) {
                _rwaStreamSetPitch(m_stream, m_pitch * sample->GetFrequency());
            }
        }

        if (m_update_flags & SOUND3D_UPDATE_FLAG_PLAYING) {
            if (m_playing) {
                m_finished = FALSE;
                if (m_stream != NULL) {
                    _rwaStreamSetPlayState(m_stream, rwaSTREAMPLAYSTATE_PLAY);
                    DkSoundGetEngine()->BeginUpdate();
                    DkSoundGetEngine()->EndUpdate();
                    if ((int)m_sound_emitter != NULL) { // ??? (this has to be a signed comparison)
                        goto check;

                        while (TRUE) {
                            DkSoundGetEngine()->BeginUpdate();
                            DkSoundGetEngine()->EndUpdate();
                            VIWaitForRetrace();

check:
                            RwaStreamStatus status = _rwaStreamGetStatus(m_stream);
                            if (status == rwaSTREAMSTATUS_PLAYING || status == rwaSTREAMSTATUS_NOMEDIA) {
                                break;
                            }
                        }

                        m_position = NULL;
                    }
                }
            }
            if (!m_playing) {
                m_finished = TRUE;
                if (m_stream != NULL) {
                    _rwaStreamSetPlayState(m_stream, rwaSTREAMPLAYSTATE_STOP);

                    RwaStreamPosParams params;
                    RwaStreamPos pos;
                    params.flush = FALSE;
                    params.pos = &pos;
                    pos.segmentType = rwaSTREAMPOSSEGMENTTYPE_INDEX;
                    pos.segment.index = 0;
                    pos.timeType = rwaSTREAMPOSTIMETYPE_SAMPLES;
                    pos.time.samples = 0;
                    _rwaStreamSetStreamPos(m_stream, &params);

                    DkSoundGetEngine()->BeginUpdate();
                    DkSoundGetEngine()->EndUpdate();
                    VIWaitForRetrace();
                }
            }
        }

        if (m_update_flags & SOUND3D_UPDATE_FLAG_RESTART) {
            if (m_stream != NULL) {
                m_finished = FALSE;
                _rwaStreamSetPlayState(m_stream, rwaSTREAMPLAYSTATE_STOP);
                _rwaStreamSetPlayState(m_stream, rwaSTREAMPLAYSTATE_PLAY);
            }
        }

        if (m_update_flags & SOUND3D_UPDATE_FLAG_PAUSED) {
            if (!m_paused && m_stream != NULL) {
                _rwaStreamSetPlayState(m_stream, rwaSTREAMPLAYSTATE_PLAY);
            }
            if (m_paused && m_stream != NULL) {
                _rwaStreamSetPlayState(m_stream, rwaSTREAMPLAYSTATE_PAUSE);
            }
        }

        if (m_update_flags & SOUND3D_UPDATE_FLAG_LOOP_MODE) {
            if (m_stream != NULL) {
                _rwaStreamSetLoopSegment(m_stream, m_loop_mode);
            }
        }
    }

    m_update_flags = 0;
    return TRUE;
}

RwaVirtualVoice* CSound3D::RWAGetVirtualVoice() {
    return m_virtual_voice;
}

BOOL CSound3D::IsLooping() {
    BOOL looping = FALSE;
    if (m_virtual_voice == NULL) {
        return FALSE;
    }

    if (m_playing && !m_paused && m_loop_mode) {
        U32 position = (m_virtual_voice->state.RealVoice != NULL && m_virtual_voice->state.Trigger) ? RwaObjHandleGetParamUInt32(m_virtual_voice->state.RealVoice, 7, 2, NULL) : m_virtual_voice->state.WavePosition;
        m_virtual_voice->state.WavePosition = position;
        m_wave_position = position;

        looping = (m_wave_position < m_last_wave_position_maybe);
        m_last_wave_position_maybe = m_wave_position;
    }

    return looping;
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
            RwaStreamStatus status = _rwaStreamGetStatus(m_stream);
            if ((int)m_position != NULL) { // what? (this has to be a signed comparison)
                if (status == rwaSTREAMSTATUS_PLAYING) {
                    m_position = NULL;
                }
                return FALSE;
            }

            finished = (status != rwaSTREAMSTATUS_PLAYING && status != rwaSTREAMSTATUS_OPENING && status != rwaSTREAMSTATUS_STARVED);
        }
    } else {
        if (m_virtual_voice == NULL || !m_playing) {
            finished = TRUE;
        } else if (m_playing && !m_paused && !m_loop_mode) {
            finished = !m_virtual_voice->state.Trigger;
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
