#include "entities/CEntityManager.h"
#include "entities/CEntity.h"
#include <cstring>
#include <iostream>

f32 InterpolValue(f32 f1, f32 f2, f32 f3, f32 f4) {
    if (f3 >= f4) {
        return f2;
    }

    if (f3 <= 0.0f) {
        return f1;
    }

    f32 unk = f2;
    unk -= f1;
    unk /= f4;
    unk *= f3;
    return f1 + unk;
}

CEntityManager::CEntityManager(CGame* game) {
    m_game = game;
    m_unk10 = 0;
    m_unk14 = 0;
    m_entity_light_01 = 0;

    // Did they really do this?
    // FIXME: This is probably part of some std::vector function
    m_entities.m_size = 0;
    if (m_entities.data() != NULL) {
        delete m_entities.data();
    }
    m_entities.m_data = NULL;
    m_entities.m_capacity = 0;

    m_entities.reserve(256);

    m_unk1C = 0;
    m_unk20 = 0;
}

// Equivalent
u32 CEntityManager::GetTypeFromModel(std::string model_name) {
    std::string temp;
    DkXmd::CChunkIterator model_chunk;
    DkXmd::CChunkIterator dest;

    // FIXME: This gets downcasted to 8bit.
    if (FindChunkModel(model_name, model_chunk) == TRUE) {
        if (model_chunk.GetFirstChildChunk(dest) == TRUE) {
            do {
                temp = dest.GetName();
                if (temp == "ModelType") {
                    temp = dest.GetStringValue();
                    return GetTypeFromString(temp);
                }
            } while (dest.GetNextSiblingChunk(dest) == TRUE);
        }
    }

    return 0;
}

u32 CEntityManager::GetTypeFromString(std::string type_str) {
    if (type_str == "WorldEntity") {
        return 3;
    } else if (type_str == "WarpEntity") {
        return 4;
    } else if (type_str == "MeshEntity") {
        return 14;
    } else if (type_str == "LightEntity") {
        return 10;
    } else if (type_str == "RTCPlayerEntity") {
        return 12;
    } else if (type_str == "FXEntity") {
        return 13;
    } else if (type_str == "ZoneEntity") {
        return 6;
    } else if (type_str == "SearchableZoneEntity") {
        return 7;
    } else if (type_str == "ScaryFaceBoxEntity") {
        return 8;
    } else if (type_str == "PigletEntity") {
        return 18;
    } else if (type_str == "WinnieEntity") {
        return 19;
    } else if (type_str == "TiggerEntity") {
        return 20;
    } else if (type_str == "NPCEntity") {
        return 21;
    } else if (type_str == "BasicEffalumEntity") {
        return 24;
    } else if (type_str == "BasicWoozleEntity") {
        return 25;
    } else if (type_str == "BasicTreeEntity") {
        return 26;
    } else if (type_str == "BeeEffalumEntity") {
        return 27;
    } else if (type_str == "HideAndSeekWoozleEntity") {
        return 28;
    } else if (type_str == "MirrorWoozleEntity") {
        return 29;
    } else if (type_str == "RoadSweeperEffalumEntity") {
        return 30;
    } else if (type_str == "TubaEffalumEntity") {
        return 31;
    } else if (type_str == "JackpotEffalumEntity") {
        return 32;
    } else if (type_str == "TennisWoozleEntity") {
        return 33;
    } else if (type_str == "DoorEntity") {
        return 34;
    } else if (type_str == "GralosaurusEntity") {
        return 35;
    } else if (type_str == "PathFinder") {
        return 45;
    } else if (type_str == "PushingBoxEntity") {
        return 15;
    } else if (type_str == "TimerEntity") {
        return 11;
    } else if (type_str == "CookieEntity") {
        return 16;
    } else if (type_str == "EscapingObjectEntity") {
        return 39;
    } else if (type_str == "NPCArrowEntity") {
        return 42;
    } else if (type_str == "KeySequenceEntity") {
        return 43;
    } else if (type_str == "3DSoundEntity") {
        return 46;
    } else {
        return 0;
    }
}

BOOL CEntityManager::CreateEntity(std::string name, std::string type, std::string model) {
    u32 type_id = GetTypeFromString(type);
    return CreateEntity(name, type_id, model);
}

// Equivalent: regalloc. Also see FIXME.
BOOL CEntityManager::FindChunkModel(std::string str, DkXmd::CChunkIterator& iter) {
    DkXmd::CChunkIterator dest2;
    DkXmd::CChunkIterator dest;
    std::string temp;

    if (m_unk18->GetFirstChildChunk(dest) == TRUE) {
        do {
            temp = dest.GetName();
            if (temp == "EntityModel") {
                if (dest.GetFirstChildChunk(dest2) == TRUE) {
                    do {
                        temp = dest2.GetName();
                        if (temp == "ModelName") {
                            temp = dest2.GetStringValue();
                            // FIXME: This check gets downcased to 8bit. May be something inside std::basic_string.
                            if (temp == str) {
                                iter = dest;
                                return TRUE;
                            }
                        }
                    } while (dest2.GetNextSiblingChunk(dest2) == TRUE);
                }
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }

    return FALSE;
}

BOOL CEntityManager::Parse(DkXmd::CChunkIterator iter) {
    BOOL ret = FALSE;
    DkXmd::CChunkIterator dest;

    char buf[128];
    char type[128];
    char name[128];
    char model[128];
    strcpy(buf, "");
    strcpy(type, "");
    strcpy(name, "");
    strcpy(model, "");

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "Type") == 0) {
                strcpy(type, dest.GetStringValue());
            } else if (strcmp(buf, "Name") == 0) {
                strcpy(name, dest.GetStringValue());
            } else if (strcmp(buf, "Model") == 0) {
                strcpy(model, dest.GetStringValue());
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);

        ret = CreateEntity(name, type, model);

        CEntity* entity = GetEntity(name);
        if (entity != NULL) {
            entity->Parse(iter);
            entity->SetBehavior(entity->m_unk10);
            entity->Init();
        }
    }

    return ret;
}

u32 CEntityManager::GetEntityLight01() {
    return m_entity_light_01;
}

CEntity* CEntityManager::GetEntity(u32 n) {
    return reinterpret_cast<CEntity*>(m_entities[n]);
}

u32 CEntityManager::GetEntityCount() {
    return m_entities.size();
}

CEntity* CEntityManager::GetHero() {
    CEntity* entity = GetEntity("Piglet");
    if (entity == 0) {
        entity = GetEntity("Tigger");
        if (entity == 0) {
            entity = GetEntity("Winnie");
        }
    }
    return entity;
}
