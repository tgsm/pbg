#include "entities/CEntityBhvTagPosition.h"
#include "entities/CEntityObject.h"
#include <cstring>
#include <iostream>

CEntityBhvTagPosition::CEntityBhvTagPosition() {
    m_position = CDKW_V3d(0.0f, 0.0f, 0.0f);
}

CEntityBhvTagPosition::~CEntityBhvTagPosition() {

}

void CEntityBhvTagPosition::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest;

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "X") == 0) {
                m_position.m_x = dest.GetFloatValue();
            } else if (strcmp(buf, "Y") == 0) {
                m_position.m_y = dest.GetFloatValue();
            } else if (strcmp(buf, "Z") == 0) {
                m_position.m_z = dest.GetFloatValue();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CEntityBhvTagPosition::Set(CEntity* entity) {
    ((CEntityObject*)entity)->SetPosition(m_position);
    ((CEntityObject*)entity)->UpdateVolumes();
}
