#include "engine/sound/CSound2D.h"
#include "engine/sound/DkSound.h"

namespace DKSND {

CSound2D::CSound2D(ISample* sample) {
    m_panning = 0.0f;
    DefaultConstructor(sample);

    if (sample == NULL) {
        return;
    }

    if (sample->Is3D()) {
        if (sample->m_unk0 > 0) {
            sample->m_unk0--;
        }
        return;
    }

    CreateSound2DSpecific();
}

CSound2D::CSound2D(std::string* str) {
    m_panning = 0.0f;
    DefaultConstructor(str);

    if (m_sample == NULL) {
        return;
    }

    if (m_sample->Is3D()) {
        if (m_sample->m_unk0 > 0) {
            m_sample->m_unk0--;
        }
        return;
    }

    CreateSound2DSpecific();
}

F32 CSound2D::GetPanning() {
    return m_panning;
}

void CSound2D::Clear() {
    ClearGeneric();
    m_panning = 0.0f;
}

void CSound2D::DestroySound2D() {
    Destroy();
}

void CSound2D::SetPanning(F32 panning) {
    if (panning > 1.0f) {
        m_panning = 1.0f;
    } else if (panning < -1.0f) {
        m_panning = -1.0f;
    } else {
        m_panning = panning;
    }

    UpdateSpecific();
}

void CSound2D::DefaultConstructor(std::string* str) {
    Clear();

    m_sample = DkSoundGetEngine()->GetSampleDictionary()->FindSample(str, 1);
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

void CSound2D::DefaultConstructor(ISample* sample) {
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

void CSound2D::ClearGeneric() {
    m_volume = 1.0f;
    m_pitch = 1.0f;
    m_sample = NULL;
    m_playing = FALSE;
    m_paused = FALSE;
    m_layer = 0;
    m_unk28 = 0;
    m_update_flags = 0;
    m_loop_mode = 0;
    m_unk20 = 0;
    m_unk30 = 0;
}

void CSound2D::Destroy() {
    if (m_sample != NULL && m_sample->m_unk0 > 0) {
        m_sample->m_unk0--;
    }

    ReleaseSpecific();
}

void CSound2D::PlayGeneric(U32 a1, U32 a2) {
    if (!m_playing) {
        m_playing = TRUE;
        m_update_flags |= (1 << 2);
    } else if (a1 == 1) {
        Restart();
    }

    if (m_paused && a2 != 0) {
        m_paused = FALSE;
        m_update_flags |= (1 << 4);
        if (a2 == 1) {
            Restart();
        }
    }

    UpdateSpecific();

    m_unk20 = 0;
}

void CSound2D::Stop() {
    if (m_playing) {
        m_playing = FALSE;
        m_update_flags |= (1 << 2);
    }

    if (m_paused) {
        m_paused = FALSE;
        m_update_flags |= (1 << 4);
    }
    UpdateSpecific();
}

void CSound2D::Resume() {
    if (m_paused) {
        m_paused = FALSE;
        m_update_flags |= (1 << 4);
        UpdateSpecific();
    }
}

void CSound2D::Pause() {
    if (!m_paused) {
        m_paused = TRUE;
        m_update_flags |= (1 << 4);
        UpdateSpecific();
    }
}

void CSound2D::SetLayer(U32 layer) {
    m_layer = layer;
    DkSoundGetEngine()->AddLayer(layer);
}

void CSound2D::SetVolume(F32 volume) {
    m_volume = volume;
    m_update_flags |= (1 << 0);
    UpdateSpecific();
}

void CSound2D::SetPitch(F32 pitch) {
    m_pitch = pitch;
    m_update_flags |= (1 << 1);
    UpdateSpecific();
}

void CSound2D::SetLoopMode(int loop_mode) {
    m_loop_mode = loop_mode;
    m_update_flags |= (1 << 6);
    UpdateSpecific();
}

U32 CSound2D::GetLayer() {
    return m_layer;
}

F32 CSound2D::GetVolume() {
    return m_volume;
}

F32 CSound2D::GetPitch() {
    return m_pitch;
}

CSample* CSound2D::GetSample() {
    return m_sample;
}

int CSound2D::GetLoopMode() {
    return m_loop_mode;
}

void CSound2D::Restart() {
    m_unk20 = 1;
    m_update_flags |= (1 << 3);
}

BOOL CSound2D::IsPlaying() {
    return m_playing;
}

}
