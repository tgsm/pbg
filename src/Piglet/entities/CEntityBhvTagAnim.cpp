#include "entities/CEntityBhvTagAnim.h"
#include <iostream>

CEntityBhvTagAnim::CEntityBhvTagAnim() {

}

CEntityBhvTagAnim::~CEntityBhvTagAnim() {

}

void CEntityBhvTagAnim::Parse(DkXmd::CChunkIterator iter) {
    m_unkC = iter.GetStringValue();
}
