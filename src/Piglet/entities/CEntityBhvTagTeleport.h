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
        entity->m_entity_manager->GetGame()->ResetOpcodeBuffer();
        if (m_room > 0) {
            entity->m_entity_manager->GetGame()->PushOpcodeValue(1);
            entity->m_entity_manager->GetGame()->PushOpcodeValue(m_room);
            if (entity->m_entity_manager->GetGame()->m_unk4F5C == 3) {
                entity->m_entity_manager->GetGame()->PushOpcodeValue(3);
            } else {
                entity->m_entity_manager->GetGame()->PushOpcodeValue(m_hero);
            }
            entity->m_entity_manager->GetGame()->PushOpcodeValue(m_hero_pos_x);
            entity->m_entity_manager->GetGame()->PushOpcodeValue(m_hero_pos_y);
            entity->m_entity_manager->GetGame()->PushOpcodeValue(m_hero_pos_z);
            entity->m_entity_manager->GetGame()->PushOpcodeValue(m_hero_rot_x);
            entity->m_entity_manager->GetGame()->PushOpcodeValue(m_hero_rot_y);
            entity->m_entity_manager->GetGame()->PushOpcodeValue(m_hero_rot_z);
            entity->m_entity_manager->GetGame()->PushOpcodeValue(0);
        } else {
            if (m_room == -1) {
                entity->m_entity_manager->GetGame()->PushOpcodeValue(8);
            } else if (m_room == -2) {
                entity->m_entity_manager->GetGame()->PushOpcodeValue(9);
            } else if (m_room == -3) {
                entity->m_entity_manager->GetGame()->PushOpcodeValue(7);
            } else if (m_room == -4) {
                entity->m_entity_manager->GetGame()->PushOpcodeValue(5);
            } else if (m_room == -5) {
                entity->m_entity_manager->GetGame()->PushOpcodeValue(4);
            }
        }
        CGamePartIngame* game_part = (CGamePartIngame*)entity->m_entity_manager->GetGame()->GetGamePartPointer();
        game_part->m_unk4 = 0;

        entity->m_entity_manager->GetGame()->SetCurrentRoomReturnType(CGame::RETURN_TYPE_0, -1);

        if (!m_no_fade) {
            CDKW_RGBA fade_color = entity->m_entity_manager->GetGame()->ComputeGameFadeColor();
            entity->m_entity_manager->GetGame()->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.m_r, fade_color.m_g, fade_color.m_b, 0.0f);
            entity->m_entity_manager->GetGame()->m_unk503C &= ~(1 << 3); // bitfield?

            CGamePartIngame* game_part = (CGamePartIngame*)entity->m_entity_manager->GetGame()->GetGamePartPointer();
            game_part->m_game_room_manager->m_unk0 |= (1 << 5);

            entity->m_entity_manager->GetGame()->FadeIn(-1.0f);

            CEntityHero* hero = entity->m_entity_manager->GetHero();
            if (hero != NULL) {
                entity->m_entity_manager->GetGame()->GetMailbox()->SendMessage(entity->m_unk0, hero->m_unk0, "EXIT", 0);
            }
        } else {
            CGame* game = entity->m_entity_manager->GetGame();
            game->m_unk502C = game->m_fade_duration + game->m_unk5048;
        }
    }

    virtual U32 GetType() {
        return BEHAVIOR_TAG_TELEPORT;
    }
};
REQUIRE_SIZE(CEntityBhvTagTeleport, 0x30);

#endif
