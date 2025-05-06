#include "entities/CEntityBhvTagBoxPosition.h"
#include "entities/CEntity.h"
#include "entities/CEntityPushingBox.h"
#include <cstring>
#include <iostream>

CEntityBhvTagBoxPosition::CEntityBhvTagBoxPosition() {
    m_x = 0;
    m_y = 0;
}

CEntityBhvTagBoxPosition::~CEntityBhvTagBoxPosition() {

}

void CEntityBhvTagBoxPosition::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest;

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "X") == 0) {
                m_x = dest.GetS32Value();
            } else if (strcmp(buf, "Y") == 0) {
                m_y = dest.GetS32Value();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CEntityBhvTagBoxPosition::Set(CEntity* entity) {
    if (entity->GetType() == ENTITY_PUSHING_BOX) {
        ((CEntityPushingBox*)entity)->DeleteCurrentBoxPosition();
        ((CEntityPushingBox*)entity)->SetBoxPosition(m_x, m_y);
    }
}
