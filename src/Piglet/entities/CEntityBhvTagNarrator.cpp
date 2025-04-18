#include "entities/CEntityBhvTagNarrator.h"
#include "entities/CEntity.h"
#include <iostream>

CEntityBhvTagNarrator::CEntityBhvTagNarrator() {

}

CEntityBhvTagNarrator::~CEntityBhvTagNarrator() {

}

void CEntityBhvTagNarrator::Parse(DkXmd::CChunkIterator iter) {
    m_narrator_line_id = iter.GetStringValue();
}
