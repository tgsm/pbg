#include "entities/CEntityBhvTagAnim.h"
#include "entities/CEntityMesh.h"
#include <iostream>

CEntityBhvTagAnim::CEntityBhvTagAnim() {

}

CEntityBhvTagAnim::~CEntityBhvTagAnim() {

}

void CEntityBhvTagAnim::Parse(DkXmd::CChunkIterator iter) {
    m_animation_name = iter.GetStringValue();
}

void CEntityBhvTagAnim::Set(CEntity* entity) {
    if (((CEntityMesh*)entity)->m_animation_star_controller != NULL) {
        ((CEntityMesh*)entity)->m_animation_star_controller->Play(m_animation_name);
    }
}
