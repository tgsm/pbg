#include "entities/CEntityPiglet.h"
#include "CRTCGrimaceEvent.h"
#include <cstring>
#include <iostream>

CRTCGrimaceEvent::CRTCGrimaceEvent(CRTCPlayerEntity* player_entity) : CRTCEvent(player_entity) {
    m_position.x = 0.0f;
    m_position.y = 0.0f;
    m_position.z = 0.0f;
    m_orientation.x = 0.0f;
    m_orientation.y = 0.0f;
    m_orientation.z = 0.0f;
    m_duration = 0.0f;
    m_delay = 0.0f;
    m_type = EVENT_GRIMACE;
}

CRTCGrimaceEvent::~CRTCGrimaceEvent() {

}

void CRTCGrimaceEvent::Activate() {
    CRTCSeqKey::Activate();

    U32 grimace_level = ((CEntityPiglet*)m_player_entity->m_entity_manager->GetHero())->m_grimace_level;

    m_player_entity->m_entity_manager->GetHero()->SetPosition(m_position);
    m_player_entity->m_entity_manager->GetHero()->SetOrientation(m_orientation);
    m_player_entity->m_entity_manager->GetHero()->m_controller->StopAllAnimations(DKDSP::ANIMATION_TYPE_4, 1);

    ((CEntityPiglet*)m_player_entity->m_entity_manager->GetHero())->SetGrimace(grimace_level);

    m_player_entity->m_entity_manager->GetHero()->m_animation_star_controller->Update(0.1f);
    m_player_entity->m_entity_manager->GetHero()->m_animation_star_controller->Pause();

    // what?
    F32 duration = m_duration;
    CRTCPlayerEntity* player_entity = m_player_entity;
    player_entity->m_grimace_delay = m_delay;
    player_entity->m_grimace_duration = duration;
}

BOOL CRTCGrimaceEvent::ParseParam(DkXmd::CChunkIterator* iter) {
    DkXmd::CChunkIterator dest1, dest2;

    if (iter->GetFirstChildChunk(dest1)) {
        do {
            if (strcmp(dest1.GetName(), "Position") == 0) {
                if (dest1.GetFirstChildChunk(dest2)) {
                    do {
                        if (strcmp(dest2.GetName(), "X") == 0) {
                            m_position.x = dest2.GetFloatValue();
                        } else if (strcmp(dest2.GetName(), "Y") == 0) {
                            m_position.y = dest2.GetFloatValue();
                        } else if (strcmp(dest2.GetName(), "Z") == 0) {
                            m_position.z = dest2.GetFloatValue();
                        }
                    } while (dest2.GetNextSiblingChunk(dest2));
                }
            } else if (strcmp(dest1.GetName(), "Orientation") == 0) {
                if (dest1.GetFirstChildChunk(dest2)) {
                    do {
                        if (strcmp(dest2.GetName(), "X") == 0) {
                            m_orientation.x = dest2.GetFloatValue();
                        } else if (strcmp(dest2.GetName(), "Y") == 0) {
                            m_orientation.y = dest2.GetFloatValue();
                        } else if (strcmp(dest2.GetName(), "Z") == 0) {
                            m_orientation.z = dest2.GetFloatValue();
                        }
                    } while (dest2.GetNextSiblingChunk(dest2));
                }
            } else if (strcmp(dest1.GetName(), "Duration") == 0) {
                m_duration = dest1.GetS32Value();
                m_duration /= 1000.0f;
            } else if (strcmp(dest1.GetName(), "Delay") == 0) {
                m_delay = dest1.GetS32Value();
                m_delay /= 1000.0f;
            }
        } while (dest1.GetNextSiblingChunk(dest1));

        return TRUE;
    } else {
        return FALSE;
    }
}
