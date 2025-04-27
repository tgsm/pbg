#include "entities/CEntityBhvTagOrientation.h"
#include "entities/CEntityObject.h"
#include <cstring>
#include <iostream>

CEntityBhvTagOrientation::CEntityBhvTagOrientation() {
    m_orientation = CDKW_V3d(0.0f, 0.0f, 0.0f);
}

CEntityBhvTagOrientation::~CEntityBhvTagOrientation() {

}

void CEntityBhvTagOrientation::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest;

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());
            if (strcmp(buf, "X") == 0) {
                m_orientation.m_x = dest.GetFloatValue();
            } else if (strcmp(buf, "Y") == 0) {
                m_orientation.m_y = dest.GetFloatValue();
            } else if (strcmp(buf, "Z") == 0) {
                m_orientation.m_z = dest.GetFloatValue();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CEntityBhvTagOrientation::Set(CEntity* entity) {
    ((CEntityObject*)entity)->SetOrientation(m_orientation);
    ((CEntityObject*)entity)->UpdateVolumes();
}
