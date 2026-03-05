#ifndef ENGINE_DISPLAY_COBJECTDICTIONARY_H
#define ENGINE_DISPLAY_COBJECTDICTIONARY_H

#include <vector>
#include "engine/display/IObjectDictionary.h"

namespace DKDSP {

class CEngine;

struct SCLUMPENTRY {
    CClump* clump;
    std::string name;

    SCLUMPENTRY() : name("") {

    }
};

struct SSPLINEENTRY {
    CSpline* spline;
    std::string name;

    SSPLINEENTRY() : name("") {

    }
};

class CObjectDictionary : public IObjectDictionary {
public:
    CEngine* m_engine;
    std::vector<SCLUMPENTRY> m_clump_entries;
    std::vector<SSPLINEENTRY> m_spline_entries;
    std::vector<CParticleEmitterDefinition*> m_particle_emitter_definitions;

public:
    CObjectDictionary() {
        m_engine = NULL;
        m_clump_entries.reserve(16);
        m_spline_entries.reserve(16);
        m_particle_emitter_definitions.reserve(16);
    }
    ~CObjectDictionary();

    virtual void Release();
    virtual void* FindResource(std::string name);
    virtual U32 GetAtomicSize();
    virtual U32 GetClumpSize();
    virtual U32 GetGeometrySize();
    virtual U32 GetCameraSize();
    virtual U32 GetLightSize();
    virtual U32 GetSplineSize();
    virtual U32 GetParticleEmitterDefinitionSize();
    virtual U32 GetSize();
    virtual U32 GetVectorSize();
    virtual CAtomic* LoadAtomic(std::string name, IRWStream* stream);
    virtual CAtomic* LoadAtomicFromFile(std::string name, char* filename);
    virtual CAtomic* CopyAtomic(std::string, std::string);
    virtual CAtomic* CreateAtomic(std::string name);
    virtual CAtomic* FindAtomic(std::string name);
    virtual std::string* FindAtomicID(IAtomic* atomic);
    virtual U32 GetNumberOfAtomics();
    virtual void RemoveAtomic(std::string name);
    virtual void RemoveAtomic(IAtomic* atomic);
    virtual void RemoveAllAtomics();
    virtual CClump* LoadClump(std::string name, IRWStream* stream, int);
    virtual CClump* LoadClumpFromFile(std::string name, char* filename, int);
    virtual CClump* CopyClump(std::string, std::string);
    virtual CClump* CreateClump(std::string name);
    virtual BOOL WriteClump(IClump* clump, char* filename);
    virtual BOOL WriteClump(IClump* clump, IRWStream* stream);
    virtual CClump* FindClump(std::string name);
    virtual std::string* FindClumpID(IClump* clump);
    virtual U32 GetNumberOfClumps();
    virtual void RemoveClump(std::string name);
    virtual void RemoveClump(IClump* clump);
    virtual void RemoveAllClumps();
    virtual CSpline* LoadSpline(std::string name, IRWStream* stream);
    virtual CSpline* LoadSplineFromFile(std::string name, char* filename);
    virtual CSpline* CopySpline(std::string, std::string);
    virtual CSpline* CreateSpline(std::string name, U32);
    virtual BOOL WriteSpline(ISpline* spline, char* filename);
    virtual BOOL WriteSpline(ISpline* spline, IRWStream* stream);
    virtual CSpline* FindSpline(std::string name);
    virtual std::string* FindSplineID(ISpline* spline);
    virtual U32 GetNumberOfSplines();
    virtual void RemoveSpline(std::string name);
    virtual void RemoveSpline(ISpline* spline);
    virtual void RemoveAllSplines();
    virtual CParticleEmitterDefinition* LoadParticleEmitterDefinition(std::string name, IRWStream* stream);
    virtual CParticleEmitterDefinition* LoadParticleEmitterDefinitionFromFile(std::string name, char* filename);
    virtual CParticleEmitterDefinition* FindParticleEmitterDefinition(std::string name);
    virtual std::string* FindParticleEmitterDefinitionID(IParticleEmitterDefinition* ped);
    virtual U32 GetNumberOfParticleEmitterDefinitions();
    virtual void RemoveParticleEmitterDefinition(std::string name);
    virtual void RemoveParticleEmitterDefinition(IParticleEmitterDefinition* ped);
    virtual void RemoveAllParticleEmitterDefinitions();
    virtual CGeometry* LoadGeometry(std::string name, IRWStream* stream);
    virtual CGeometry* CopyGeometry(std::string, std::string);
    virtual CGeometry* CreateGeometry(std::string name);
    virtual CGeometry* FindGeometry(std::string name);
    virtual std::string* FindGeometryID(IGeometry* geometry);
    virtual U32 GetNumberOfGeometry();
    virtual void RemoveGeometry(std::string name);
    virtual void RemoveGeometry(IGeometry* geometry);
    virtual void RemoveAllGeometries();
    virtual CCamera* LoadCamera(std::string name, IRWStream* stream);
    virtual CCamera* CopyCamera(std::string, std::string);
    virtual CCamera* CreateCamera(std::string name);
    virtual CCamera* FindCamera(std::string name);
    virtual std::string* FindCameraID(ICamera* camera);
    virtual U32 GetNumberOfCamera();
    virtual void RemoveCamera(std::string name);
    virtual void RemoveAllCamera();
    virtual CLight* LoadLight(std::string name, IRWStream* stream);
    virtual CLight* CopyLight(std::string, std::string);
    virtual CLight* CreateLight(std::string name);
    virtual CLight* FindLight(std::string name);
    virtual std::string* FindLightID(ILight* light);
    virtual U32 GetNumberOfLights();
    virtual void RemoveLight(std::string name);
    virtual void RemoveAllLights();

    void AddClump(CClump* clump, std::string name);
    void AddSpline(CSpline* spline, std::string name);
    void AddParticleEmitterDefinition(CParticleEmitterDefinition* ped, std::string name);
};
REQUIRE_SIZE(CObjectDictionary, 0x2C);

}

#endif
