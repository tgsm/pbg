#ifndef PIGLET_ENTITIES_CENTITY3DSOUND_H
#define PIGLET_ENTITIES_CENTITY3DSOUND_H

#include <string>
#include <vector>
#include "engine/sound/CSampleDictionary.h"
#include "engine/sound/CSoundEmitter.h"
#include "entities/CEntityObject.h"
#include "CGame.h"

struct SAttachedSound {
    int id;
    DKSND::CSample* sample;
    int unk8;
    F32 pitch_variation;
    F32 volume;
};

class CEntity3DSound : public CEntityObject {
public:
    std::string m_unk40;
    F32 m_radius;
    F32 m_unk48;
    U32 m_unk4C;
    std::vector<SAttachedSound> m_attached_sounds;
    DKSND::CSoundEmitter* m_sound_emitter;
    int m_unk60;
    CDKW_V3d m_position;
    U32 m_unk70;

public:
    CEntity3DSound(CEntityManager* entity_manager, std::string name);
    ~CEntity3DSound();

    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void Init();
    virtual U32 GetType() { return ENTITY_3D_SOUND; }
    virtual CDKW_V3d GetPosition() { return m_position; }
    virtual void SetPosition(CDKW_V3d& position) { m_position = position; }
    virtual void Render(F32 dt);
    virtual void Reset();
    virtual void DelFlag(U32 flag);
    virtual void AddFlag(U32 flag);

    void ParseSound(DkXmd::CChunkIterator iter) {
        SAttachedSound sound;
        sound.sample = NULL;
        sound.unk8 = 0;
        sound.id = -1;
        sound.pitch_variation = 0.0f;
        sound.volume = 1.0f;

        DkXmd::CChunkIterator dest;

        if (m_sound_emitter == NULL) {
            m_sound_emitter = m_entity_manager->GetGame()->m_sound_engine->CreateEmitter();
            if (m_sound_emitter != NULL) {
                m_sound_emitter->SetName(&m_unk0);
            }
        }

        if (iter.GetChunk("Id", dest)) {
            sound.id = dest.GetS32Value();
        }

        if (iter.GetChunk("SoundId", dest)) {
            std::string str;
            str = dest.GetStringValue();
            sound.sample = m_entity_manager->GetGame()->m_sample_dictionary->FindSample(&str, 1);
        }

        if (iter.GetChunk("PitchVariation", dest)) {
            sound.pitch_variation = dest.GetFloatValue();
        }

        if (iter.GetChunk("Volume", dest)) {
            sound.volume = dest.GetFloatValue();
        }

        m_attached_sounds.push_back(sound);
    }
};
REQUIRE_SIZE(CEntity3DSound, 0x74);

#endif
