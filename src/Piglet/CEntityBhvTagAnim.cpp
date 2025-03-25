#include "CEntityBhvTagAnim.h"

CEntityBhvTagAnim::CEntityBhvTagAnim() {

}

CEntityBhvTagAnim::~CEntityBhvTagAnim() {

}

void CEntityBhvTagAnim::Parse(DkXmd::CChunkIterator iter) {
    m_unkC = iter.GetStringValue();
}
