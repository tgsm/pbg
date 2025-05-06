#include "entities/CEntityBhvTagReceiveShadow.h"
#include <cstring>
#include <iostream>

CEntityBhvTagReceiveShadow::CEntityBhvTagReceiveShadow() {
    m_on = TRUE;
}

CEntityBhvTagReceiveShadow::~CEntityBhvTagReceiveShadow() {

}

void CEntityBhvTagReceiveShadow::Parse(DkXmd::CChunkIterator iter) {
    if (strcmp(iter.GetStringValue(), "On") == 0) {
        m_on = TRUE;
    } else {
        m_on = FALSE;
    }
}
