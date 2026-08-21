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

        entity->GetManager()->GetGame()->ResetOpcodeBuffer();
        entity->GetManager()->GetGame()->PushOpcodeValue(SCRIPT_COMMAND_COMPLETE_CURRENT_MISSION);
        entity->GetManager()->GetGame()->PushOpcodeValue(SCRIPT_COMMAND_UNK14);
        entity->GetManager()->GetGame()->GetIngameGamePart()->m_unk4 = 0;
        entity->GetManager()->GetGame()->SetCurrentRoomReturnType(CGame::RETURN_TYPE_0, -1);

        CGame* game = entity->GetManager()->GetGame();
        game->m_unk502C = game->m_fade_duration + game->m_unk5048;

        if (entity->GetManager()->GetGame()->GetCurrentMissionId() == MISSION_FINAL) {
            CDKW_RGBA fade_color = entity->GetManager()->GetGame()->ComputeGameFadeColor();
            entity->GetManager()->GetGame()->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.red, fade_color.green, fade_color.blue, 0.0f);
            entity->GetManager()->GetGame()->m_unk503C &= ~(1 << 3);
            entity->GetManager()->GetGame()->GetIngameGamePart()->GetGameRoomManager()->m_flags |= (1 << 5);
            entity->GetManager()->GetGame()->FadeIn(-1.0f);
        }
    }

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagFinishMission, 0xC);

#endif
