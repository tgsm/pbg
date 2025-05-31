#include "engine/xmd/CXmdFile.h"
#include "entities/CEntityManager.h"
#include "entities/CEntity.h"
#include "entities/CEntity3DSound.h"
#include "entities/CEntityCookie.h"
#include "entities/CEntityEscapingObject.h"
#include "entities/CEntityFX.h"
#include "entities/CEntityHero.h"
#include "entities/CEntityLight.h"
#include "entities/CEntityMesh.h"
#include "entities/CEntityNPCArrow.h"
#include "entities/CEntityNPCBasicEffalump.h"
#include "entities/CEntityNPCBasicWoozle.h"
#include "entities/CEntityNPCBeeEffalump.h"
#include "entities/CEntityNPCDoor.h"
#include "entities/CEntityNPCGralosaurus.h"
#include "entities/CEntityNPCHnSWoozle.h"
#include "entities/CEntityNPCJackpotEffalump.h"
#include "entities/CEntityNPCMirrorWoozle.h"
#include "entities/CEntityNPCRoadSweeperEffalump.h"
#include "entities/CEntityNPCTennisWoozle.h"
#include "entities/CEntityNPCTree.h"
#include "entities/CEntityNPCTubaEffalump.h"
#include "entities/CEntityPathFinder.h"
#include "entities/CEntityPhial.h"
#include "entities/CEntityPiglet.h"
#include "entities/CEntityPushingBox.h"
#include "entities/CEntityScaryFaceBox.h"
#include "entities/CEntitySearchableZone.h"
#include "entities/CEntitySeqKey.h"
#include "entities/CEntityTigger.h"
#include "entities/CEntityTimer.h"
#include "entities/CEntityWarp.h"
#include "entities/CEntityWinnie.h"
#include "entities/CEntityWorld.h"
#include "entities/CEntityZone.h"
#include "entities/CRTCPlayerEntity.h"
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
    m_model_xmd_data = NULL;
    m_model_xmd = NULL;
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
        return ENTITY_WORLD;
    } else if (type_str == "WarpEntity") {
        return ENTITY_WARP;
    } else if (type_str == "MeshEntity") {
        return ENTITY_MESH;
    } else if (type_str == "LightEntity") {
        return ENTITY_LIGHT;
    } else if (type_str == "RTCPlayerEntity") {
        return ENTITY_RTC_PLAYER;
    } else if (type_str == "FXEntity") {
        return ENTITY_FX;
    } else if (type_str == "ZoneEntity") {
        return ENTITY_ZONE;
    } else if (type_str == "SearchableZoneEntity") {
        return ENTITY_SEARCHABLE_ZONE;
    } else if (type_str == "ScaryFaceBoxEntity") {
        return ENTITY_SCARY_FACE_BOX;
    } else if (type_str == "PigletEntity") {
        return ENTITY_PIGLET;
    } else if (type_str == "WinnieEntity") {
        return ENTITY_WINNIE;
    } else if (type_str == "TiggerEntity") {
        return ENTITY_TIGGER;
    } else if (type_str == "NPCEntity") {
        return ENTITY_NPC21;
    } else if (type_str == "BasicEffalumEntity") {
        return ENTITY_NPC_BASIC_EFFALUMP;
    } else if (type_str == "BasicWoozleEntity") {
        return ENTITY_NPC_BASIC_WOOZLE;
    } else if (type_str == "BasicTreeEntity") {
        return ENTITY_NPC_TREE;
    } else if (type_str == "BeeEffalumEntity") {
        return ENTITY_NPC_BEE_EFFALUMP;
    } else if (type_str == "HideAndSeekWoozleEntity") {
        return ENTITY_NPC_HNS_WOOZLE;
    } else if (type_str == "MirrorWoozleEntity") {
        return ENTITY_NPC_MIRROR_WOOZLE;
    } else if (type_str == "RoadSweeperEffalumEntity") {
        return ENTITY_NPC_ROAD_SWEEPER_EFFALUMP;
    } else if (type_str == "TubaEffalumEntity") {
        return ENTITY_NPC_TUBA_EFFALUMP;
    } else if (type_str == "JackpotEffalumEntity") {
        return ENTITY_NPC_JACKPOT_EFFALUMP;
    } else if (type_str == "TennisWoozleEntity") {
        return ENTITY_NPC_TENNIS_WOOZLE;
    } else if (type_str == "DoorEntity") {
        return ENTITY_NPC_DOOR;
    } else if (type_str == "GralosaurusEntity") {
        return ENTITY_NPC_GRALOSAURUS;
    } else if (type_str == "PathFinder") {
        return ENTITY_PATH_FINDER;
    } else if (type_str == "PushingBoxEntity") {
        return ENTITY_PUSHING_BOX;
    } else if (type_str == "TimerEntity") {
        return ENTITY_TIMER;
    } else if (type_str == "CookieEntity") {
        return ENTITY_COOKIE;
    } else if (type_str == "EscapingObjectEntity") {
        return ENTITY_ESCAPING_OBJECT;
    } else if (type_str == "NPCArrowEntity") {
        return ENTITY_NPC_ARROW;
    } else if (type_str == "KeySequenceEntity") {
        return ENTITY_SEQ_KEY;
    } else if (type_str == "3DSoundEntity") {
        return ENTITY_3D_SOUND;
    } else {
        return ENTITY_INVALID;
    }
}

BOOL CEntityManager::CreateEntity(std::string name, std::string type, std::string model) {
    u32 type_id = GetTypeFromString(type);
    return CreateEntity(name, type_id, model);
}

BOOL CEntityManager::CreateEntity(std::string name, u32 type, std::string model) {
    CEntity* entity = NULL;

    BOOL exists = GetEntity(name) != NULL;
    if (exists) {
        return TRUE;
    }

    u32 type_ = GetTypeFromModel(model);
    if (type_ == 0) {
        type_ = type;
    }

    switch (type_) {
        case ENTITY_WORLD:
            entity = new CEntityWorld(this, name);
            break;
        case ENTITY_WARP:
            entity = new CEntityWarp(this, name);
            break;
        case ENTITY_LIGHT:
            entity = new CEntityLight(this, name);
            break;
        case ENTITY_MESH:
            entity = new CEntityMesh(this, name);
            break;
        case ENTITY_TIMER:
            entity = new CEntityTimer(this, name);
            break;
        case ENTITY_FX:
            entity = new CEntityFX(this, name);
            break;
        case ENTITY_ZONE:
            entity = new CEntityZone(this, name);
            break;
        case ENTITY_SEARCHABLE_ZONE:
            entity = new CEntitySearchableZone(this, name);
            break;
        case ENTITY_SCARY_FACE_BOX:
            entity = new CEntityScaryFaceBox(this, name);
            break;
        case ENTITY_PIGLET:
            entity = new CEntityPiglet(this, name);
            break;
        case ENTITY_WINNIE:
            entity = new CEntityWinnie(this, name);
            break;
        case ENTITY_TIGGER:
            entity = new CEntityTigger(this, name);
            break;
        case ENTITY_NPC_BASIC_EFFALUMP:
            entity = new CEntityNPCBasicEffalump(this, name);
            break;
        case ENTITY_NPC_BASIC_WOOZLE:
            entity = new CEntityNPCBasicWoozle(this, name);
            break;
        case ENTITY_NPC_TREE:
            entity = new CEntityNPCTree(this, name);
            break;
        case ENTITY_NPC_BEE_EFFALUMP:
            entity = new CEntityNPCBeeEffalump(this, name);
            break;
        case ENTITY_NPC_HNS_WOOZLE:
            entity = new CEntityNPCHnSWoozle(this, name);
            break;
        case ENTITY_NPC_MIRROR_WOOZLE:
            entity = new CEntityNPCMirrorWoozle(this, name);
            break;
        case ENTITY_NPC_ROAD_SWEEPER_EFFALUMP:
            entity = new CEntityNPCRoadSweeperEffalump(this, name);
            break;
        case ENTITY_NPC_TUBA_EFFALUMP:
            entity = new CEntityNPCTubaEffalump(this, name);
            break;
        case ENTITY_NPC_JACKPOT_EFFALUMP:
            entity = new CEntityNPCJackpotEffalump(this, name);
            break;
        case ENTITY_NPC_TENNIS_WOOZLE:
            entity = new CEntityNPCTennisWoozle(this, name);
            break;
        case ENTITY_NPC_DOOR:
            entity = new CEntityNPCDoor(this, name);
            break;
        case ENTITY_NPC_GRALOSAURUS:
            entity = new CEntityNPCGralosaurus(this, name);
            break;
        case ENTITY_NPC_ARROW:
            entity = new CEntityNPCArrow(this, name);
            break;
        case ENTITY_SEQ_KEY:
            entity = new CEntitySeqKey(this, name);
            break;
        case ENTITY_PHIAL:
            entity = new CEntityPhial(this, name);
            break;
        case ENTITY_PATH_FINDER:
            entity = new CEntityPathFinder(this, name);
            break;
        case ENTITY_3D_SOUND:
            entity = new CEntity3DSound(this, name);
            break;
        case ENTITY_PUSHING_BOX:
            entity = new CEntityPushingBox(this, name);
            break;
        case ENTITY_COOKIE:
            entity = new CEntityCookie(this, name);
            break;
        case ENTITY_ESCAPING_OBJECT:
            entity = new CEntityEscapingObject(this, name);
            break;
        case ENTITY_RTC_PLAYER:
            entity = new CRTCPlayerEntity(this, name);
            break;
        default:
            return FALSE;
    }

    DkXmd::CChunkIterator iter;
    if (FindChunkModel(model, iter) == 1) {
        entity->Parse(iter);
    }

    m_entities.push_back(reinterpret_cast<size_t>(entity));
    m_entity_light_01 = GetEntity("light01");

    return TRUE;
}

// Equivalent: std::vector erase() scheduling
BOOL CEntityManager::DestroyEntity(u32 index) {
    if (m_entities[index] != 0) {
        delete reinterpret_cast<CEntity*>(m_entities[index]);
    }
    m_entities.erase(m_entities.data() + index);

    m_entity_light_01 = GetEntity("light01");

    return TRUE;
}

BOOL CEntityManager::SetModelFile(std::string model) {
    m_model_xmd_data = m_game->m_resource_factory->LoadPureFile(model, NULL);

    m_model_xmd = new DkXmd::CXmdFile;
    if (!m_model_xmd->Parse(m_model_xmd_data)) {
        return FALSE;
    }

    m_model_xmd_iter = &m_model_xmd->m_chunk_iterator;

    return TRUE;
}

// Equivalent: regalloc. Also see FIXME.
BOOL CEntityManager::FindChunkModel(std::string str, DkXmd::CChunkIterator& iter) {
    DkXmd::CChunkIterator dest2;
    DkXmd::CChunkIterator dest;
    std::string temp;

    if (m_model_xmd_iter->GetFirstChildChunk(dest) == TRUE) {
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

CEntity* CEntityManager::GetEntityLight01() {
    return m_entity_light_01;
}

CEntity* CEntityManager::GetEntity(u32 n) {
    return reinterpret_cast<CEntity*>(m_entities[n]);
}

u32 CEntityManager::GetEntityCount() {
    return m_entities.size();
}

CEntityHero* CEntityManager::GetHero() {
    CEntityHero* entity = static_cast<CEntityHero*>(GetEntity("Piglet"));
    if (entity == 0) {
        entity = static_cast<CEntityHero*>(GetEntity("Tigger"));
        if (entity == 0) {
            entity = static_cast<CEntityHero*>(GetEntity("Winnie"));
        }
    }
    return entity;
}
