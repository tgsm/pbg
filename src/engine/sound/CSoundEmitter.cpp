#include "engine/sound/CSoundEmitter.h"
#include "engine/sound/CSound3D.h"
#include "engine/sound/DkSound.h"

namespace DKSND {

CSoundEmitter::CSoundEmitter() {
    Clear();
}

// Equivalent: std::vector dtor
CSoundEmitter::~CSoundEmitter() {
    Destroy();
}

void CSoundEmitter::Clear() {
    m_pPosition = NULL;
    m_num_sounds = 0;
    m_active = FALSE;
    m_unk38 = 0;
    m_position.x = 0.0f;
    m_position.y = 0.0f;
    m_position.z = 0.0f;
    m_pPosition = &m_position;

    m_unk14 = CDKW_V3d(-10000.0f, -10000.0f, -10000.0f);
}

void CSoundEmitter::Destroy() {
    RemoveAllSounds();
}

void CSoundEmitter::SetPosition(CDKW_V3d* position) {
    m_pPosition = position;
    Update();
}

void CSoundEmitter::SetPosition(CDKW_V3d position) {
    m_position = position;
    m_pPosition = &m_position;
    Update();
}

CDKW_V3d* CSoundEmitter::GetPosition() {
    return m_pPosition;
}

void CSoundEmitter::SetName(std::string* name) {
    m_name.assign(*name, 0);
}

std::string* CSoundEmitter::GetName() {
    return &m_name;
}

void CSoundEmitter::AddSound(ISound* sound) {
    size_t sound_as_size = reinterpret_cast<size_t>(sound);

    U32 i = 0;
    for (; i < m_sounds.size(); i++) {
        if (m_sounds[i] == NULL) {
            m_sounds[i] = sound_as_size;
            break;
        }
    }

    if (i == m_sounds.size()) {
        m_sounds.push_back(sound_as_size);
    }

    reinterpret_cast<CSound3D*>(sound_as_size)->SetPosition(m_pPosition);
    reinterpret_cast<CSound3D*>(sound_as_size)->SetSoundEmitter(this);
    m_num_sounds++;
}

void CSoundEmitter::RemoveSound(ISound* sound, int a2) {
    for (U32 i = 0; i < m_sounds.size(); i++) {
        if (reinterpret_cast<ISound*>(m_sounds[i]) == reinterpret_cast<ISound*>(sound)) {
            m_sounds[i] = NULL;
            ((CSound3D*)sound)->SetSoundEmitter(NULL);
            if (a2 != 0) {
                DkSoundGetEngine()->RemoveSound(sound);
            }
            m_num_sounds--;
            break;
        }
    }
}

// Equivalent
void CSoundEmitter::RemoveAllSounds() {
    std::vector<size_t> sounds_to_remove;

    size_t sound_as_size;
    for (U32 i = 0; i < m_sounds.size(); i++) {
        sound_as_size = m_sounds[i];

        if (sound_as_size != NULL) {
            sounds_to_remove.push_back(sound_as_size);
        }
    }

    for (U32 i = 0; i < sounds_to_remove.size(); i++) {
        sound_as_size = sounds_to_remove[i];
        RemoveSound(reinterpret_cast<ISound*>(sound_as_size), 1);
    }

    m_sounds.m_size = 0;
    if (m_sounds.m_data != NULL) {
        delete m_sounds.m_data;
    }
    m_sounds.m_data = NULL;
    m_sounds.m_capacity = 0;

    sounds_to_remove.m_size = 0;
    if (sounds_to_remove.m_data != NULL) {
        delete sounds_to_remove.m_data;
    }
    sounds_to_remove.m_data = NULL;
    sounds_to_remove.m_capacity = 0;

    m_num_sounds = 0;
}

void CSoundEmitter::Activate() {
    m_active = TRUE;
}

void CSoundEmitter::Deactivate() {
    m_active = FALSE;
}

U32 CSoundEmitter::GetNbSounds() {
    return m_num_sounds;
}

void CSoundEmitter::Update() {
    CDKW_V3d* position = m_pPosition;
    if (position->x != m_unk14.x || position->y != m_unk14.y || position->z != m_unk14.z) {
        for (U32 i = 0; i < m_sounds.size(); i++) {
            CSound3D* sound = (CSound3D*)m_sounds[i];
            if (sound != NULL) {
                sound->SetPosition(m_pPosition);
            }
        }
    }

    UpdateSpecific();

    m_unk14 = *m_pPosition;

    if (m_unk38 && GetNbSounds() == 0) {
        DkSoundGetEngine()->RemoveEmitter(this);
    }
}

CSound3D* CSoundEmitter::PlaySound(ISample* sample, int loop_mode) {
    CSound3D* sound = DkSoundGetEngine()->CreateSound3D(sample, 1);
    if (sound == NULL) {
        return NULL;
    }

    AddSound(sound);
    UpdateSpecific();

    sound->SetLoopMode(loop_mode);
    sound->Play(0, 0);

    return sound;
}

CSound3D* CSoundEmitter::PlaySound(std::string* name, int loop_mode) {
    CSound3D* sound = DkSoundGetEngine()->CreateSound3D(name, 1);
    if (sound == NULL) {
        return NULL;
    }

    AddSound(sound);

    sound->SetLoopMode(loop_mode);
    sound->Play(0, 0);

    return sound;
}

CSound3D* CSoundEmitter::PlaySound(std::string name, int loop_mode) {
    return PlaySound(&name, loop_mode);
}

void CSoundEmitter::SetDestroyWhenNoSound() {
    m_unk38 = TRUE;
}

}
