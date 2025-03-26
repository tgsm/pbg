#include "CEntityBhvTagNarrator.h"
#include "CEntity.h"

CEntityBhvTagNarrator::CEntityBhvTagNarrator() {

}

CEntityBhvTagNarrator::~CEntityBhvTagNarrator() {

}

void CEntityBhvTagNarrator::Parse(DkXmd::CChunkIterator iter) {
    m_narrator_line_id = iter.GetStringValue();
}
