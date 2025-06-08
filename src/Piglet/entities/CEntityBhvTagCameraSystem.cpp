#include "entities/CEntityBhvTagCameraSystem.h"
#include <cstring>
#include <iostream>

CEntityBhvTagCameraSystem::CEntityBhvTagCameraSystem() {
    m_cam_zone = "";
    m_cam_system = "";
}

CEntityBhvTagCameraSystem::~CEntityBhvTagCameraSystem() {

}

void CEntityBhvTagCameraSystem::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest;

    m_room = -1;

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "CamZone") == 0) {
                m_cam_zone = dest.GetStringValue();
            } else if (strcmp(buf, "CamSystem") == 0) {
                m_cam_system = dest.GetStringValue();
            } else if (strcmp(buf, "Room") == 0) {
                m_room = dest.GetS32Value();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}
