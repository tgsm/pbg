#include "CGamePartIngame.h"
#include "CGame.h"
#include "entities/CEntity.h"
#include "entities/CEntityNPC.h"
#include "entities/CEntityNPCDoor.h"
#include "entities/CEntityPiglet.h"
#include "entities/CEntitySearchableZone.h"
#include "entities/CEntityTigger.h"
#include "entities/CEntityWinnie.h"
#include <cstdio>
#include <cstring>
#include <iostream>

// Incomplete
CGamePartIngame::CGamePartIngame(CGame* game) {

}

// Incomplete
CGamePartIngame::~CGamePartIngame() {

}

// Inlined and deadstripped, unofficial name
BOOL CGamePartIngame::LoadCollisionBSP(DkXmd::CChunkIterator iter) {
    char path[256];
    strcpy(path, iter.GetStringValue());

    U32 size;
    void* bsp_data = m_game->GetResourceFactory()->LoadPureFile(path, &size);
    if (bsp_data != NULL) {
        CDKW_Memory memory;
        memory.start = (RwUInt8*)bsp_data;
        memory.length = size;
        DKDSP::CRWStream* stream = m_game->GetDisplayEngine()->OpenStreamMemory(rwSTREAMREAD, &memory);
        if (stream != NULL) {
            if (stream->FindChunk(11, 0, 0)) {
                m_game->GetScene()->LoadCollisionBSP(stream);
                m_game->GetDisplayEngine()->DestroyStream(stream);
                delete bsp_data;
            }
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }

    return TRUE;
}

// Inlined and deadstripped, unofficial name
BOOL CGamePartIngame::LoadCameraSystem(DkXmd::CChunkIterator iter) {
    char path[256];
    m_camera = new CCACamera;

    strcpy(path, iter.GetStringValue());
    void* xmd_data = m_game->GetResourceFactory()->LoadPureFile(path, NULL);
    DkXmd::CXmdFile* xmd = new DkXmd::CXmdFile;
    if (!xmd->Parse(xmd_data)) {
        return FALSE;
    }

    m_camera->Load(&xmd->m_chunk_iterator);
    delete xmd;
    delete xmd_data;

    m_game->GetEntityManager()->GetPathFinder()->Init(m_game->GetScene()->GetCollisionWorld(), 100.0f, NULL);

    return TRUE;
}

// Inlined and deadstripped, unofficial name
BOOL CGamePartIngame::LoadSplines(DkXmd::CChunkIterator iter) {
    return m_pig_spline_bank->Parse(iter);
}

// Inlined and deadstripped, unofficial name
U32 CGamePartIngame::UnknownRoomCheck() {
    return (m_game->GetCurrentRoomFlagsMaybe() & (1 << 3)) ? TRUE : FALSE;
}

BOOL CGamePartIngame::Load() {
    U32 mission_id = m_game->GetCurrentMissionId();
    U32 room_id = m_game->GetCurrentRoomId();

    char room_path[256];
    sprintf(room_path, "Missions/");
    char temp[128];
    sprintf(temp, "Mission%d/Room%02d", mission_id, room_id);
    strcat(room_path, temp);
    sprintf(temp, "/Room%d%02d.XMD", mission_id, room_id);
    char xmd_path[256];
    strcpy(xmd_path, room_path);
    strcat(xmd_path, temp);

    void* xmd_data = m_game->GetResourceFactory()->LoadPureFile(xmd_path, NULL);
    DkXmd::CXmdFile* xmd = new DkXmd::CXmdFile;
    if (!xmd->Parse(xmd_data)) {
        return FALSE;
    }

    m_game->GetCamera()->SetZFar(300.0f);
    m_game->GetCamera()->SetZNear(0.6f);

    DkXmd::CChunkIterator iter = xmd->m_chunk_iterator;
    DkXmd::CChunkIterator dest;
    if (iter.GetFirstChildChunk(dest)) {
        do {
            char buf[128];
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "TexDict") == 0) {
                char* path = dest.GetStringValue();
                if (path == NULL) {
                    path = "Error - No File Name Given";
                }

                if (m_game->GetCurrentMissionId() == MISSION_EEYORE &&
                    m_game->GetCurrentRoomId() != 1 && m_game->GetCurrentRoomId() != 2 && m_game->GetCurrentRoomId() != 11 &&
                    !UnknownRoomCheck() &&
                    m_game->GetCurrentMission()->m_unk2C == 0) {
                    // Append "_NB" to the filename
                    char nb_path[256];
                    int path_len = strlen(path);
                    strcpy(nb_path, path);
                    strcpy(nb_path + path_len - 4, "_NB.TXD");

                    m_game->GetResourceFactory()->LoadResource(RESOURCE_TYPE_TEXTURE_DICTIONARY, nb_path);
                } else {
                    m_game->GetResourceFactory()->LoadResource(RESOURCE_TYPE_TEXTURE_DICTIONARY, path);
                }
            } else if (strcmp(buf, "SndDict") == 0) {
                char* path = dest.GetStringValue();
                if (path == NULL) {
                    path = "Error - No File Name Given";
                }
                m_game->GetResourceFactory()->LoadResource(RESOURCE_TYPE_SAMPLE_BANK1, path);
            } else if (strcmp(buf, "SndDictTigger") == 0 && m_game->GetCurrentHeroId() == HERO_TIGGER) {
                char* path = dest.GetStringValue();
                if (path == NULL) {
                    path = "Error - No File Name Given";
                }
                m_game->GetResourceFactory()->LoadResource(RESOURCE_TYPE_SAMPLE_BANK1, path);
            } else if (strcmp(buf, "SndDictWinnie") == 0 && m_game->GetCurrentHeroId() == HERO_WINNIE) {
                char* path = dest.GetStringValue();
                if (path == NULL) {
                    path = "Error - No File Name Given";
                }
                m_game->GetResourceFactory()->LoadResource(RESOURCE_TYPE_SAMPLE_BANK1, path);
            } else if (strcmp(buf, "SndDictCatch") == 0 && m_game->GetCurrentHeroId() == HERO_CATCH_THEM_ALL) {
                char* path = dest.GetStringValue();
                if (path == NULL) {
                    path = "Error - No File Name Given";
                }
                m_game->GetResourceFactory()->LoadResource(RESOURCE_TYPE_SAMPLE_BANK1, path);
            } else if (strcmp(buf, "StreamDict") == 0) {
                char* path = dest.GetStringValue();
                if (path == NULL) {
                    path = "Error - No File Name Given";
                }
                m_game->GetResourceFactory()->LoadResource(RESOURCE_TYPE_SAMPLE_BANK2, path);
            }
        } while (dest.GetNextSiblingChunk(dest));
    }

    if (iter.GetChunk("CollisionBsp", dest)) {
        if (!LoadCollisionBSP(dest)) {
            return FALSE;
        }
    }

    if (iter.GetFirstChildChunk(dest)) {
        do {
            char buf[128];
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "Mesh") == 0) {
                m_game->GetResourceFactory()->LoadResource(RESOURCE_TYPE_CLUMP, dest.GetStringValue());
            } else if (strcmp(buf, "ANM") == 0) {
                m_game->GetResourceFactory()->LoadResource(RESOURCE_TYPE_ANIMATION, dest.GetStringValue());
            } else if (strcmp(buf, "DMA") == 0) {
                m_game->GetResourceFactory()->LoadResource(RESOURCE_TYPE_DMORPH_ANIMATION, dest.GetStringValue());
            } else if (strcmp(buf, "TAN") == 0) {
                m_game->GetResourceFactory()->LoadResource(RESOURCE_TYPE_MATERIAL_ANIMATION, dest.GetStringValue());
            } else if (strcmp(buf, "ParticleEmitterFile") == 0) {
                char* path = dest.GetStringValue();
                if (path != NULL) {
                    m_game->GetResourceFactory()->LoadResource(RESOURCE_TYPE_PARTICLE_EMITTER_DEFINITION, dest.GetStringValue());
                }
            } else if (strcmp(buf, "Entity") == 0) {
                m_game->GetEntityManager()->Parse(dest);
            } else if (strcmp(buf, "CameraSystem") == 0) {
                if (!LoadCameraSystem(dest)) {
                    return FALSE;
                }
            } else if (strcmp(buf, "Splines") == 0) {
                if (!LoadSplines(dest)) {
                    return FALSE;
                }
            } else if (strcmp(buf, "ZFAR") == 0) {
                m_game->GetCamera()->SetZFar(dest.GetFloatValue());
            } else if (strcmp(buf, "ZNEAR") == 0) {
                m_game->GetCamera()->SetZNear(dest.GetFloatValue());
            } else if (strcmp(buf, "AmbientColor") == 0) {
                F32 r, g, b;
                r = g = b = 0.0f;
                DkXmd::CChunkIterator color_dest;
                if (dest.GetChunk("R", color_dest)) {
                    r = color_dest.GetFloatValue();
                }
                if (dest.GetChunk("G", color_dest)) {
                    g = color_dest.GetFloatValue();
                }
                if (dest.GetChunk("B", color_dest)) {
                    b = color_dest.GetFloatValue();
                }
                m_game->GetScene()->SetAmbient(r, g, b);
            } else if (strcmp(buf, "PigletPosition") == 0 && m_game->m_unk5090 == 1) {
                F32 x, y, z;
                x = y = z = 0.0f;
                DkXmd::CChunkIterator pos_dest;
                if (dest.GetChunk("X", pos_dest)) {
                    x = pos_dest.GetFloatValue();
                }
                if (dest.GetChunk("Y", pos_dest)) {
                    y = pos_dest.GetFloatValue();
                }
                if (dest.GetChunk("Z", pos_dest)) {
                    z = pos_dest.GetFloatValue();
                }

                CEntityHero* hero;
                if (hero = (CEntityHero*)m_game->GetEntityManager()->GetEntity("Piglet"), hero != NULL) {
                    CDKW_V3d pos(x, y, z);
                    ((CEntityPiglet*)m_game->GetEntityManager()->GetEntity("Piglet"))->SetPosition(pos);
                    ((CEntityPiglet*)m_game->GetEntityManager()->GetEntity("Piglet"))->Reset();
                } else if (hero = (CEntityHero*)m_game->GetEntityManager()->GetEntity("Tigger"), hero != NULL) {
                    CDKW_V3d pos(x, y, z);
                    ((CEntityTigger*)m_game->GetEntityManager()->GetEntity("Tigger"))->SetPosition(pos);
                    ((CEntityTigger*)m_game->GetEntityManager()->GetEntity("Tigger"))->Reset();
                } else if (hero = (CEntityHero*)m_game->GetEntityManager()->GetEntity("Winnie"), hero != NULL) {
                    CDKW_V3d pos(x, y, z);
                    ((CEntityWinnie*)m_game->GetEntityManager()->GetEntity("Winnie"))->SetPosition(pos);
                    ((CEntityWinnie*)m_game->GetEntityManager()->GetEntity("Winnie"))->Reset();
                }
            } else if (strcmp(buf, "PigletRotation") == 0 && m_game->m_unk5090 == 1) {
                F32 x, y, z;
                x = y = z = 0.0f;
                DkXmd::CChunkIterator rot_dest;
                if (dest.GetChunk("X", rot_dest)) {
                    x = rot_dest.GetFloatValue();
                }
                if (dest.GetChunk("Y", rot_dest)) {
                    y = rot_dest.GetFloatValue();
                }
                if (dest.GetChunk("Z", rot_dest)) {
                    z = rot_dest.GetFloatValue();
                }

                CEntityHero* hero;
                if (hero = (CEntityHero*)m_game->GetEntityManager()->GetEntity("Piglet"), hero != NULL) {
                    CDKW_V3d rot(x, y, z);
                    ((CEntityPiglet*)m_game->GetEntityManager()->GetEntity("Piglet"))->SetOrientation(rot);
                    ((CEntityPiglet*)m_game->GetEntityManager()->GetEntity("Piglet"))->Reset();
                } else if (hero = (CEntityHero*)m_game->GetEntityManager()->GetEntity("Tigger"), hero != NULL) {
                    CDKW_V3d rot(x, y, z);
                    ((CEntityTigger*)m_game->GetEntityManager()->GetEntity("Tigger"))->SetOrientation(rot);
                    ((CEntityTigger*)m_game->GetEntityManager()->GetEntity("Tigger"))->Reset();
                } else if (hero = (CEntityHero*)m_game->GetEntityManager()->GetEntity("Winnie"), hero != NULL) {
                    CDKW_V3d rot(x, y, z);
                    ((CEntityWinnie*)m_game->GetEntityManager()->GetEntity("Winnie"))->SetOrientation(rot);
                    ((CEntityWinnie*)m_game->GetEntityManager()->GetEntity("Winnie"))->Reset();
                }
            } else if (strcmp(buf, "GUI") == 0) {
                m_game->GetGuiManager()->LoadGui(&dest, 0);
            } else if (strcmp(buf, "ShadowZone") == 0) {
                m_game->GetShadowZone()->LoadShadowZone(&dest);
            } else if (strcmp(buf, "SecondPassTimer") == 0) {
                m_second_pass_timer = dest.GetFloatValue();
            } else if (strcmp(buf, "SCRFX") == 0) {
                m_game_room_manager->CreateScreenEffect(dest);
            }
        } while (dest.GetNextSiblingChunk(dest));
    }

    if (iter.GetChunk("PathNodes", dest)) {
        if (!LoadPathNodes(xmd, dest)) {
            return FALSE;
        }
    }

    delete xmd;
    delete xmd_data;

    return TRUE;
}

BOOL CGamePartIngame::StartFightMode(CEntityMesh* mesh, int a2) {
    m_game_room_manager->GetState(); // unused

    if (((CEntityNPC*)mesh)->GetGenericBehaviour() != 4 && m_game_room_manager->GetState() != 2) {
        return FALSE;
    }

    if (a2 != 0) {
        m_game_room_manager->AddFlag(1 << 1);
    } else {
        m_game_room_manager->AddFlag(1 << 0);
    }
    m_game_room_manager->SetEntitiesOnFight(mesh);

    return TRUE;
}

BOOL CGamePartIngame::InterruptFightMode() {
    if (m_game_room_manager->GetState() == 6) {
        m_game_room_manager->AddFlag(1 << 11);
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL CGamePartIngame::UnblockFightMode() {
    if (m_game_room_manager->GetState() == 6) {
        m_game_room_manager->AddFlag(1 << 19);
        return TRUE;
    } else {
        return FALSE;
    }
}

// Incomplete: Unwanted copy, downcast
BOOL CGamePartIngame::HeroAskForFightModeLaunch() {
    if (m_game_room_manager->GetFlag() & (1 << 2)) {
        return FALSE;
    }

    if (m_game->GetCurrentHeroId() != HERO_PIGLET || m_game_room_manager->IsOnFight() || m_game_room_manager->GetState() != 2) {
        return FALSE;
    }

    if (m_game_room_manager->UnkInlineFor4C()) {
        CEntityNPC* npc = m_game_room_manager->m_unk4C;
        if (npc->GetType() == ENTITY_NPC_DOOR && !((CEntityNPCDoor*)(npc))->CanLaunchFightMode()) {
            return FALSE;
        }

        CEntityNPCDoor* door = (CEntityNPCDoor*)npc;
        if (door->GetGenericBehaviour() == GENERIC_BEHAVIOUR_0) {
            door->SaveBaseBehaviour();
        }
        door->SetGenericBehaviour(GENERIC_BEHAVIOUR_4);
        door->CreatePathFinder(m_game_room_manager->GetCurrentHero()->GetPosition(), 1, 0);

        m_game_room_manager->GetState(); // unused
        if (door->GetGenericBehaviour() == GENERIC_BEHAVIOUR_4 || m_game_room_manager->GetState() == 2) {
            m_game_room_manager->AddFlag(1 << 0);
            m_game_room_manager->SetEntitiesOnFight(door);
        }

        return TRUE;
    } else {
        return FALSE;
    }
}

void CGamePartIngame::StartRTCMode(CRTCPlayerEntity* player, int a2) {
    if (m_game_room_manager->GetFlag() & (1 << 0)) {
        return;
    }
    if (m_game_room_manager->GetFlag() & (1 << 1)) {
        return;
    }

    m_game_room_manager->AddFlag(1 << 2);

    CGameRoomManager* manager = m_game_room_manager;
    manager->m_player_entity = player;
    manager->m_flags |= (1 << 20);

    if (a2 != 0) {
        m_game_room_manager->AddFlag(1 << 24);
    } else {
        m_game_room_manager->RemFlag(1 << 24);
    }
}

BOOL CGamePartIngame::IsAllCookiesCollected() {
    U32 count = m_game->GetEntityManager()->GetEntityTypeCount(ENTITY_SEARCHABLE_ZONE);
    for (U32 i = 0; i < count; i++) {
        CEntitySearchableZone* zone = (CEntitySearchableZone*)m_game->GetEntityManager()->GetEntityType(ENTITY_SEARCHABLE_ZONE, i);
        if (zone != NULL && zone->m_unk21C != zone->m_generated_entity_number) {
            return FALSE;
        }
    }
    return TRUE;
}

void CGamePartIngame::SetupSoundsLayer(U32 a1, U32 a2) {
    switch (a1) {
        case 1:
            m_game->GetSoundEngine()->ResumeLayer(1);
            break;
        case 4:
            if (a2) {
                m_game->GetSoundEngine()->ResumeLayer(1);
            } else {
                m_game->GetSoundEngine()->PauseLayer(1);
            }
            break;
        case 2:
            if (a2) {
                if (m_game_room_manager->IsOnFight()) {
                    m_game->GetSoundEngine()->ResumeLayer(4);
                } else {
                    m_game->GetSoundEngine()->ResumeLayer(1);
                }
            } else {
                if (m_game_room_manager->IsOnFight()) {
                    m_game->GetSoundEngine()->PauseLayer(4);
                } else {
                    m_game->GetSoundEngine()->PauseLayer(1);
                }
            }
            break;
        case 3:
            if (a2) {
                m_game->GetSoundEngine()->ResumeLayer(1);
            } else {
                m_game->GetSoundEngine()->PauseLayer(1);
            }
            break;
        case 5:
            if (a2) {
                m_game->GetSoundEngine()->ResumeLayer(1);
            } else {
                m_game->GetSoundEngine()->PauseLayer(1);
            }
            break;
    }
}
