#pragma once on

#include "engine/CChunkIterator.h"
#include "entities/CEntity.h"
#include "entities/CEntityBhvTagData.h"

class CEntityBhvTagTeleport : public CEntityBhvTagData {
private:
    f32 m_hero_pos_x;
    f32 m_hero_pos_y;
    f32 m_hero_pos_z;
    f32 m_hero_rot_x;
    f32 m_hero_rot_y;
    f32 m_hero_rot_z;
    int m_room;
    u32 m_hero;
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
        CGamePart* game_part = entity->m_entity_manager->GetGame()->GetGamePartPointer();
        game_part->m_unk4 = 0;

        entity->m_entity_manager->GetGame()->SetCurrentRoomReturnType(CGame::RETURN_TYPE_0, -1);

        if (!m_no_fade) {
            CDKW_RGBA fade_color = entity->m_entity_manager->GetGame()->ComputeGameFadeColor();
            entity->m_entity_manager->GetGame()->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.m_r, fade_color.m_g, fade_color.m_b, 0.0f);
            entity->m_entity_manager->GetGame()->m_unk503C &= ~(1 << 3); // bitfield?

            // FIXME: Which GamePart is this?
            u8* game_part_raw = reinterpret_cast<u8*>(entity->m_entity_manager->GetGame()->GetGamePartPointer());
            **((u32**)(game_part_raw+0x14)) |= (1 << 5);

            entity->m_entity_manager->GetGame()->FadeIn(-1.0f);

            CEntity* hero = entity->m_entity_manager->GetHero();
            if (hero != NULL) {
                entity->m_entity_manager->GetGame()->GetMailbox()->SendMessage(entity->m_unk0, hero->m_unk0, "EXIT", 0);
            }
        } else {
            CGame* game = entity->m_entity_manager->GetGame();
            game->m_unk502C = game->m_unk5030 + game->m_unk5048;
        }
    }

    virtual u32 GetType() {
        return 10;
    }
};
REQUIRE_SIZE(CEntityBhvTagTeleport, 0x30);
