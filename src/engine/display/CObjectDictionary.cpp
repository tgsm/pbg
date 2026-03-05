#include <new>
#include "engine/display/CEngine.h"
#include "engine/display/CObjectDictionary.h"

namespace DKDSP {

// Equivalent: std::vector
CObjectDictionary::~CObjectDictionary() {
    Release();
}

void CObjectDictionary::Release() {
    RemoveAllClumps();
    RemoveAllSplines();
    RemoveAllAtomics();
    RemoveAllGeometries();
    RemoveAllCamera();
    RemoveAllLights();
    RemoveAllParticleEmitterDefinitions();
}

/// Stubbed.
///
/// @return 0
U32 CObjectDictionary::GetAtomicSize() {
    return 0;
}

U32 CObjectDictionary::GetClumpSize() {
    U32 size = 0;
    for (int i = 0; i < (int)m_clump_entries.size(); i++) {
        if (m_clump_entries[i].clump != NULL) {
            size += m_clump_entries[i].clump->GetSize();
        }
    }

    size += m_clump_entries.m_capacity * sizeof(SCLUMPENTRY);
    return size;
}

/// Stubbed.
///
/// @return 0
U32 CObjectDictionary::GetGeometrySize() {
    return 0;
}

/// Stubbed.
///
/// @return 0
U32 CObjectDictionary::GetCameraSize() {
    return 0;
}

/// Stubbed.
///
/// @return 0
U32 CObjectDictionary::GetLightSize() {
    return 0;
}

U32 CObjectDictionary::GetSplineSize() {
    return m_spline_entries.m_capacity * 8; // sizeof(CSpline)?
}

/// Stubbed.
///
/// @return 0
U32 CObjectDictionary::GetParticleEmitterDefinitionSize() {
    return 0;
}

U32 CObjectDictionary::GetSize() {
    return GetAtomicSize() + GetClumpSize() + GetGeometrySize() + GetCameraSize() + GetLightSize() + GetSplineSize() + GetParticleEmitterDefinitionSize();
}

U32 CObjectDictionary::GetVectorSize() {
    return (m_clump_entries.m_capacity + m_spline_entries.m_capacity + m_particle_emitter_definitions.m_capacity) * sizeof(unsigned long);
}

void* CObjectDictionary::FindResource(std::string name) {
    CClump* found_clump = FindClump(name);
    if (found_clump != NULL) {
        return found_clump;
    }

    CAtomic* found_atomic = FindAtomic(name);
    if (found_atomic != NULL) {
        return found_atomic;
    }

    return NULL;
}

/// Stubbed.
///
/// @return NULL
CAtomic* CObjectDictionary::LoadAtomic(std::string name, IRWStream* stream) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
CAtomic* CObjectDictionary::LoadAtomicFromFile(std::string name, char* filename) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
CAtomic* CObjectDictionary::CopyAtomic(std::string, std::string) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
CAtomic* CObjectDictionary::CreateAtomic(std::string name) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
CAtomic* CObjectDictionary::FindAtomic(std::string name) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
std::string* CObjectDictionary::FindAtomicID(IAtomic* atomic) {
    return NULL;
}

/// Stubbed.
///
/// @return 0
U32 CObjectDictionary::GetNumberOfAtomics() {
    return 0;
}

/// Stubbed.
void CObjectDictionary::RemoveAtomic(std::string name) {

}

/// Stubbed.
void CObjectDictionary::RemoveAtomic(IAtomic* atomic) {

}

/// Stubbed.
void CObjectDictionary::RemoveAllAtomics() {

}

void CObjectDictionary::AddClump(CClump* clump, std::string name) {
    for (int i = 0; i < (int)m_clump_entries.size(); i++) {
        SCLUMPENTRY& entry = m_clump_entries[i];
        if (entry.clump == NULL) {
            entry.clump = clump;
            entry.name.assign(name, 0);
            return;
        }
    }

    SCLUMPENTRY new_entry;
    new_entry.clump = clump;
    new_entry.name.assign(name, 0);
    m_clump_entries.push_back(new_entry);
}

CClump* CObjectDictionary::LoadClump(std::string name, IRWStream* stream_, int a3) {
    if (stream_ == NULL) {
        return NULL;
    }
    CRWStream* stream = (CRWStream*)stream_;

    CDKW_Clump* wrap_clump = stream->ReadClump();
    if (wrap_clump == NULL) {
        return NULL;
    }

    CClump* clump = new CClump(wrap_clump, a3);
    AddClump(clump, name);
    return clump;
}

CClump* CObjectDictionary::LoadClumpFromFile(std::string name, char* filename, int a3) {
    if (filename == NULL) {
        return NULL;
    }

    CRWStream* stream = m_engine->OpenStream(rwSTREAMFILENAME, rwSTREAMREAD, filename);
    CClump* ret = NULL;
    if (stream != NULL) {
        if (stream->FindChunk(16, 0, 0)) {
            ret = LoadClump(name, stream, a3);
        }
        m_engine->DestroyStream(stream);
    }

    return ret;
}

/// Stubbed.
///
/// @return NULL
CClump* CObjectDictionary::CopyClump(std::string, std::string) {
    return NULL;
}

CClump* CObjectDictionary::CreateClump(std::string name) {
    CDKW_Clump::ms_NbObject++;
    CDKW_Clump* wrap_clump = new (RwEngineInstance->unk140(CDKW_Clump::ms_pMemEntry)) CDKW_Clump();

    CClump* clump = new CClump(wrap_clump);
    AddClump(clump, name);
    return clump;
}

BOOL CObjectDictionary::WriteClump(IClump* clump_, char* filename) {
    if (clump_ == NULL) {
        return FALSE;
    }
    CClump* clump = (CClump*)clump_;

    if (clump->IsReoriented() == TRUE) {
        RwFrame* rw_frame = clump->m_wrap_clump->GetRwFrame();
        CDKW_Frame* wrap_frame;
        if (rw_frame != NULL) {
            wrap_frame = CDKW_Frame::GetInstance(rw_frame);
        } else {
            wrap_frame = NULL;
        }
        RwFrameRemoveChild(wrap_frame->m_rwframe);
    }

    // Inline?
    CDKW_Clump* wrap_clump = clump->m_wrap_clump;
    CRWStream* stream = CEngine::ms_pEngine->OpenStreamFileName(rwSTREAMWRITE, filename);
    if (stream == NULL) {
        return FALSE;
    }

    BOOL ret = stream->WriteClump(wrap_clump);
    if (clump->IsReoriented() == TRUE) {
        RwFrame* rw_frame = clump->m_wrap_clump->GetRwFrame();
        CDKW_Frame* wrap_frame;
        if (rw_frame != NULL) {
            wrap_frame = CDKW_Frame::GetInstance(rw_frame);
        } else {
            wrap_frame = NULL;
        }
        RwFrameAddChild(clump->GetFrame()->m_rwframe, wrap_frame->m_rwframe);
    }
    CEngine::ms_pEngine->DestroyStream(stream);

    return ret;
}

BOOL CObjectDictionary::WriteClump(IClump* clump_, IRWStream* stream_) {
    if (clump_ == NULL) {
        return FALSE;
    }
    CClump* clump = (CClump*)clump_;

    if (clump->IsReoriented() == TRUE) {
        RwFrame* rw_frame = clump->m_wrap_clump->GetRwFrame();
        CDKW_Frame* wrap_frame;
        if (rw_frame != NULL) {
            wrap_frame = CDKW_Frame::GetInstance(rw_frame);
        } else {
            wrap_frame = NULL;
        }
        RwFrameRemoveChild(wrap_frame->m_rwframe);
    }

    CRWStream* stream = (CRWStream*)stream_;
    BOOL ret = stream->WriteClump(clump->m_wrap_clump);
    if (clump->IsReoriented() == TRUE) {
        RwFrame* rw_frame = clump->m_wrap_clump->GetRwFrame();
        CDKW_Frame* wrap_frame;
        if (rw_frame != NULL) {
            wrap_frame = CDKW_Frame::GetInstance(rw_frame);
        } else {
            wrap_frame = NULL;
        }
        RwFrameAddChild(clump->GetFrame()->m_rwframe, wrap_frame->m_rwframe);
    }

    return ret;
}

CClump* CObjectDictionary::FindClump(std::string name) {
    for (int i = 0; i < (int)m_clump_entries.size(); i++) {
        SCLUMPENTRY& entry = m_clump_entries[i];
        if (entry.name == name) {
            return entry.clump;
        }
    }

    return NULL;
}

std::string* CObjectDictionary::FindClumpID(IClump* clump) {
    if (clump == NULL) {
        return NULL;
    }

    for (int i = 0; i < (int)m_clump_entries.size(); i++) {
        SCLUMPENTRY& entry = m_clump_entries[i];
        if (entry.clump == clump) {
            return &entry.name;
        }
    }

    return NULL;
}

U32 CObjectDictionary::GetNumberOfClumps() {
    return m_clump_entries.size();
}

// Incomplete
void CObjectDictionary::RemoveClump(std::string name) {
    for (int i = 0; i < (int)m_clump_entries.size(); i++) {
        SCLUMPENTRY& entry = m_clump_entries[i];
        if (entry.name == name) {
            if (entry.clump != NULL) {
                entry.clump->Release();
                delete entry.clump;
            }
            entry.clump = NULL;
            entry.name.reserve(0);
            break;
        }
    }

    while (m_clump_entries.size() != 0 && m_clump_entries[m_clump_entries.size() - 1].clump == NULL) {
        int size = --m_clump_entries.m_size;
        SCLUMPENTRY* what = &m_clump_entries[size];
        if (what != NULL) {
            what->name.~basic_string();
        }
    }

    if (m_clump_entries.size() == 0) {
        m_clump_entries.clear();
    }
}

// Incomplete
void CObjectDictionary::RemoveClump(IClump* clump) {
    int i;
    for (i = 0; i < (int)m_clump_entries.size(); i++) {
        SCLUMPENTRY& entry = m_clump_entries[i];
        if (entry.clump == clump) {
            if (entry.clump != NULL) {
                entry.clump->Release();
                delete entry.clump;
            }
            entry.clump = NULL;
            entry.name.reserve(0);
            break;
        }
    }

    while (m_clump_entries.size() != 0 && m_clump_entries[m_clump_entries.size() - 1].clump == NULL) {
        int size = --m_clump_entries.m_size;
        SCLUMPENTRY* what = &m_clump_entries[size];
        if (what != NULL) {
            what->name.~basic_string();
        }
    }

    if (m_clump_entries.size() == 0) {
        m_clump_entries.clear();
    }
}

void CObjectDictionary::RemoveAllClumps() {
    for (int i = 0; i < (int)m_clump_entries.size(); i++) {
        SCLUMPENTRY& entry = m_clump_entries[i];
        if (entry.clump != NULL) {
            entry.clump->Release();
            delete entry.clump;
        }
        entry.clump = NULL;
        entry.name.reserve(0);
    }

    m_clump_entries.clear();
}

void CObjectDictionary::AddSpline(CSpline* spline, std::string name) {
    for (int i = 0; i < (int)m_spline_entries.size(); i++) {
        SSPLINEENTRY& entry = m_spline_entries[i];
        if (entry.spline == NULL) {
            entry.spline = spline;
            entry.name.assign(name, 0);
            return;
        }
    }

    SSPLINEENTRY new_entry;
    new_entry.spline = spline;
    new_entry.name.assign(name, 0);
    m_spline_entries.push_back(new_entry);
}

CSpline* CObjectDictionary::LoadSpline(std::string name, IRWStream* stream_) {
    if (stream_ == NULL) {
        return NULL;
    }
    CRWStream* stream = (CRWStream*)stream_;

    CDKW_Spline* wrap_spline = stream->ReadSpline();
    if (wrap_spline == NULL) {
        return NULL;
    }

    CSpline* spline = new CSpline(wrap_spline);
    AddSpline(spline, name);
    return spline;
}

CSpline* CObjectDictionary::LoadSplineFromFile(std::string name, char* filename) {
    if (filename == NULL) {
        return NULL;
    }

    CRWStream* stream = m_engine->OpenStream(rwSTREAMFILENAME, rwSTREAMREAD, filename);
    CSpline* ret = NULL;
    if (stream != NULL) {
        if (stream->FindChunk(12, 0, 0)) {
            ret = LoadSpline(name, stream);
        }
        m_engine->DestroyStream(stream);
    }

    return ret;
}

/// Stubbed.
///
/// @return NULL
CSpline* CObjectDictionary::CopySpline(std::string, std::string) {
    return NULL;
}

CSpline* CObjectDictionary::CreateSpline(std::string name, U32 a2) {
    CDKW_Spline* wrap_spline = new (RwEngineInstance->unk140(CDKW_Spline::ms_pMemEntry)) CDKW_Spline(a2);

    CSpline* spline = new CSpline(wrap_spline);
    AddSpline(spline, name);
    return (CSpline*)spline;
}

BOOL CObjectDictionary::WriteSpline(ISpline* spline_, char* filename) {
    if (spline_ == NULL) {
        return FALSE;
    }
    CSpline* spline = (CSpline*)spline_;

    CDKW_Spline* wrap_spline = spline->m_wrap_spline;
    CRWStream* stream = CEngine::ms_pEngine->OpenStreamFileName(rwSTREAMWRITE, filename);
    if (stream == NULL) {
        return FALSE;
    }

    BOOL ret = stream->WriteSpline(wrap_spline);
    CEngine::ms_pEngine->DestroyStream(stream);
    return ret;
}

BOOL CObjectDictionary::WriteSpline(ISpline* spline_, IRWStream* stream_) {
    if (spline_ == NULL) {
        return FALSE;
    }
    CSpline* spline = (CSpline*)spline_;
    CRWStream* stream = (CRWStream*)stream_;

    return stream->WriteSpline(spline->m_wrap_spline);
}

CSpline* CObjectDictionary::FindSpline(std::string name) {
    for (int i = 0; i < (int)m_spline_entries.size(); i++) {
        SSPLINEENTRY& entry = m_spline_entries[i];
        if (entry.name == name) {
            return entry.spline;
        }
    }

    return NULL;
}

std::string* CObjectDictionary::FindSplineID(ISpline* spline) {
    if (spline == NULL) {
        return NULL;
    }

    for (int i = 0; i < (int)m_spline_entries.size(); i++) {
        SSPLINEENTRY& entry = m_spline_entries[i];
        if (entry.spline == spline) {
            return &entry.name;
        }
    }

    return NULL;
}

U32 CObjectDictionary::GetNumberOfSplines() {
    return m_spline_entries.size();
}

// Incomplete
void CObjectDictionary::RemoveSpline(std::string name) {
    for (int i = 0; i < (int)m_spline_entries.size(); i++) {
        SSPLINEENTRY& entry = m_spline_entries[i];
        if (entry.name == name) {
            if (entry.spline != NULL) {
                delete entry.spline;
            }
            entry.spline = NULL;
            entry.name.reserve(0);
            break;
        }
    }

    while (m_spline_entries.size() != 0 && m_spline_entries[m_spline_entries.size() - 1].spline == NULL) {
        int size = --m_spline_entries.m_size;
        SSPLINEENTRY* what = &m_spline_entries[size];
        if (what != NULL) {
            what->name.~basic_string();
        }
    }

    if (m_spline_entries.size() == 0) {
        m_spline_entries.clear();
    }
}

// Incomplete
void CObjectDictionary::RemoveSpline(ISpline* spline) {
    int i;
    for (i = 0; i < (int)m_spline_entries.size(); i++) {
        SSPLINEENTRY& entry = m_spline_entries[i];
        if (entry.spline == spline) {
            if (entry.spline != NULL) {
                delete entry.spline;
            }
            entry.spline = NULL;
            entry.name.reserve(0);
            break;
        }
    }

    while (m_spline_entries.size() != 0 && m_spline_entries[m_spline_entries.size() - 1].spline == NULL) {
        int size = --m_spline_entries.m_size;
        SSPLINEENTRY* what = &m_spline_entries[size];
        if (what != NULL) {
            what->name.~basic_string();
        }
    }

    if (m_spline_entries.size() == 0) {
        m_spline_entries.clear();
    }
}

void CObjectDictionary::RemoveAllSplines() {
    for (int i = 0; i < (int)m_spline_entries.size(); i++) {
        SSPLINEENTRY& entry = m_spline_entries[i];
        if (entry.spline != NULL) {
            entry.spline->Release();
            delete entry.spline;
        }
        entry.spline = NULL;
        entry.name.reserve(0);
    }

    m_spline_entries.clear();
}

// Equivalent: stack offsets
void CObjectDictionary::AddParticleEmitterDefinition(CParticleEmitterDefinition* ped, std::string name) {
    unsigned long ped_ul = reinterpret_cast<unsigned long>(ped);
    if (ped_ul != NULL) {
        reinterpret_cast<CParticleEmitterDefinition*>(ped_ul)->SetName(name);

        for (int i = 0; i < (int)AS_ULONG_VECTOR_HACK(m_particle_emitter_definitions).size(); i++) {
            if (AS_ULONG_VECTOR_HACK(m_particle_emitter_definitions)[i] == NULL) {
                AS_ULONG_VECTOR_HACK(m_particle_emitter_definitions)[i] = ped_ul;
                return;
            }
        }

        AS_ULONG_VECTOR_HACK(m_particle_emitter_definitions).push_back(ped_ul);
    }
}

CParticleEmitterDefinition* CObjectDictionary::LoadParticleEmitterDefinition(std::string name, IRWStream* stream) {
    if (stream == NULL) {
        return NULL;
    }

    CParticleEmitterDefinition* ped = new CParticleEmitterDefinition;
    if (!ped->Load(stream)) {
        delete ped;
        return NULL;
    }

    AddParticleEmitterDefinition(ped, name);
    return ped;
}

CParticleEmitterDefinition* CObjectDictionary::LoadParticleEmitterDefinitionFromFile(std::string name, char* filename) {
    if (filename == NULL) {
        return NULL;
    }

    CRWStream* stream = m_engine->OpenStream(rwSTREAMFILENAME, rwSTREAMREAD, filename);
    CParticleEmitterDefinition* ret = NULL;
    if (stream != NULL) {
        ret = LoadParticleEmitterDefinition(name, stream);
        m_engine->DestroyStream(stream);
    }

    return ret;
}

CParticleEmitterDefinition* CObjectDictionary::FindParticleEmitterDefinition(std::string name) {
    for (int i = 0; i < (int)m_particle_emitter_definitions.size(); i++) {
        CParticleEmitterDefinition* ped = m_particle_emitter_definitions[i];
        if (ped != NULL) {
            if (*ped->GetName() == name) {
                return ped;
            }
        }
    }
    return NULL;
}

std::string* CObjectDictionary::FindParticleEmitterDefinitionID(IParticleEmitterDefinition* ped) {
    if (ped == NULL) {
        return NULL;
    }

    for (int i = 0; i < (int)m_particle_emitter_definitions.size(); i++) {
        if (m_particle_emitter_definitions[i] == ped) {
            return m_particle_emitter_definitions[i]->GetName();
        }
    }
    return NULL;
}

U32 CObjectDictionary::GetNumberOfParticleEmitterDefinitions() {
    return m_particle_emitter_definitions.size();
}

void CObjectDictionary::RemoveAllParticleEmitterDefinitions() {
    for (int i = 0; i < (int)m_particle_emitter_definitions.size(); i++) {
        CParticleEmitterDefinition* ped = m_particle_emitter_definitions[i];
        if (ped != NULL) {
            ped->Release();
            delete ped;
        }
        m_particle_emitter_definitions[i] = NULL;
    }

    m_particle_emitter_definitions.m_size = 0;
    if (m_particle_emitter_definitions.m_data != NULL) {
        delete m_particle_emitter_definitions.m_data;
    }
    m_particle_emitter_definitions.m_data = NULL;
    m_particle_emitter_definitions.m_capacity = 0;
}

/// Stubbed.
///
/// @return NULL
CGeometry* CObjectDictionary::LoadGeometry(std::string name, IRWStream* stream) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
CGeometry* CObjectDictionary::CopyGeometry(std::string, std::string) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
CGeometry* CObjectDictionary::CreateGeometry(std::string name) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
CGeometry* CObjectDictionary::FindGeometry(std::string name) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
std::string* CObjectDictionary::FindGeometryID(IGeometry* geometry) {
    return NULL;
}

/// Stubbed.
///
/// @return 0
U32 CObjectDictionary::GetNumberOfGeometry() {
    return 0;
}

/// Stubbed.
void CObjectDictionary::RemoveGeometry(std::string name) {

}

/// Stubbed.
void CObjectDictionary::RemoveGeometry(IGeometry* geometry) {

}

/// Stubbed.
void CObjectDictionary::RemoveAllGeometries() {

}

/// Stubbed.
///
/// @return NULL
CCamera* CObjectDictionary::LoadCamera(std::string name, IRWStream* stream) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
CCamera* CObjectDictionary::CopyCamera(std::string, std::string) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
CCamera* CObjectDictionary::CreateCamera(std::string name) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
CCamera* CObjectDictionary::FindCamera(std::string name) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
std::string* CObjectDictionary::FindCameraID(ICamera* camera) {
    return NULL;
}

/// Stubbed.
///
/// @return 0
U32 CObjectDictionary::GetNumberOfCamera() {
    return 0;
}

/// Stubbed.
void CObjectDictionary::RemoveCamera(std::string name) {

}

/// Stubbed.
void CObjectDictionary::RemoveAllCamera() {

}

/// Stubbed.
///
/// @return NULL
CLight* CObjectDictionary::LoadLight(std::string name, IRWStream* stream) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
CLight* CObjectDictionary::CopyLight(std::string, std::string) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
CLight* CObjectDictionary::CreateLight(std::string name) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
CLight* CObjectDictionary::FindLight(std::string name) {
    return NULL;
}

/// Stubbed.
///
/// @return NULL
std::string* CObjectDictionary::FindLightID(ILight* light) {
    return NULL;
}

/// Stubbed.
///
/// @return 0
U32 CObjectDictionary::GetNumberOfLights() {
    return 0;
}

/// Stubbed.
void CObjectDictionary::RemoveLight(std::string name) {

}

/// Stubbed.
void CObjectDictionary::RemoveAllLights() {

}

}
