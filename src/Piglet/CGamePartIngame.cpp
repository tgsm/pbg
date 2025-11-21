#include "CGamePartIngame.h"
#include "CGame.h"
#include "entities/CEntity.h"
#include "entities/CEntityNPC.h"
#include "entities/CEntityNPCDoor.h"
#include "entities/CEntitySearchableZone.h"
#include <iostream>

// Incomplete
CGamePartIngame::CGamePartIngame(CGame* game) {

}

// Incomplete
CGamePartIngame::~CGamePartIngame() {

}

// Incomplete: Unwanted copy, downcast
BOOL CGamePartIngame::HeroAskForFightModeLaunch() {
    if (m_game_room_manager->GetFlag() & (1 << 2)) {
        return FALSE;
    }

    if (m_game->m_unk4F5C != 0 || m_game_room_manager->IsOnFight() || m_game_room_manager->GetState() != 2) {
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
    U32 count = m_game->m_entity_manager->GetEntityTypeCount(ENTITY_SEARCHABLE_ZONE);
    for (U32 i = 0; i < count; i++) {
        CEntitySearchableZone* zone = (CEntitySearchableZone*)m_game->m_entity_manager->GetEntityType(ENTITY_SEARCHABLE_ZONE, i);
        if (zone != NULL && zone->m_unk21C != zone->m_generated_entity_number) {
            return FALSE;
        }
    }
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

void CGamePartIngame::SetupSoundsLayer(U32 a1, U32 a2) {
    switch (a1) {
        case 1:
            m_game->m_sound_engine->ResumeLayer(1);
            break;
        case 4:
            if (a2) {
                m_game->m_sound_engine->ResumeLayer(1);
            } else {
                m_game->m_sound_engine->PauseLayer(1);
            }
            break;
        case 2:
            if (a2) {
                if (m_game_room_manager->IsOnFight()) {
                    m_game->m_sound_engine->ResumeLayer(4);
                } else {
                    m_game->m_sound_engine->ResumeLayer(1);
                }
            } else {
                if (m_game_room_manager->IsOnFight()) {
                    m_game->m_sound_engine->PauseLayer(4);
                } else {
                    m_game->m_sound_engine->PauseLayer(1);
                }
            }
            break;
        case 3:
            if (a2) {
                m_game->m_sound_engine->ResumeLayer(1);
            } else {
                m_game->m_sound_engine->PauseLayer(1);
            }
            break;
        case 5:
            if (a2) {
                m_game->m_sound_engine->ResumeLayer(1);
            } else {
                m_game->m_sound_engine->PauseLayer(1);
            }
            break;
    }
}
