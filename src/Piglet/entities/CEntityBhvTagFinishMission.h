#ifndef PIGLET_ENTITIES_CENTITYBHVTAGFINISHMISSION_H
#define PIGLET_ENTITIES_CENTITYBHVTAGFINISHMISSION_H

#include "engine/xmd/CChunkIterator.h"
#include "entities/CEntityBhvTagData.h"
#include "entities/CEntity.h"
#include "CGame.h"
#include "CGamePartIngame.h"

class CEntityBhvTagFinishMission : public CEntityBhvTagData {
public:
    CEntityBhvTagFinishMission();
    virtual ~CEntityBhvTagFinishMission();

    virtual U32 GetType() {
        return BEHAVIOR_TAG_FINISH_MISSION;
    }

    virtual void Set(CEntity* entity) {
        if (entity == NULL) {
            return;
        }

        entity->m_entity_manager->GetGame()->ResetOpcodeBuffer();
        entity->m_entity_manager->GetGame()->PushOpcodeValue(3);
        entity->m_entity_manager->GetGame()->PushOpcodeValue(14);
        CGamePartIngame* game_part = (CGamePartIngame*)entity->m_entity_manager->GetGame()->GetGamePartPointer();
        game_part->m_unk4 = 0;
        entity->m_entity_manager->GetGame()->SetCurrentRoomReturnType(CGame::RETURN_TYPE_0, -1);

        CGame* game = entity->m_entity_manager->GetGame();
        game->m_unk502C = game->m_fade_duration + game->m_unk5048;

        if (entity->m_entity_manager->GetGame()->m_unk4F54 == 7) {
            CDKW_RGBA fade_color = entity->m_entity_manager->GetGame()->ComputeGameFadeColor();
            entity->m_entity_manager->GetGame()->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.red, fade_color.green, fade_color.blue, 0.0f);
            entity->m_entity_manager->GetGame()->m_unk503C &= ~(1 << 3);
            ((CGamePartIngame*)entity->m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->m_flags |= (1 << 5);
            entity->m_entity_manager->GetGame()->FadeIn(-1.0f);
        }
    }

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagFinishMission, 0xC);

#endif
