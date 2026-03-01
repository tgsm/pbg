#include "engine/display/CAnimDictionary.h"
#include "engine/display/CEngine.h"

// FIXME: Get rid of this
#define CLEAR_VECTOR(vec) \
    (vec).m_size = 0; \
    if ((vec).m_data != NULL) { \
        delete (vec).m_data; \
    } \
    (vec).m_data = NULL; \
    (vec).m_capacity = 0

// FIXME: These need a home.
extern U8 __lower_map[256];
inline int tolower(int c) { return (c == -1 ? -1 : (int)__lower_map[(U8)c]); }

int dkdsp_stricmp(const char* a, const char* b) {
    if (a == NULL) {
        return -1;
    } else if (b == NULL) {
        return 1;
    }

    char a_, b_;
    do {
        a_ = tolower(*(a++));
        b_ = tolower(*(b++));

        if (a_ < b_) {
            return -1;
        } else if (a_ > b_) {
            return 1;
        }
    } while (a_ != 0);

    return 0;
}

namespace DKDSP {

CAnimDictionary::~CAnimDictionary() {
    Release();
}

void CAnimDictionary::Release() {
    RemoveAllAnimationStarControllers();
    RemoveAllAnimationStars();
    RemoveAllControllers();
    RemoveAllAnimations();
    RemoveAllDMorphAnimations();
    RemoveAllMaterialAnimations();
    RemoveAllEventAnimations();
}

U32 CAnimDictionary::GetAnimationSize() {
    U32 size = 0;
    for (int i = 0; i < (int)m_animations.size(); i++) {
        if (m_animations[i] != NULL) {
            size += m_animations[i]->GetSize();
        }
    }
    size += m_animations.m_capacity * sizeof(CAnimation*);
    return size;
}

U32 CAnimDictionary::GetDMorphAnimationSize() {
    U32 size = 0;
    for (int i = 0; i < (int)m_dmorph_animations.size(); i++) {
        if (m_dmorph_animations[i] != NULL) {
            size += m_dmorph_animations[i]->GetSize();
        }
    }
    size += m_dmorph_animations.m_capacity * sizeof(CDMorphAnimation*);
    return size;
}

U32 CAnimDictionary::GetMaterialAnimationSize() {
    U32 size = 0;
    for (int i = 0; i < (int)m_material_animations.size(); i++) {
        if (m_material_animations[i] != NULL) {
            size += m_material_animations[i]->GetSize();
        }
    }
    size += m_material_animations.m_capacity * sizeof(CMaterialAnimation*);
    return size;
}

U32 CAnimDictionary::GetEventAnimationSize() {
    U32 size = 0;
    for (int i = 0; i < (int)m_event_animations.size(); i++) {
        if (m_event_animations[i] != NULL) {
            size += m_event_animations[i]->GetSize();
        }
    }
    size += m_event_animations.m_capacity * sizeof(CEventAnimation*);
    return size;
}

U32 CAnimDictionary::GetControllerSize() {
    U32 size = 0;
    for (int i = 0; i < (int)m_controllers.size(); i++) {
        if (m_controllers[i] != NULL) {
            size += m_controllers[i]->GetSize();
        }
    }
    size += m_controllers.m_capacity * sizeof(CController*);
    return size;
}

U32 CAnimDictionary::GetAnimationStarSize() {
    U32 size = 0;
    for (int i = 0; i < (int)m_animation_stars.size(); i++) {
        if (m_animation_stars[i] != NULL) {
            size += m_animation_stars[i]->GetSize();
        }
    }
    size += m_animation_stars.m_capacity * sizeof(CAnimationStar*);
    return size;
}

U32 CAnimDictionary::GetAnimationStarControllerSize() {
    U32 size = 0;
    for (int i = 0; i < (int)m_animation_star_controllers.size(); i++) {
        if (m_animation_star_controllers[i] != NULL) {
            size += m_animation_star_controllers[i]->GetSize();
        }
    }
    size += m_animation_star_controllers.m_capacity * sizeof(CAnimationStarController*);
    return size;
}

U32 CAnimDictionary::GetSize() {
    return GetAnimationSize() + GetDMorphAnimationSize() + GetMaterialAnimationSize() +
           GetEventAnimationSize() + GetControllerSize() + GetAnimationStarSize() +
           GetAnimationStarControllerSize();
}

U32 CAnimDictionary::GetVectorSize() {
    return (m_animation_star_controllers.m_capacity + m_animation_stars.m_capacity +
            m_controllers.m_capacity + m_event_animations.m_capacity + m_material_animations.m_capacity +
            m_dmorph_animations.m_capacity + m_animations.m_capacity) * sizeof(unsigned long);
}

// Equivalent
CAnimation* CAnimDictionary::AddAnimation(CAnimation* animation, std::string name) {
    unsigned long ret = reinterpret_cast<unsigned long>(animation);
    reinterpret_cast<CAnimation*>(ret)->SetName(name);
    CAnimation* found_animation = FindAnimation(name);
    if (found_animation != NULL) {
        reinterpret_cast<CAnimation*>(ret)->Release();
        delete reinterpret_cast<CAnimation*>(ret);
        ret = NULL;
        return found_animation;
    } else {
        for (int i = 0; i < (int)m_animations.size(); i++) {
            if (m_animations[i] == NULL) {
                m_animations[i] = reinterpret_cast<CAnimation*>(ret);
                return reinterpret_cast<CAnimation*>(ret);
            }
        }
        AS_ULONG_VECTOR_HACK(m_animations).push_back(ret);
        return reinterpret_cast<CAnimation*>(ret);
    }
}

CAnimation* CAnimDictionary::LoadAnimation(std::string name, IRWStream* stream_) {
    CRWStream* stream = (CRWStream*)stream_;
    if (stream == NULL) {
        return NULL;
    }

    CAnimation* found_animation = FindAnimation(name);
    if (found_animation != NULL) {
        return found_animation;
    }

    CDKW_Animation* wrap_animation = stream->ReadAnimation();
    if (wrap_animation == NULL) {
        return NULL;
    }

    CAnimation* ret = new CAnimation(wrap_animation);
    return AddAnimation(ret, name);
}

CAnimation* CAnimDictionary::LoadAnimationFromFile(std::string name, char* file) {
    if (file == NULL) {
        return NULL;
    }

    CAnimation* found_animation = FindAnimation(name);
    if (found_animation != NULL) {
        return found_animation;
    }

    CRWStream* stream = m_engine->OpenStream(rwSTREAMFILENAME, rwSTREAMREAD, (void*)file);
    CAnimation* ret = NULL;
    if (stream != NULL) {
        if (stream->FindChunk(27, 0, 0)) {
            ret = LoadAnimation(name, stream);
        }
        m_engine->DestroyStream(stream);
    }

    return ret;
}

CAnimation* CAnimDictionary::LoadSmoothAnimation(std::string name, IRWStream* stream_) {
    CRWStream* stream = (CRWStream*)stream_;
    if (stream == NULL) {
        return NULL;
    }

    CAnimation* found_animation = FindAnimation(name);
    if (found_animation != NULL) {
        return found_animation;
    }

    CDKW_Animation* wrap_animation = stream->ReadSmoothAnimation();
    if (wrap_animation == NULL) {
        return NULL;
    }

    CAnimation* ret = new CAnimation(wrap_animation);
    return AddAnimation(ret, name);
}

CAnimation* CAnimDictionary::LoadSmoothAnimationFromFile(std::string name, char* file) {
    if (file == NULL) {
        return NULL;
    }

    CAnimation* found_animation = FindAnimation(name);
    if (found_animation != NULL) {
        return found_animation;
    }

    CRWStream* stream = m_engine->OpenStream(rwSTREAMFILENAME, rwSTREAMREAD, (void*)file);
    CAnimation* ret = NULL;
    if (stream != NULL) {
        if (stream->FindChunk(27, 0, 0)) {
            ret = LoadSmoothAnimation(name, stream);
        }
        m_engine->DestroyStream(stream);
    }

    return ret;
}

BOOL CAnimDictionary::WriteAnimation(IAnimation* animation_, char* a2) {
    if (animation_ == NULL) {
        return FALSE;
    }
    CDKW_Animation* wrap_animation = ((CAnimation*)animation_)->m_wrap_animation;

    CRWStream* stream = CEngine::ms_pEngine->OpenStreamFileName(rwSTREAMWRITE, a2);
    if (stream == NULL) {
        return FALSE;
    }

    BOOL result = stream->WriteAnimation(wrap_animation);
    CEngine::ms_pEngine->DestroyStream(stream);
    return result;
}

BOOL CAnimDictionary::WriteAnimation(IAnimation* animation_, IRWStream* stream_) {
    if (animation_ == NULL) {
        return FALSE;
    }
    CAnimation* animation = (CAnimation*)animation_;

    CRWStream* stream = (CRWStream*)stream_;
    return stream->WriteAnimation(animation->m_wrap_animation);
}

CAnimation* CAnimDictionary::FindAnimation(std::string name) {
    for (int i = 0; i < (int)m_animations.size(); i++) {
        CAnimation* animation = m_animations[i];
        if (animation != NULL && *animation->GetName() == name) {
            return animation;
        }
    }

    return NULL;
}

std::string* CAnimDictionary::FindAnimationID(IAnimation* animation_) {
    if (animation_ == NULL) {
        return NULL;
    }

    CAnimation* animation = (CAnimation*)animation_;
    for (int i = 0; i < (int)m_animations.size(); i++) {
        if (m_animations[i] == animation) {
            return m_animations[i]->GetName();
        }
    }

    return NULL;
}

int CAnimDictionary::GetNumberOfAnimations() {
    return m_animations.size();
}

CAnimation* CAnimDictionary::GetAnimation(int id) {
    if (id < 0 || id >= GetNumberOfAnimations()) {
        return NULL;
    }
    return m_animations[id];
}

void CAnimDictionary::RemoveAnimation(IAnimation* animation_) {
    if (animation_ == NULL) {
        return;
    }

    CAnimation* animation = (CAnimation*)animation_;
    for (int i = 0; i < (int)m_animations.size(); i++) {
        if (m_animations[i] == animation) {
            RemoveAnimation(i);
            return;
        }
    }
}

void CAnimDictionary::RemoveAllAnimations() {
    for (int i = 0; i < (int)m_animations.size(); i++) {
        CAnimation* animation = m_animations[i];
        if (animation != NULL) {
            animation->Release();
            delete animation;
        }
        m_animations[i] = NULL;
    }

    CLEAR_VECTOR(m_animations);
}

// Equivalent
CDMorphAnimation* CAnimDictionary::AddDMorphAnimation(CDMorphAnimation* animation, std::string name) {
    unsigned long ret = reinterpret_cast<unsigned long>(animation);
    reinterpret_cast<CDMorphAnimation*>(ret)->SetName(name);
    CDMorphAnimation* found_animation = FindDMorphAnimation(name);
    if (found_animation != NULL) {
        reinterpret_cast<CDMorphAnimation*>(ret)->Release();
        delete reinterpret_cast<CDMorphAnimation*>(ret);
        ret = NULL;
        return found_animation;
    } else {
        for (int i = 0; i < (int)m_animations.size(); i++) {
            if (m_animations[i] == NULL) {
                m_animations[i] = reinterpret_cast<CAnimation*>(ret);
                return reinterpret_cast<CDMorphAnimation*>(ret);
            }
        }
        AS_ULONG_VECTOR_HACK(m_animations).push_back(ret);
        return reinterpret_cast<CDMorphAnimation*>(ret);
    }
}

CDMorphAnimation* CAnimDictionary::LoadDMorphAnimation(std::string name, IRWStream* stream_) {
    CRWStream* stream = (CRWStream*)stream_;
    if (stream == NULL) {
        return NULL;
    }

    CDMorphAnimation* found_animation = FindDMorphAnimation(name);
    if (found_animation != NULL) {
        return found_animation;
    }

    RpDMorphAnimation* rw_dmorph_animation = stream->ReadDMorphAnimation();
    if (rw_dmorph_animation == NULL) {
        return NULL;
    }

    CDMorphAnimation* ret = new CDMorphAnimation(rw_dmorph_animation);
    return AddDMorphAnimation(ret, name);
}

CDMorphAnimation* CAnimDictionary::LoadDMorphAnimationFromFile(std::string name, char* file) {
    if (file == NULL) {
        return NULL;
    }

    CDMorphAnimation* found_animation = FindDMorphAnimation(name);
    if (found_animation != NULL) {
        return found_animation;
    }

    CRWStream* stream = m_engine->OpenStream(rwSTREAMFILENAME, rwSTREAMREAD, (void*)file);
    CDMorphAnimation* ret = NULL;
    if (stream != NULL) {
        if (stream->FindChunk(30, 0, 0)) {
            ret = LoadDMorphAnimation(name, stream);
        }
        m_engine->DestroyStream(stream);
    }

    return ret;
}

CDMorphAnimation* CAnimDictionary::FindDMorphAnimation(std::string name) {
    for (int i = 0; i < (int)m_dmorph_animations.size(); i++) {
        CDMorphAnimation* animation = m_dmorph_animations[i];
        if (animation != NULL && *animation->GetName() == name) {
            return animation;
        }
    }

    return NULL;
}

std::string* CAnimDictionary::FindDMorphAnimationID(IDMorphAnimation* animation_) {
    if (animation_ == NULL) {
        return NULL;
    }

    CDMorphAnimation* animation = (CDMorphAnimation*)animation_;
    for (int i = 0; i < (int)m_dmorph_animations.size(); i++) {
        if (m_dmorph_animations[i] == animation) {
            return m_dmorph_animations[i]->GetName();
        }
    }

    return NULL;
}

int CAnimDictionary::GetNumberOfDMorphAnimations() {
    return m_dmorph_animations.size();
}

CDMorphAnimation* CAnimDictionary::GetDMorphAnimation(int id) {
    if (id < 0 || id >= GetNumberOfDMorphAnimations()) {
        return NULL;
    }
    return m_dmorph_animations[id];
}

void CAnimDictionary::RemoveDMorphAnimation(std::string name) {
    for (int i = 0; i < (int)m_dmorph_animations.size(); i++) {
        if (m_dmorph_animations[i] != NULL) {
            if (*m_dmorph_animations[i]->GetName() == name) {
                RemoveDMorphAnimation(i);
                return;
            }
        }
    }
}

void CAnimDictionary::RemoveDMorphAnimation(IDMorphAnimation* animation_) {
    if (animation_ == NULL) {
        return;
    }

    CDMorphAnimation* animation = (CDMorphAnimation*)animation_;
    for (int i = 0; i < (int)m_dmorph_animations.size(); i++) {
        if (m_dmorph_animations[i] == animation) {
            RemoveDMorphAnimation(i);
            return;
        }
    }
}

void CAnimDictionary::RemoveAllDMorphAnimations() {
    for (int i = 0; i < (int)m_dmorph_animations.size(); i++) {
        CDMorphAnimation* animation = m_dmorph_animations[i];
        if (animation != NULL) {
            animation->Release();
            delete animation;
        }
        m_dmorph_animations[i] = NULL;
    }

    CLEAR_VECTOR(m_dmorph_animations);
}

// Equivalent
CMaterialAnimation* CAnimDictionary::AddMaterialAnimation(CMaterialAnimation* animation, std::string name) {
    unsigned long ret = reinterpret_cast<unsigned long>(animation);
    reinterpret_cast<CMaterialAnimation*>(ret)->SetName(name);
    CMaterialAnimation* found_animation = FindMaterialAnimation(name);
    if (found_animation != NULL) {
        reinterpret_cast<CMaterialAnimation*>(ret)->Release();
        delete reinterpret_cast<CMaterialAnimation*>(ret);
        ret = NULL;
        return found_animation;
    } else {
        for (int i = 0; i < (int)m_material_animations.size(); i++) {
            if (m_material_animations[i] == NULL) {
                m_material_animations[i] = reinterpret_cast<CMaterialAnimation*>(ret);
                return reinterpret_cast<CMaterialAnimation*>(ret);
            }
        }
        AS_ULONG_VECTOR_HACK(m_material_animations).push_back(ret);
        return reinterpret_cast<CMaterialAnimation*>(ret);
    }
}

CMaterialAnimation* CAnimDictionary::LoadMaterialAnimation(std::string name, IRWStream* stream_) {
    void* xmd_data;
    CMaterialAnimation* found_animation;
    int offset;
    U32 unk;
    CMaterialAnimation* ret;

    CRWStream* stream = (CRWStream*)stream_;
    if (stream == NULL) {
        return NULL;
    }

    found_animation = FindMaterialAnimation(name);
    if (found_animation != NULL) {
        return found_animation;
    }

    xmd_data = NULL;
    offset = 0;
    do {
        xmd_data = RwEngineInstance->memoryFuncs.rwrealloc(xmd_data, offset + 0x400);
        if (xmd_data == NULL) {
            return NULL;
        }

        unk = stream->Read((void*)((int)xmd_data + offset), 0x400);
        offset += unk;
    } while (unk == 0x400);

    DkXmd::CXmdFile xmd;
    xmd.Parse(xmd_data);

    ret = LoadMaterialAnimationFromChunk(name, &xmd.m_chunk_iterator);
    RwEngineInstance->memoryFuncs.rwfree(xmd_data);
    return ret;
}

CMaterialAnimation* CAnimDictionary::LoadMaterialAnimationFromFile(std::string name, char* file) {
    if (file == NULL) {
        return NULL;
    }

    CMaterialAnimation* found_animation = FindMaterialAnimation(name);
    if (found_animation != NULL) {
        return found_animation;
    }

    CRWStream* stream = m_engine->OpenStream(rwSTREAMFILENAME, rwSTREAMREAD, (void*)file);
    CMaterialAnimation* ret = NULL;
    if (stream != NULL) {
        ret = LoadMaterialAnimation(name, stream);
        m_engine->DestroyStream(stream);
    }

    return ret;
}

// Incomplete
CMaterialAnimation* CAnimDictionary::LoadMaterialAnimationFromChunk(std::string name, DkXmd::CChunkIterator* iter) {
    CMaterialAnimation* animation = NULL;

    if (iter == NULL) {
        return NULL;
    }

    CMaterialAnimation* found_animation = FindMaterialAnimation(name);
    if (found_animation != NULL) {
        return found_animation;
    }

    if (iter->GetName() == NULL) {
        return NULL;
    }

    if (dkdsp_stricmp(iter->GetName(), "BitmapAnimation") != 0) {
        return NULL;
    }

    DkXmd::CChunkIterator dest;
    if (!iter->GetFirstChildChunk(dest)) {
        return NULL;
    }

    dest.GetNextSiblingChunk(dest);
    if (dkdsp_stricmp(dest.GetName(), "BitmapList") != 0) {
        return NULL;
    }

    DkXmd::CChunkIterator dest2;
    dest.GetFirstChildChunk(dest2);

    while (dkdsp_stricmp(dest2.GetName(), "Bitmap") == 0) {
        DkXmd::CChunkIterator dest3;
        dest2.GetFirstChildChunk(dest3);
        dest3.GetNextSiblingChunk(dest3);

        EMATANIMSCHEME uVar10 = MAT_ANIM_SCHEME_0;
        EMATANIMSCHEME uVar12 = MAT_ANIM_SCHEME_0;
        EMATANIMSCHEME uVar11 = MAT_ANIM_SCHEME_0;
        if (dkdsp_stricmp(dest3.GetName(), "POS_CTRL_TYPE") == 0) {
            if (dest3.GetS32Value() > 0) {
                uVar10 = MAT_ANIM_SCHEME_1;
            }
        }
        dest3.GetNextSiblingChunk(dest3);
        if (dkdsp_stricmp(dest3.GetName(), "SCALE_CTRL_TYPE") == 0) {
            if (dest3.GetS32Value() > 0) {
                uVar12 = MAT_ANIM_SCHEME_1;
            }
        }
        dest3.GetNextSiblingChunk(dest3);
        if (dkdsp_stricmp(dest3.GetName(), "ROT_CTRL_TYPE") == 0) {
            if (dest3.GetS32Value() > 0) {
                uVar11 = MAT_ANIM_SCHEME_1;
            }
        }
        dest3.GetNextSiblingChunk(dest3);
        if (dkdsp_stricmp(dest3.GetName(), "Duration") != 0) {
            return NULL;
        }
        F32 duration = dest3.GetFloatValue();

        dest3.GetNextSiblingChunk(dest3);
        if (dkdsp_stricmp(dest3.GetName(), "KeyList") != 0) {
            return NULL;
        }

        DkXmd::CChunkIterator dest4;
        dest3.GetFirstChildChunk(dest4);
        int nb_entries = 0;
        while (dkdsp_stricmp(dest4.GetName(), "KeyEntry") == 0) {
            nb_entries++;
            if (!dest4.GetNextSiblingChunk(dest4)) {
                break;
            }
        }
        animation = CreateMaterialAnimation(name, nb_entries, duration);
        if (animation == NULL) {
            return NULL;
        }
        animation->SetInterpolatorType(MAT_ANIM_COMPONENT_1, uVar10);
        animation->SetInterpolatorType(MAT_ANIM_COMPONENT_0, uVar11);
        animation->SetInterpolatorType(MAT_ANIM_COMPONENT_2, uVar12);

        nb_entries = 0;
        while (dkdsp_stricmp(dest4.GetName(), "KeyEntry") == 0) {
            SMATERIALANIMKEY key;
            DkXmd::CChunkIterator dest5;

            dest4.GetFirstChildChunk(dest5);
            key.unk0 = dest5.GetFloatValue();
            dest5.GetNextSiblingChunk(dest5);
            dest5.GetFloatValue();
            dest5.GetNextSiblingChunk(dest5);
            key.unk4 = dest5.GetFloatValue();
            dest5.GetNextSiblingChunk(dest5);
            key.unk8 = dest5.GetFloatValue();
            dest5.GetNextSiblingChunk(dest5);
            key.unkC = dest5.GetFloatValue();
            dest5.GetNextSiblingChunk(dest5);
            key.unk10 = dest5.GetFloatValue();
            dest5.GetNextSiblingChunk(dest5);
            key.unk14 = dest5.GetFloatValue();

            animation->SetKey(nb_entries, key);
            nb_entries++;

            if (!dest4.GetNextSiblingChunk(dest4)) {
                break;
            }
        }

        if (!dest2.GetNextSiblingChunk(dest2)) {
            break;
        }
    }

    return animation;
}

CMaterialAnimation* CAnimDictionary::CreateMaterialAnimation(std::string name, int a2, F32 a3) {
    CMaterialAnimation* found_animation = FindMaterialAnimation(name);
    if (found_animation != NULL) {
        return found_animation;
    }

    CMaterialAnimation* ret = new CMaterialAnimation;
    ret->Create(a2, a3, 0);
    return AddMaterialAnimation(ret, name);
}

CMaterialAnimation* CAnimDictionary::FindMaterialAnimation(std::string name) {
    for (int i = 0; i < (int)m_material_animations.size(); i++) {
        CMaterialAnimation* animation = m_material_animations[i];
        if (animation != NULL && *animation->GetName() == name) {
            return animation;
        }
    }

    return NULL;
}

std::string* CAnimDictionary::FindMaterialAnimationID(IMaterialAnimation* animation_) {
    if (animation_ == NULL) {
        return NULL;
    }

    CMaterialAnimation* animation = (CMaterialAnimation*)animation_;
    for (int i = 0; i < (int)m_material_animations.size(); i++) {
        if (m_material_animations[i] == animation) {
            return m_material_animations[i]->GetName();
        }
    }

    return NULL;
}

int CAnimDictionary::GetNumberOfMaterialAnimations() {
    return m_material_animations.size();
}

CMaterialAnimation* CAnimDictionary::GetMaterialAnimation(int id) {
    if (id < 0 || id >= GetNumberOfMaterialAnimations()) {
        return NULL;
    }
    return m_material_animations[id];
}

void CAnimDictionary::RemoveMaterialAnimation(std::string name) {
    for (int i = 0; i < (int)m_material_animations.size(); i++) {
        if (m_material_animations[i] != NULL) {
            if (*m_material_animations[i]->GetName() == name) {
                RemoveMaterialAnimation(i);
                return;
            }
        }
    }
}

void CAnimDictionary::RemoveMaterialAnimation(IMaterialAnimation* animation_) {
    if (animation_ == NULL) {
        return;
    }

    CMaterialAnimation* animation = (CMaterialAnimation*)animation_;
    for (int i = 0; i < (int)m_material_animations.size(); i++) {
        if (m_material_animations[i] == animation) {
            RemoveMaterialAnimation(i);
            return;
        }
    }
}

void CAnimDictionary::RemoveAllMaterialAnimations() {
    for (int i = 0; i < (int)m_material_animations.size(); i++) {
        CMaterialAnimation* material_animation = m_material_animations[i];
        if (material_animation != NULL) {
            material_animation->Release();
            delete material_animation;
        }
        m_material_animations[i] = NULL;
    }

    CLEAR_VECTOR(m_material_animations);
}

// Equivalent
CEventAnimation* CAnimDictionary::AddEventAnimation(CEventAnimation* animation, std::string name) {
    unsigned long ret = reinterpret_cast<unsigned long>(animation);
    reinterpret_cast<CEventAnimation*>(ret)->SetName(name);
    CEventAnimation* found_animation = FindEventAnimation(name);
    if (found_animation != NULL) {
        reinterpret_cast<CEventAnimation*>(ret)->Release();
        delete reinterpret_cast<CEventAnimation*>(ret);
        ret = NULL;
        return found_animation;
    } else {
        for (int i = 0; i < (int)m_event_animations.size(); i++) {
            if (m_event_animations[i] == NULL) {
                m_event_animations[i] = reinterpret_cast<CEventAnimation*>(ret);
                return reinterpret_cast<CEventAnimation*>(ret);
            }
        }
        AS_ULONG_VECTOR_HACK(m_event_animations).push_back(ret);
        return reinterpret_cast<CEventAnimation*>(ret);
    }
}

CEventAnimation* CAnimDictionary::LoadEventAnimation(std::string name, IRWStream* stream_) {
    CEventAnimation* found_animation;
    void* xmd_data;
    int offset;
    U32 unk;
    CEventAnimation* ret;

    CRWStream* stream = (CRWStream*)stream_;
    if (stream == NULL) {
        return NULL;
    }

    found_animation = FindEventAnimation(name);
    if (found_animation != NULL) {
        return found_animation;
    }

    xmd_data = NULL;
    offset = 0;
    do {
        xmd_data = RwEngineInstance->memoryFuncs.rwrealloc(xmd_data, offset + 0x400);
        if (xmd_data == NULL) {
            return NULL;
        }

        unk = stream->Read((void*)((int)xmd_data + offset), 0x400);
        offset += unk;
    } while (unk == 0x400);

    DkXmd::CXmdFile* xmd = new DkXmd::CXmdFile;
    if (xmd == NULL) {
        RwEngineInstance->memoryFuncs.rwfree(xmd_data);
        return NULL;
    }
    xmd->Parse(xmd_data);

    ret = LoadEventAnimationFromChunk(name, &xmd->m_chunk_iterator, xmd, xmd_data);
    if (ret == NULL) {
        RwEngineInstance->memoryFuncs.rwfree(xmd_data);
    }
    return ret;
}

CEventAnimation* CAnimDictionary::LoadEventAnimationFromFile(std::string name, char* file) {
    if (file == NULL) {
        return NULL;
    }

    CEventAnimation* found_animation = FindEventAnimation(name);
    if (found_animation != NULL) {
        return found_animation;
    }

    CRWStream* stream = m_engine->OpenStream(rwSTREAMFILENAME, rwSTREAMREAD, (void*)file);
    CEventAnimation* ret = NULL;
    if (stream != NULL) {
        ret = LoadEventAnimation(name, stream);
        m_engine->DestroyStream(stream);
    }

    return ret;
}

// Incomplete
CEventAnimation* CAnimDictionary::LoadEventAnimationFromChunk(std::string name, DkXmd::CChunkIterator* iter, DkXmd::CXmdFile* xmd, void* a4) {
    CEventAnimation* animation;

    if (iter == NULL || a4 == NULL) {
        return NULL;
    }

    CEventAnimation* found_animation = FindEventAnimation(name);
    if (found_animation != NULL) {
        return found_animation;
    }

    if (iter->GetName() == NULL) {
        return NULL;
    }

    if (dkdsp_stricmp(iter->GetName(), "EventAnimation") != 0) {
        return NULL;
    }

    DkXmd::CChunkIterator dest;
    if (!iter->GetFirstChildChunk(dest)) {
        return NULL;
    }

    F32 duration = 0.0f;
    int nb_keyframes = 0;
    do {
        if (dkdsp_stricmp(dest.GetName(), "Duration") == 0) {
            duration = dest.GetFloatValue();
        } else if (dkdsp_stricmp(dest.GetName(), "KeyList") == 0) {
            DkXmd::CChunkIterator dest2;
            if (dest.GetFirstChildChunk(dest2)) {
                do {
                    if (dkdsp_stricmp(dest2.GetName(), "KeyEntry") == 0) {
                        nb_keyframes++;
                    }
                } while (dest2.GetNextSiblingChunk(dest2));
            }
        }
    } while (dest.GetNextSiblingChunk(dest));

    animation = CreateEventAnimation(name, nb_keyframes, duration, xmd);
    if (animation == NULL) {
        return NULL;
    }

    animation->m_unk14 = a4;

    DkXmd::CChunkIterator dest2;
    do {
        if (dkdsp_stricmp(dest.GetName(), "KeyList") == 0) {
            dest.GetFirstChildChunk(dest2);
            break;
        }
    } while (dest.GetNextSiblingChunk(dest));

    int keyframe_id = 0;
    if (nb_keyframes > 0) {
        do {
            if (dkdsp_stricmp(dest2.GetName(), "KeyEntry") == 0) {
                DkXmd::CChunkIterator dest3;
                if (dest2.GetFirstChildChunk(dest3)) {
                    DkXmd::CChunkIterator unused_maybe;
                    SEVENTANIMKEY key;
                    do {
                        if (dkdsp_stricmp(dest3.GetName(), "Time") == 0) {
                            key.unk0 = dest3.GetFloatValue();
                        } else if (dkdsp_stricmp(dest3.GetName(), "EventType") == 0) {
                            int id = dest3.GetS32Value();
                            if (id < 0) {
                                key.unk4 = m_engine->FindEvent(dest3.GetStringValue());
                            } else {
                                key.unk4 = m_engine->FindEvent(id);
                            }
                        } else if (dkdsp_stricmp(dest3.GetName(), "EventData") == 0) {
                            ParseEventData(dest3);
                        }
                    } while (dest3.GetNextSiblingChunk(dest3));

                    animation->SetKey(keyframe_id, key);
                    keyframe_id++;
                }
            }
        } while (dest2.GetNextSiblingChunk(dest2));
    }

    return animation;
}

CEventAnimation* CAnimDictionary::CreateEventAnimation(std::string name, int nb_keyframes, F32 duration, DkXmd::CXmdFile* xmd) {
    CEventAnimation* found_animation = FindEventAnimation(name);
    if (found_animation != NULL) {
        return found_animation;
    }

    CEventAnimation* animation = new CEventAnimation;
    animation->Create(nb_keyframes, duration, xmd, 0);
    return AddEventAnimation(animation, name);
}

CEventAnimation* CAnimDictionary::FindEventAnimation(std::string name) {
    for (int i = 0; i < (int)m_event_animations.size(); i++) {
        CEventAnimation* animation = m_event_animations[i];
        if (animation != NULL && *animation->GetName() == name) {
            return animation;
        }
    }

    return NULL;
}

std::string* CAnimDictionary::FindEventAnimationID(IEventAnimation* animation_) {
    if (animation_ == NULL) {
        return NULL;
    }

    CEventAnimation* animation = (CEventAnimation*)animation_;
    for (int i = 0; i < (int)m_event_animations.size(); i++) {
        if (m_event_animations[i] == animation) {
            return m_event_animations[i]->GetName();
        }
    }

    return NULL;
}

int CAnimDictionary::GetNumberOfEventAnimations() {
    return m_event_animations.size();
}

CEventAnimation* CAnimDictionary::GetEventAnimation(int id) {
    if (id < 0 || id >= GetNumberOfEventAnimations()) {
        return NULL;
    }
    return m_event_animations[id];
}

void CAnimDictionary::RemoveEventAnimation(std::string name) {
    for (int i = 0; i < (int)m_event_animations.size(); i++) {
        if (m_event_animations[i] != NULL) {
            if (*m_event_animations[i]->GetName() == name) {
                RemoveEventAnimation(i);
                return;
            }
        }
    }
}

void CAnimDictionary::RemoveEventAnimation(IEventAnimation* animation_) {
    if (animation_ == NULL) {
        return;
    }

    CEventAnimation* animation = (CEventAnimation*)animation_;
    for (int i = 0; i < (int)m_event_animations.size(); i++) {
        if (m_event_animations[i] == animation) {
            RemoveEventAnimation(i);
            return;
        }
    }
}

void CAnimDictionary::RemoveAllEventAnimations() {
    for (int i = 0; i < (int)m_event_animations.size(); i++) {
        CEventAnimation* event_animation = m_event_animations[i];
        if (event_animation != NULL) {
            event_animation->Release();
            delete event_animation;
        }
        m_event_animations[i] = NULL;
    }

    CLEAR_VECTOR(m_event_animations);
}

// Equivalent
CController* CAnimDictionary::CreateController(std::string name, IClump* clump, int a3) {
    if (clump == NULL) {
        return NULL;
    }

    unsigned long controller = reinterpret_cast<unsigned long>(new CController);
    reinterpret_cast<CController*>(controller)->Create(clump, clump->GetHierarchy(), a3);
    reinterpret_cast<CController*>(controller)->SetName(name);

    for (int i = 0; i < (int)m_controllers.size(); i++) {
        if (m_controllers[i] == NULL) {
            m_controllers[i] = reinterpret_cast<CController*>(controller);
            return reinterpret_cast<CController*>(controller);
        }
    }

    AS_ULONG_VECTOR_HACK(m_controllers).push_back(controller);
    return reinterpret_cast<CController*>(controller);
}

CController* CAnimDictionary::FindController(std::string name) {
    for (int i = 0; i < (int)m_controllers.size(); i++) {
        CController* controller = m_controllers[i];
        if (controller != NULL && controller->GetName() == name) {
            return controller;
        }
    }

    return NULL;
}

std::string* CAnimDictionary::FindControllerID(IController* controller_) {
    if (controller_ == NULL) {
        return NULL;
    }

    CController* controller = (CController*)controller_;
    for (int i = 0; i < (int)m_controllers.size(); i++) {
        if (m_controllers[i] == controller) {
            return &m_controllers[i]->GetName();
        }
    }

    return NULL;
}

int CAnimDictionary::GetNumberOfControllers() {
    return m_controllers.size();
}

CController* CAnimDictionary::GetController(int id) {
    if (id < 0 || id >= GetNumberOfControllers()) {
        return NULL;
    }
    return m_controllers[id];
}

void CAnimDictionary::RemoveController(std::string name) {
    for (int i = 0; i < (int)m_controllers.size(); i++) {
        if (m_controllers[i] != NULL) {
            if (m_controllers[i]->GetName() == name) {
                RemoveController(i);
                return;
            }
        }
    }
}
void CAnimDictionary::RemoveController(IController* animation_) {
    if (animation_ == NULL) {
        return;
    }

    CController* animation = (CController*)animation_;
    for (int i = 0; i < (int)m_controllers.size(); i++) {
        if (m_controllers[i] == animation) {
            RemoveController(i);
            return;
        }
    }
}

void CAnimDictionary::RemoveAllControllers() {
    for (int i = 0; i < (int)m_controllers.size(); i++) {
        CController* controller = m_controllers[i];
        if (controller != NULL) {
            controller->Release();
            delete controller;
        }
        m_controllers[i] = NULL;
    }

    CLEAR_VECTOR(m_controllers);
}

// Equivalent
CAnimationStar* CAnimDictionary::CreateAnimationStar(std::string name) {
    unsigned long animation_star = reinterpret_cast<unsigned long>(new CAnimationStar);
    reinterpret_cast<CAnimationStar*>(animation_star)->SetAnimDictionary(this);
    reinterpret_cast<CAnimationStar*>(animation_star)->SetName(name);

    for (int i = 0; i < (int)m_animation_stars.size(); i++) {
        if (m_animation_stars[i] == NULL) {
            m_animation_stars[i] = reinterpret_cast<CAnimationStar*>(animation_star);
            return reinterpret_cast<CAnimationStar*>(animation_star);
        }
    }

    AS_ULONG_VECTOR_HACK(m_animation_stars).push_back(animation_star);
    return reinterpret_cast<CAnimationStar*>(animation_star);
}

CAnimationStar* CAnimDictionary::LoadAnimationStar(std::string name, IRWStream* stream) {
    if (stream == NULL) {
        return NULL;
    }

    CAnimationStar* found_animation_star = FindAnimationStar(name);
    if (found_animation_star != NULL) {
        return found_animation_star;
    }

    CAnimationStar* ret = CreateAnimationStar(name);
    if (ret == NULL) {
        return NULL;
    }

    ret->Load(stream);
    return ret;
}

CAnimationStar* CAnimDictionary::LoadAnimationStarFromFile(std::string name, char* path) {
    if (path == NULL) {
        return NULL;
    }

    CAnimationStar* found_animation_star = FindAnimationStar(name);
    if (found_animation_star != NULL) {
        return found_animation_star;
    }

    CRWStream* stream = m_engine->OpenStream(rwSTREAMFILENAME, rwSTREAMREAD, path);
    if (stream == NULL) {
        return NULL;
    }

    CAnimationStar* ret = LoadAnimationStar(name, stream);
    m_engine->DestroyStream(stream);
    if (ret == NULL) {
        return NULL;
    }

    return ret;
}

CAnimationStar* CAnimDictionary::LoadAnimationStarFromChunk(std::string name, DkXmd::CChunkIterator* iter) {
    if (iter == NULL) {
        return NULL;
    }

    CAnimationStar* found_animation_star = FindAnimationStar(name);
    if (found_animation_star != NULL) {
        return found_animation_star;
    }

    CAnimationStar* ret = CreateAnimationStar(name);
    if (ret == NULL) {
        return NULL;
    }

    ret->Load(iter);
    return ret;
}

CAnimationStar* CAnimDictionary::FindAnimationStar(std::string name) {
    for (int i = 0; i < (int)m_animation_stars.size(); i++) {
        CAnimationStar* animation_star = m_animation_stars[i];
        if (animation_star != NULL && *animation_star->GetName() == name) {
            return animation_star;
        }
    }

    return NULL;
}

std::string* CAnimDictionary::FindAnimationStarID(IAnimationStar* animation_star_) {
    if (animation_star_ == NULL) {
        return NULL;
    }

    CAnimationStar* animation_star = (CAnimationStar*)animation_star_;
    for (int i = 0; i < (int)m_animation_stars.size(); i++) {
        if (m_animation_stars[i] == animation_star) {
            return m_animation_stars[i]->GetName();
        }
    }

    return NULL;
}

int CAnimDictionary::GetNumberOfAnimationStars() {
    return m_animation_stars.size();
}

CAnimationStar* CAnimDictionary::GetAnimationStar(int id) {
    if (id < 0 || id >= GetNumberOfAnimationStars()) {
        return NULL;
    }
    return m_animation_stars[id];
}

void CAnimDictionary::RemoveAnimationStar(std::string name) {
    for (int i = 0; i < (int)m_animation_stars.size(); i++) {
        if (m_animation_stars[i] != NULL) {
            if (*m_animation_stars[i]->GetName() == name) {
                RemoveAnimationStar(i);
                return;
            }
        }
    }
}

void CAnimDictionary::RemoveAnimationStar(IAnimationStar* animation_) {
    if (animation_ == NULL) {
        return;
    }

    CAnimationStar* animation = (CAnimationStar*)animation_;
    for (int i = 0; i < (int)m_animation_stars.size(); i++) {
        if (m_animation_stars[i] == animation) {
            RemoveAnimationStar(i);
            return;
        }
    }
}

void CAnimDictionary::RemoveAllAnimationStars() {
    for (int i = 0; i < (int)m_animation_stars.size(); i++) {
        CAnimationStar* animation_star = m_animation_stars[i];
        if (animation_star != NULL) {
            animation_star->Release();
            delete animation_star;
        }
        m_animation_stars[i] = NULL;
    }

    CLEAR_VECTOR(m_animation_stars);
}

// Equivalent
CAnimationStarController* CAnimDictionary::CreateAnimationStarController(std::string name, IAnimationStar* animation_star_, IController* controller_) {
    if (animation_star_ == NULL || controller_ == NULL) {
        return NULL;
    }

    unsigned long animation_star_controller = reinterpret_cast<unsigned long>(new CAnimationStarController);
    reinterpret_cast<CAnimationStarController*>(animation_star_controller)->SetAnimationStar(animation_star_);
    reinterpret_cast<CAnimationStarController*>(animation_star_controller)->SetController(controller_);
    reinterpret_cast<CAnimationStarController*>(animation_star_controller)->SetName(name);

    for (int i = 0; i < (int)m_animation_star_controllers.size(); i++) {
        if (m_animation_star_controllers[i] == NULL) {
            m_animation_star_controllers[i] = reinterpret_cast<CAnimationStarController*>(animation_star_controller);
            return reinterpret_cast<CAnimationStarController*>(animation_star_controller);
        }
    }

    AS_ULONG_VECTOR_HACK(m_animation_star_controllers).push_back(animation_star_controller);
    return reinterpret_cast<CAnimationStarController*>(animation_star_controller);
}

CAnimationStarController* CAnimDictionary::FindAnimationStarController(std::string name) {
    for (int i = 0; i < (int)m_animation_star_controllers.size(); i++) {
        CAnimationStarController* animation_star_controller = m_animation_star_controllers[i];
        if (animation_star_controller != NULL && *animation_star_controller->GetName() == name) {
            return animation_star_controller;
        }
    }

    return NULL;
}

std::string* CAnimDictionary::FindAnimationStarControllerID(IAnimationStarController* animation_star_) {
    if (animation_star_ == NULL) {
        return NULL;
    }

    CAnimationStarController* animation_star = (CAnimationStarController*)animation_star_;
    for (int i = 0; i < (int)m_animation_star_controllers.size(); i++) {
        if (m_animation_star_controllers[i] == animation_star) {
            return m_animation_star_controllers[i]->GetName();
        }
    }

    return NULL;
}

int CAnimDictionary::GetNumberOfAnimationStarControllers() {
    return m_animation_star_controllers.size();
}

CAnimationStarController* CAnimDictionary::GetAnimationStarController(int id) {
    if (id < 0 || id >= GetNumberOfAnimationStarControllers()) {
        return NULL;
    }
    return m_animation_star_controllers[id];
}

void CAnimDictionary::RemoveAnimationStarController(std::string name) {
    for (int i = 0; i < (int)m_animation_star_controllers.size(); i++) {
        if (m_animation_star_controllers[i] != NULL) {
            if (*m_animation_star_controllers[i]->GetName() == name) {
                RemoveAnimationStarController(i);
                return;
            }
        }
    }
}

void CAnimDictionary::RemoveAnimationStarController(IAnimationStarController* animation_) {
    if (animation_ == NULL) {
        return;
    }

    CAnimationStarController* animation = (CAnimationStarController*)animation_;
    for (int i = 0; i < (int)m_animation_star_controllers.size(); i++) {
        if (m_animation_star_controllers[i] == animation) {
            RemoveAnimationStarController(i);
            return;
        }
    }
}

void CAnimDictionary::RemoveAllAnimationStarControllers() {
    for (int i = 0; i < (int)m_animation_star_controllers.size(); i++) {
        CAnimationStarController* controller = m_animation_star_controllers[i];
        if (controller != NULL) {
            controller->Release();
            delete controller;
        }
        m_animation_star_controllers[i] = NULL;
    }

    CLEAR_VECTOR(m_animation_star_controllers);
}

}
