#ifndef PIGLET_ENTITIES_CENTITYBHVTAGCAMERASYSTEM_H
#define PIGLET_ENTITIES_CENTITYBHVTAGCAMERASYSTEM_H

#include <string>
#include <vector>
#include "engine/xmd/CChunkIterator.h"
#include "entities/CEntity.h"
#include "entities/CEntityBhvTagData.h"
#include "CCACamera.h"
#include "CGame.h"
#include "CGamePartIngame.h"

class CEntityBhvTagCameraSystem : public CEntityBhvTagData {
public:
    std::string m_cam_zone;
    std::string m_cam_system;
    int m_room;

public:
    CEntityBhvTagCameraSystem();
    virtual ~CEntityBhvTagCameraSystem();

    // Equivalent: regalloc, STL functions shouldn't be inlined here
    virtual void Set(CEntity* entity) {
        std::vector<CCAZone*> zones;
        CCACamera* camera = ((CGamePartIngame*)entity->m_entity_manager->GetGame()->GetGamePartPointer())->m_camera;
        CGame* game = entity->m_entity_manager->GetGame();
        CMission* mission = &game->m_unk210[game->m_unk4F54 - 1];

        int unk4F58 = game->m_unk4F58;
        if (m_room == -1) {
            m_room = unk4F58;
        }

        if (unk4F58 == m_room && camera != NULL) {
            camera->SetActiveSystemForAZone((char*)m_cam_zone.data(), (char*)m_cam_system.data());
            if (mission != NULL) {
                zones = camera->GetZoneList();
                for (int i = 0; i < (int)zones.size(); i++) {
                    mission->m_unk7C[unk4F58].unk0[i] = zones[i]->GetActiveSystemIndex();
                }
            }
        }
    }

    virtual U32 GetType() {
        return BEHAVIOR_TAG_CAMERA_SYSTEM;
    }

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagCameraSystem, 0x18);

#endif
