#ifndef PIGLET_ENTITIES_CENTITYBHVTAGTELEPORT_H
#define PIGLET_ENTITIES_CENTITYBHVTAGTELEPORT_H

#include "engine/xmd/CChunkIterator.h"
#include "entities/CEntity.h"
#include "entities/CEntityHero.h"
#include "entities/CEntityBhvTagData.h"
#include "CGame.h"
#include "CGamePartIngame.h"

class CEntityBhvTagTeleport : public CEntityBhvTagData {
private:
    F32 m_hero_pos_x;
    F32 m_hero_pos_y;
    F32 m_hero_pos_z;
    F32 m_hero_rot_x;
    F32 m_hero_rot_y;
    F32 m_hero_rot_z;
    int m_room;
    U32 m_hero;
    BOOL m_no_fade;

public:
    CEntityBhvTagTeleport();
    virtual ~CEntityBhvTagTeleport();

    void Parse(DkXmd::CChunkIterator iter);

    virtual void Set(CEntity* entity) {
        entity->GetManager()->GetGame()->ResetOpcodeBuffer();
        if (m_room > 0) {
            entity->GetManager()->GetGame()->PushOpcodeValue(SCRIPT_COMMAND_SET_CURRENT_ROOM);
            entity->GetManager()->GetGame()->PushOpcodeValue(m_room);
            if (entity->GetManager()->GetGame()->GetCurrentHeroId() == HERO_CATCH_THEM_ALL) {
                entity->GetManager()->GetGame()->PushOpcodeValue(HERO_CATCH_THEM_ALL);
            } else {
                entity->GetManager()->GetGame()->PushOpcodeValue(m_hero);
            }
            entity->GetManager()->GetGame()->PushOpcodeValue(m_hero_pos_x);
            entity->GetManager()->GetGame()->PushOpcodeValue(m_hero_pos_y);
            entity->GetManager()->GetGame()->PushOpcodeValue(m_hero_pos_z);
            entity->GetManager()->GetGame()->PushOpcodeValue(m_hero_rot_x);
            entity->GetManager()->GetGame()->PushOpcodeValue(m_hero_rot_y);
            entity->GetManager()->GetGame()->PushOpcodeValue(m_hero_rot_z);
            entity->GetManager()->GetGame()->PushOpcodeValue(0);
        } else {
            if (m_room == -1) {
                entity->GetManager()->GetGame()->PushOpcodeValue(SCRIPT_COMMAND_LOAD_MOVIE_CLIPS);
            } else if (m_room == -2) {
                entity->GetManager()->GetGame()->PushOpcodeValue(SCRIPT_COMMAND_LOAD_SCRAP_BOOK);
            } else if (m_room == -3) {
                entity->GetManager()->GetGame()->PushOpcodeValue(SCRIPT_COMMAND_LOAD_CREDITS);
            } else if (m_room == -4) {
                entity->GetManager()->GetGame()->PushOpcodeValue(SCRIPT_COMMAND_LOAD_MISSION_SELECT_ROOM);
            } else if (m_room == -5) {
                entity->GetManager()->GetGame()->PushOpcodeValue(SCRIPT_COMMAND_UNK4);
            }
        }
        entity->GetManager()->GetGame()->GetIngameGamePart()->m_unk4 = 0;
        entity->GetManager()->GetGame()->SetCurrentRoomReturnType(CGame::RETURN_TYPE_0, -1);

        if (!m_no_fade) {
            CDKW_RGBA fade_color = entity->GetManager()->GetGame()->ComputeGameFadeColor();
            entity->GetManager()->GetGame()->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.red, fade_color.green, fade_color.blue, 0.0f);
            entity->GetManager()->GetGame()->m_unk503C &= ~(1 << 3); // bitfield?
            entity->GetManager()->GetGame()->GetIngameGamePart()->GetGameRoomManager()->m_flags |= (1 << 5);
            entity->GetManager()->GetGame()->FadeIn(-1.0f);

            CEntityHero* hero = entity->GetManager()->GetHero();
            if (hero != NULL) {
                entity->GetManager()->GetGame()->GetMailbox()->SendMessage(entity->GetName(), hero->GetName(), "EXIT", 0);
            }
        } else {
            CGame* game = entity->GetManager()->GetGame();
            game->m_unk502C = game->m_fade_duration + game->m_unk5048;
        }
    }

    virtual U32 GetType() {
        return BEHAVIOR_TAG_TELEPORT;
    }
};
REQUIRE_SIZE(CEntityBhvTagTeleport, 0x30);

#endif
