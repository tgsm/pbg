#include "CRTCCamera.h"
#include <cstring>
#include <iostream>

CRTCCamera::CRTCCamera() {
    m_fov = 45.0f;
    m_unk18 = NULL;
    m_loaded = FALSE;
}

CRTCCamera::~CRTCCamera() {

}

BOOL CRTCCamera::CreateFromFile(DkXmd::CChunkIterator* iter) {
    DkXmd::CChunkIterator dest;

    if (iter->GetFirstChildChunk(dest)) {
        do {
            if (strcmp(dest.GetName(), "Time") == 0) {
                m_unk0 = dest.GetS32Value() / 1000.0f;
            } else if (strcmp(dest.GetName(), "FOV") == 0) {
                m_fov = dest.GetFloatValue();
            } else if (strcmp(dest.GetName(), "File") == 0) {
                m_cam_filename = dest.GetStringValue();
            }
        } while (dest.GetNextSiblingChunk(dest));

        return TRUE;
    }

    return FALSE;
}

void CRTCCamera::Load(CEntityManager* entity_manager) {
    if (m_loaded) {
        return;
    }

    entity_manager->GetGame()->GetResourceFactory()->LoadResource(3, m_cam_filename);
    m_unk18 = entity_manager->GetGame()->m_anim_dictionary->FindAnimation(m_cam_filename);
    m_loaded = TRUE;
}

void CRTCCamera::UnLoad(CEntityManager* entity_manager) {
    if (!m_loaded) {
        return;
    }

    entity_manager->GetGame()->GetResourceFactory()->UnloadSpecificResource(m_cam_filename, TRUE);

    m_unk18 = NULL;
    m_loaded = FALSE;
}
