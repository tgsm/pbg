#include "entities/CEntityWarp.h"
#include <cstring>
#include <iostream>

CEntityWarp::CEntityWarp(CEntityManager* entity_manager, std::string name) : CEntity(entity_manager, name) {
    m_warp = NULL;
}

CEntityWarp::~CEntityWarp() {
    if (m_warp != NULL) {
        m_entity_manager->GetGame()->m_scene->RemoveWarp(m_warp);
        m_warp = NULL;
    }
}

void CEntityWarp::Reset() {
    CEntity::Reset();
}

void CEntityWarp::Update(f32 dt_maybe) {
    if (m_warp == NULL) {
        return;
    }

    for (int i = 0; i < m_warp->GetNumberOfClumps(); i++) {
        DKDSP::CClump* clump = (DKDSP::CClump*)m_warp->GetClump(i);
        if (clump != NULL) {
            clump->Update(dt_maybe);
        }
    }
}

void CEntityWarp::Render(f32 dt_maybe) {
    if (m_warp == NULL) {
        return;
    }

    m_entity_manager->GetGame()->m_scene->RenderWarp(m_warp);
}

void CEntityWarp::Parse(DkXmd::CChunkIterator iter) {
    CEntity::Parse(iter);

    char buf_94[128];
    char buf_114[128];
    char buf_194[128];
    char clump_id[128];
    char animation_name[128];
    DkXmd::CChunkIterator dest;
    if (iter.GetFirstChildChunk(dest)) {
        do {
            strcpy(buf_114, dest.GetName());

            if (strcmp(buf_114, "TexDict") == 0) {
                strcpy(buf_94, dest.GetStringValue());
                m_entity_manager->GetGame()->m_resource_factory->LoadResource(6, buf_94);
            }
        } while (dest.GetNextSiblingChunk(dest));
    }

    if (iter.GetChunk("WARP", dest)) {
        DkXmd::CChunkIterator object_list_chunk;
        if (dest.GetChunk("ObjectList", object_list_chunk)) {
            DkXmd::CChunkIterator child_chunk;
            if (object_list_chunk.GetFirstChildChunk(child_chunk)) {
                do {
                    strcpy(buf_94, child_chunk.GetName());
                    if (strcmp(buf_94, "ObjectEntry") == 0) {
                        DkXmd::CChunkIterator entry_chunk;
                        if (child_chunk.GetChunk("Id", entry_chunk)) {
                            strcpy(buf_194, entry_chunk.GetStringValue());
                            m_entity_manager->GetGame()->m_resource_factory->LoadResource(1, buf_194);

                            if (child_chunk.GetChunk("ANM", entry_chunk)) {
                                strcpy(buf_194, entry_chunk.GetStringValue());
                                m_entity_manager->GetGame()->m_resource_factory->LoadResource(3, buf_194);
                            }
                        }
                    }
                } while (child_chunk.GetNextSiblingChunk(child_chunk));
            }
        }

        m_warp = (DKDSP::CWarp*)m_entity_manager->GetGame()->m_scene->CreateWarp();
        if (m_warp == NULL) {
            return;
        }

        m_warp->Load(&dest);

        if (dest.GetChunk("ObjectList", object_list_chunk)) {
            DkXmd::CChunkIterator child_chunk;
            if (object_list_chunk.GetFirstChildChunk(child_chunk)) {
                do {
                    strcpy(buf_94, child_chunk.GetName());
                    if (strcmp(buf_94, "ObjectEntry") == 0) {
                        DkXmd::CChunkIterator entry_chunk;
                        if (child_chunk.GetChunk("Id", entry_chunk)) {
                            strcpy(clump_id, entry_chunk.GetStringValue());

                            if (child_chunk.GetChunk("ANM", entry_chunk)) {
                                strcpy(animation_name, entry_chunk.GetStringValue());
                                DKDSP::CClump* clump = (DKDSP::CClump*)m_warp->FindClump(clump_id);
                                if (clump != NULL) {
                                    // FIXME: Remove this cast
                                    clump->SetAnimation((DKDSP::IAnimation*)m_entity_manager->GetGame()->m_anim_dictionary->FindAnimation(animation_name));
                                }
                            }
                        }
                    }
                } while (child_chunk.GetNextSiblingChunk(child_chunk));
            }
        }
    }
}

void CEntityWarp::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntity::ParseBehavior(iter, behavior);
}
