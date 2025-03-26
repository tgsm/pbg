#include "CEntityBhvTagCollision.h"
#include <cstring>

CEntityBhvTagCollision::CEntityBhvTagCollision() {
    m_on = TRUE;
}

CEntityBhvTagCollision::~CEntityBhvTagCollision() {

}

void CEntityBhvTagCollision::Parse(DkXmd::CChunkIterator iter) {
    char str[128];

    strcpy(str, iter.GetStringValue());
    if (strcmp(str, "On") == 0) {
        m_on = TRUE;
    } else {
        m_on = FALSE;
    }
}
