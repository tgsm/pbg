#include "entities/CEntity3DSound.h"
#include <cstring>
#include <iostream>

CEntity3DSound::CEntity3DSound(CEntityManager* entity_manager, std::string name) : CEntityObject(entity_manager, name) {
    m_unk40 = "";
    m_radius = 0.0f;
    m_unk48 = 0.0f;
    m_unk70 = 1;
    m_unk4C = 1;
    m_unk60 = -1;

    m_attached_sounds.clear();

    m_sound_emitter = NULL;
}

// Equivalent?: std::vector dtor
CEntity3DSound::~CEntity3DSound() {
    if (m_sound_emitter != NULL) {
        m_entity_manager->GetGame()->m_sound_engine->RemoveEmitter(m_sound_emitter);
        m_sound_emitter = NULL;
    }
}

void CEntity3DSound::Parse(DkXmd::CChunkIterator iter) {
    char buf[256];
    char buf2[128];
    DkXmd::CChunkIterator dest;

    CEntityObject::Parse(iter);

    if (iter.GetChunk("SndDict", dest)) {
        strcpy(buf, dest.GetStringValue());
        m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(10, buf);
    }

    if (m_entity_manager->GetGame()->m_unk4F5C == 1 && iter.GetChunk("SndDictTigger", dest)) {
        strcpy(buf, dest.GetStringValue());
        m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(10, buf);
    }

    if (m_entity_manager->GetGame()->m_unk4F5C == 2 && iter.GetChunk("SndDictWinnie", dest)) {
        strcpy(buf, dest.GetStringValue());
        m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(10, buf);
    }

    if (m_entity_manager->GetGame()->m_unk4F5C == 3 && iter.GetChunk("SndDictCatch", dest)) {
        strcpy(buf, dest.GetStringValue());
        m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(10, buf);
    }

    if (iter.GetFirstChildChunk(dest)) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "AttachedSND") == 0) {
                ParseSound(dest);
            } else if (strcmp(buf, "Radius1") == 0) {
                m_radius = dest.GetFloatValue();
            } else if (strcmp(buf, "Radius2") == 0) {
                m_radius = dest.GetFloatValue();
            } else if (strcmp(buf, "Attenuate") == 0) {
                if (strcmp(dest.GetStringValue(), "LINEAR") == 0) {
                    m_unk4C = 1;
                } else if (strcmp(dest.GetStringValue(), "EXPONENTIAL") == 0) {
                    m_unk4C = 2;
                }
            } else if (strcmp(buf, "Loop") == 0) {
                strcpy(buf2, dest.GetStringValue());
                if (strcmp(buf2, "Off") == 0) {
                    m_unk70 = FALSE;
                } else {
                    m_unk70 = TRUE;
                }
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CEntity3DSound::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityObject::ParseBehavior(iter, behavior);
}

// Incomplete
// Passes a copy, but without passing a copy. What???
// https://decomp.me/scratch/I6uZu
void CEntity3DSound::Init() {
    m_sound_emitter->SetPosition(GetPosition());

    if (m_attached_sounds.size() != 0) {
        m_unk60 = m_attached_sounds[0].id;
    }
}

void CEntity3DSound::Render(F32 dt) {
    CEntityObject::Render(dt);
}

void CEntity3DSound::Reset() {
    CEntityObject::Reset();
}
