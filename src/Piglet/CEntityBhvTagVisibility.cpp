#include "CEntityBhvTagVisibility.h"
#include <cstring>
#include <iostream>

CEntityBhvTagVisibility::CEntityBhvTagVisibility() {
    m_on = TRUE;
}

CEntityBhvTagVisibility::~CEntityBhvTagVisibility() {

}

void CEntityBhvTagVisibility::Parse(DkXmd::CChunkIterator iter) {
    if (strcmp(iter.GetStringValue(), "On") == 0) {
        m_on = TRUE;
    } else {
        m_on = FALSE;
    }
}
