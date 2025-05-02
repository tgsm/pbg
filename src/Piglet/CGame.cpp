#include "CGame.h"
#include "entities/CEntityHero.h"
#include "entities/CEntityManager.h"

void CGame::ComputeDeltaTime() {
    f32 old_dt = m_delta_time;
    m_delta_time = m_timer->GetDeltaTime();
    m_delta_time += old_dt;
    m_delta_time *= 0.5f;
}

f32 CGame::GetDeltaTime() {
    return m_delta_time;
}

CGamePart* CGame::GetGamePartPointer() {
    return m_game_part;
}

CDKW_RGBA CGame::ComputeGameFadeColor() {
    CDKW_RGBA color;
    color.m_r = 0;
    color.m_g = 0;
    color.m_b = 0;
    color.m_a = 0xFF;

    if (m_game_part != NULL && m_game_part->m_unk0 == 7) {
        CEntityHero* hero = m_entity_manager->GetHero();
        if (hero != NULL && hero->GetType() == ENTITY_PIGLET) {
            switch (hero->GetState()) {
                case 1:
                    color = CDKW_RGBA(26, 26, 67, 0xFF);
                    break;
                case 2:
                    color = CDKW_RGBA(0, 0, 0, 0xFF);
                    break;
                default:
                    color = CDKW_RGBA(199, 226, 222, 0xFF);
                    break;
            }

            if (m_unk4F5C == 3) {
                color = CDKW_RGBA(78, 83, 149, 0xFF);
            }
        } else if (hero != NULL && hero->GetType() == ENTITY_TIGGER) {
            color = CDKW_RGBA(229, 120, 15, 0xFF);
        } else if (hero != NULL && hero->GetType() == ENTITY_WINNIE) {
            color = CDKW_RGBA(229, 120, 15, 0xFF);
        }
    }

    return CDKW_RGBA(color);
}
