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
        CCACamera* camera = entity->GetManager()->GetGame()->GetIngameGamePart()->m_camera;
        CGame* game = entity->GetManager()->GetGame();
        CMission* mission = game->GetCurrentMission();

        int room_id = game->GetCurrentRoomId();
        if (m_room == -1) {
            m_room = room_id;
        }

        if (room_id == m_room && camera != NULL) {
            camera->SetActiveSystemForAZone((char*)m_cam_zone.data(), (char*)m_cam_system.data());
            if (mission != NULL) {
                zones = camera->GetZoneList();
                for (int i = 0; i < (int)zones.size(); i++) {
                    mission->m_unk7C.unk[room_id].unk0[i] = zones[i]->GetActiveSystemIndex();
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
