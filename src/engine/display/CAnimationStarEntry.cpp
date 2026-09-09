#include "dont_inline_hack.h"
#include "engine/display/CAnimationStar.h"
#include "engine/display/CAnimDictionary.h"
#include "engine/display/CAnimationStarEntry.h"
#include "display/CParticleEmitterDefinition.h"

namespace DKDSP {

CAnimationStarEntry::CAnimationStarEntry() : m_name("") {
    Release();
}

// Equivalent: std::vector dtor
CAnimationStarEntry::~CAnimationStarEntry() {
    Release();
}

void CAnimationStarEntry::Release() {
    m_values.speed = 1.0f;
    m_values.loop_mode = ANIMATION_LOOP_1;
    m_values.target_weight = 1.0f;
    m_values.unkC = 0.0f;
    m_values.delay_in = 0.1f;
    m_values.delay_out = 0.1f;
    m_values.unk18 = 1;
    m_values.cut = FALSE;
    m_star = NULL;
}

void CAnimationStarEntry::SetAnimationStar(CAnimationStar* star) {
    m_star = star;
}

void CAnimationStarEntry::LoadTargetAnimationValues(DkXmd::CChunkIterator* iter_, STargetAnimationValues* dest) {
    DONT_INLINE_HACK();
    if (iter_ != NULL && dest != NULL && dest->entry != NULL && dest->entry->GetAnimationValues() != NULL) {
        dest->delay_in = dest->entry->GetAnimationValues()->GetDelayIn();
        dest->delay_out = m_values.GetDelayOut();
        dest->cut = m_values.GetCut();

        DkXmd::CChunkIterator iter;
        iter = *iter_;
        do {
            char* name = iter.GetName();
            if (dkdsp_stricmp(name, "Cut") == 0) {
                if (iter.GetS32Value() == 0) {
                    dest->cut = FALSE;
                } else {
                    dest->cut = TRUE;
                }
            } else if (dkdsp_stricmp(name, "DelayIn") == 0) {
                dest->delay_in = iter.GetFloatValue();
            } else if (dkdsp_stricmp(name, "DelayOut") == 0) {
                dest->delay_out = iter.GetFloatValue();
            }
        } while (iter.GetNextSiblingChunk(iter));
    }
}

// FIXME: One of these two functions likely did not actually exist.
BOOL CAnimationStarEntry::GetTargetAnimationEntryChunkDoWhile(DkXmd::CChunkIterator* iter, DkXmd::CChunkIterator* dest) {
    if (iter == NULL || dest == NULL) {
        return FALSE;
    }

    do {
        if (dkdsp_stricmp(iter->GetName(), "TargetAnimationEntry") == 0) {
            iter->GetFirstChildChunk(*dest);
            return TRUE;
        }
    } while (iter->GetNextSiblingChunk(*iter) == TRUE);

    return FALSE;
}

BOOL CAnimationStarEntry::GetTargetAnimationEntryChunk(DkXmd::CChunkIterator* iter, DkXmd::CChunkIterator* dest) {
    if (iter == NULL || dest == NULL) {
        return FALSE;
    }

    while (iter->GetNextSiblingChunk(*iter) == TRUE) {
        if (dkdsp_stricmp(iter->GetName(), "TargetAnimationEntry") == 0) {
            iter->GetFirstChildChunk(*dest);
            return TRUE;
        }
    }

    return FALSE;
}

void CAnimationStarEntry::LoadTargetAnimationList(DkXmd::CChunkIterator* iter) {
    if (iter == NULL) {
        return;
    }

    DkXmd::CChunkIterator list_chunk;
    if (iter->GetChunk("TargetAnimationList", list_chunk) != TRUE) {
        return;
    }

    DkXmd::CChunkIterator dest, entry_chunk;
    list_chunk.GetFirstChildChunk(dest);

    BOOL found;
    if (GetTargetAnimationEntryChunkDoWhile(&dest, &entry_chunk) == TRUE) {
        do {
            STargetAnimationValues values;
            values.delay_in = 0.1f;
            values.delay_out = 0.1f;
            values.cut = FALSE;
            values.entry = NULL;
            found = FALSE;

            DkXmd::CChunkIterator animation_id_chunk;
            if (dest.GetChunk("AnimationId", animation_id_chunk) == TRUE) {
                CAnimationStarEntry* entry = m_star->GetAnimationEntry(animation_id_chunk.GetStringValue());
                if (entry != NULL) {
                    found = TRUE;
                } else {
                    char error[2048];
                    sprintf(error, "CAnimationStarEntry::LoadTargetAnimationList() - Unable to find the referenced animation [%s]", animation_id_chunk.GetStringValue());
                }

                values.entry = entry;
            }
            LoadTargetAnimationValues(&entry_chunk, &values);
            if (found == TRUE) {
                m_target_animations.push_back(values);
            }
        } while (GetTargetAnimationEntryChunk(&dest, &entry_chunk) == TRUE);
    }
}

int CAnimationStarEntry::GetAnimationNodeFromChunk(DkXmd::CChunkIterator* iter) {
    if (iter == NULL) {
        return -1;
    }

    DkXmd::CChunkIterator dest;
    if (iter->GetChunk("Node", dest)) {
        return dest.GetS32Value();
    } else {
        return -1;
    }
}

int CAnimationStarEntry::GetSyncFlagFromChunk(DkXmd::CChunkIterator* iter) {
    if (iter == NULL) {
        return -1;
    }

    DkXmd::CChunkIterator dest;
    if (iter->GetChunk("DontSync", dest)) {
        return dest.GetS32Value();
    } else {
        return 0;
    }
}

// Equivalent: stack offsets
void CAnimationStarEntry::LoadAnimationList(DkXmd::CChunkIterator* iter) {
    if (iter == NULL) {
        return;
    }

    DkXmd::CChunkIterator list_chunk;
    if (iter->GetChunk("LocalAnimationList", list_chunk) != TRUE) {
        return;
    }

    DkXmd::CChunkIterator dest;
    if (list_chunk.GetFirstChildChunk(dest)) {
        do {
            if (dkdsp_stricmp(dest.GetName(), "ANMEntry") == 0) {
                SAnimationEntry entry;
                DkXmd::CChunkIterator anm_chunk;
                if (dest.GetChunk("ANM", anm_chunk)) {
                    CAnimation* animation;
                    char* name = anm_chunk.GetStringValue();
                    if (name != NULL) {
                        entry.animation = NULL;
                        entry.node = -1;
                        entry.unk4 = -1;
                        entry.sync_flag = 0;

                        animation = m_star->GetAnimDictionary()->FindAnimation(name);
                        if (animation == NULL) {
                            animation = m_star->GetAnimDictionary()->LoadAnimationFromFile(name, name);
                        }
                        int node = GetAnimationNodeFromChunk(&dest);
                        if (animation != NULL) {
                            entry.animation = animation;
                            entry.node = node;
                            m_animation_entries.push_back(entry);
                        }
                    }
                }
            } else if (dkdsp_stricmp(dest.GetName(), "DMAEntry") == 0) {
                SAnimationEntry entry;
                DkXmd::CChunkIterator dma_chunk;
                if (dest.GetChunk("DMA", dma_chunk)) {
                    CDMorphAnimation* animation;
                    char* pcVar2 = dma_chunk.GetStringValue();
                    if (pcVar2 != NULL) {
                        entry.animation = NULL;
                        entry.node = -1;
                        entry.unk4 = -1;
                        entry.sync_flag = 0;

                        animation = m_star->GetAnimDictionary()->FindDMorphAnimation(pcVar2);
                        if (animation == NULL) {
                            animation = m_star->GetAnimDictionary()->LoadDMorphAnimationFromFile(pcVar2, pcVar2);
                        }
                        int node = GetAnimationNodeFromChunk(&dest);
                        if (animation != NULL) {
                            entry.animation = animation;
                            entry.node = node;
                            m_animation_entries.push_back(entry);
                        }
                    }
                }
            } else if (dkdsp_stricmp(dest.GetName(), "TANEntry") == 0) {
                SAnimationEntry entry;
                DkXmd::CChunkIterator tan_chunk;
                if (dest.GetChunk("TAN", tan_chunk)) {
                    CMaterialAnimation* animation;
                    char* pcVar2 = tan_chunk.GetStringValue();
                    if (pcVar2 != NULL) {
                        entry.animation = NULL;
                        entry.node = -1;
                        entry.unk4 = -1;
                        entry.sync_flag = 0;

                        animation = m_star->GetAnimDictionary()->FindMaterialAnimation(pcVar2);
                        if (animation == NULL) {
                            animation = m_star->GetAnimDictionary()->LoadMaterialAnimationFromFile(pcVar2, pcVar2);
                        }
                        int node = GetAnimationNodeFromChunk(&dest);
                        if (animation != NULL) {
                            entry.animation = animation;
                            entry.node = node;
                            m_animation_entries.push_back(entry);
                        }
                    }
                }
            } else if (dkdsp_stricmp(dest.GetName(), "EVNEntry") == 0) {
                SAnimationEntry entry;
                DkXmd::CChunkIterator evn_chunk;
                if (dest.GetChunk("EVN", evn_chunk)) {
                    CEventAnimation* animation;
                    char* pcVar2 = evn_chunk.GetStringValue();
                    if (pcVar2 != NULL) {
                        entry.animation = NULL;
                        entry.node = -1;
                        entry.unk4 = -1;
                        entry.sync_flag = 0;

                        animation = m_star->GetAnimDictionary()->FindEventAnimation(pcVar2);
                        if (animation == NULL) {
                            animation = m_star->GetAnimDictionary()->LoadEventAnimationFromFile(pcVar2, pcVar2);
                        }
                        int sync_flag = GetSyncFlagFromChunk(&dest);
                        if (animation != NULL) {
                            entry.animation = animation;
                            entry.node = -1;
                            entry.sync_flag = sync_flag;
                            m_animation_entries.push_back(entry);
                        }
                    }
                }
            }
        } while (dest.GetNextSiblingChunk(dest));
    }
}

BOOL CAnimationStarEntry::DumpDot(IRWStream* stream, BOOL a2, int a3) {
    if ((CRWStream*)stream == NULL) {
        return FALSE;
    }

    for (int i = 0; i < (int)m_target_animations.size(); i++) {
        STargetAnimationValues* target = &m_target_animations[i];
        if (target != NULL) {
            char target_info[1024];
            sprintf(target_info, "");
            if (a2 == TRUE) {
                sprintf(target_info, "[label=\"DelayIn=%0.1f\\nDelayOut=%0.1f\\nCut=%s\"]", target->delay_in, target->delay_out, (target->cut == FALSE) ? "FALSE" : "TRUE");
            }

            char buf_c28[2048];
            sprintf(buf_c28, "Node%d -> Node%d", a3, m_star->GetAnimationEntryIndex(target->entry));
            char buf_1428[2048];
            sprintf(buf_1428, "%s %s\r\n", buf_c28, target_info);

            if (((CRWStream*)stream)->Write(buf_1428, strlen(buf_1428)) == FALSE) {
                return FALSE;
            }
        }
    }

    return TRUE;
}

BOOL CAnimationStarEntry::DumpDotNode(IRWStream* stream, int id) {
    if ((CRWStream*)stream == NULL) {
        return FALSE;
    }

    char buf[2048];
    sprintf(buf, "Node%d [shape=\"box\",label=\"%s\",URL=\"%s\"]\r\n", id, m_name.c_str(), m_name.c_str());

    return ((CRWStream*)stream)->Write(buf, strlen(buf)) != FALSE;
}

SAnimationStarValues* CAnimationStarEntry::GetAnimationValues() {
    return &m_values;
}

int CAnimationStarEntry::GetNumberOfTargetAnimations() {
    return m_target_animations.size();
}

std::string* CAnimationStarEntry::GetTargetAnimationName(int id) {
    if (id < 0 || id >= GetNumberOfTargetAnimations()) {
        return NULL;
    }

    return m_target_animations[id].entry->GetName();
}

// Equivalent? Needs more work
BOOL CAnimationStarEntry::FindNextAnimationInPath(IAnimationStarEntry* a1, IAnimationStarEntry* a2, IAnimationStarEntry* a3, int a4, SAnimationPath* path) {
    if (a4 <= 0) {
        return FALSE;
    }

    if (a3 == NULL || path == NULL) {
        return FALSE;
    }

    BOOL ret = FALSE;
    int iVar7 = -1;
    int a4_ = a4 - 1;
    F32 dVar10 = 0.0f;
    int iVar9 = 0;
    CAnimationStarEntry* pCVar8 = NULL;

    SAnimationPath local_68;
    local_68.unk0 = 0.0f;
    local_68.unk4 = 0;
    local_68.unk8 = 0;
    local_68.unkC = NULL;
    local_68.unk10 = NULL;

    for (int i = 0; i < (int)m_target_animations.size(); i++) {
        STargetAnimationValues& values = m_target_animations[i];
        if (values.entry != NULL) {
            if (values.entry == a3) {
                BOOL cut = values.cut;
                path->unk0 = values.delay_out;
                path->unk8 = 1;
                path->unk4 = cut;
                path->unkC = values.entry;
                path->unk10 = &values;
                return TRUE;
            }

            if (values.entry != a1 && values.entry != a2) {
                BOOL bVar1 = TRUE;
                if (ret == TRUE && iVar9 < local_68.unk8) {
                    bVar1 = FALSE;
                }
                if (bVar1 == TRUE && values.entry->FindNextAnimationInPath(a1, this, a3, a4_, &local_68) == TRUE) {
                    if (ret == FALSE) {
                        dVar10 = local_68.unk0;
                        iVar9 = local_68.unk8;
                        pCVar8 = values.entry;
                        ret = TRUE;
                        iVar7 = i;
                    } else {
                        if (local_68.unk0 < dVar10 || local_68.unk8 < iVar9) {
                            bVar1 = TRUE;
                        } else {
                            bVar1 = FALSE;
                        }
                        if (bVar1 == TRUE) {
                            dVar10 = local_68.unk0;
                            iVar9 = local_68.unk8;
                            pCVar8 = values.entry;
                            iVar7 = i;
                        }
                    }
                }
            }
        }
    }

    if (ret == TRUE) {
        if (pCVar8 != NULL) {
            STargetAnimationValues& values = m_target_animations[iVar7];
            BOOL cut = values.cut;
            path->unk0 = dVar10 + values.delay_out + values.delay_in;
            path->unk8 = iVar9 + 1;
            path->unk4 = cut;
            path->unkC = values.entry;
            path->unk10 = &values;
        } else {
            ret = FALSE;
        }
    }

    return ret;
}

int CAnimationStarEntry::GetSize() {
    return 0;
}

std::string* CAnimationStarEntry::GetName() {
    return &m_name;
}

void CAnimationStarEntry::SetName(std::string name) {
    m_name.assign(name, 0);
}

int CAnimationStarEntry::GetNumberOfAnimations() {
    return m_animation_entries.size();
}

IGenericAnimation* CAnimationStarEntry::GetAnimation(int id) {
    if (id < 0 || id >= GetNumberOfAnimations()) {
        return NULL;
    }

    return m_animation_entries[id].animation;
}

int CAnimationStarEntry::GetAnimationNode(int id) {
    if (id < 0 || id >= GetNumberOfAnimations()) {
        return -1;
    }

    return m_animation_entries[id].node;
}

F32 CAnimationStarEntry::GetAnimationDuration(EANIMATIONTYPE type) {
    for (int i = 0; i < GetNumberOfAnimations(); i++) {
        IGenericAnimation* animation = GetAnimation(i);
        if (animation != NULL && type == animation->GetType()) {
            return animation->GetDuration();
        }
    }

    return -1.0f;
}

int CAnimationStarEntry::GetAnimationSyncFlag(int id) {
    if (id < 0 || id >= GetNumberOfAnimations()) {
        return -1;
    }

    return m_animation_entries[id].sync_flag;
}

}
