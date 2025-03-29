#include "CEntityBhvTagActivation.h"
#include <cstring>

CEntityBhvTagActivation::CEntityBhvTagActivation() {
    m_on = TRUE;
}

CEntityBhvTagActivation::~CEntityBhvTagActivation() {

}

void CEntityBhvTagActivation::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    strcpy(buf, iter.GetStringValue());

    if (strcmp(buf, "On") == 0) {
        m_on = TRUE;
    } else {
        m_on = FALSE;
    }
}
