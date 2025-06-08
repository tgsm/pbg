#ifndef PIGLET_ENTITIES_CENTITYBHVTAGCAMERASYSTEM_H
#define PIGLET_ENTITIES_CENTITYBHVTAGCAMERASYSTEM_H

#include <string>
#include "engine/xmd/CChunkIterator.h"
#include "entities/CEntityBhvTagData.h"

class CEntityBhvTagCameraSystem : public CEntityBhvTagData {
public:
    std::string m_cam_zone;
    std::string m_cam_system;
    int m_room;

public:
    CEntityBhvTagCameraSystem();
    virtual ~CEntityBhvTagCameraSystem();

    virtual void Set(CEntity* entity);
    virtual U32 GetType() {
        return BEHAVIOR_TAG_CAMERA_SYSTEM;
    }

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagCameraSystem, 0x18);

#endif
