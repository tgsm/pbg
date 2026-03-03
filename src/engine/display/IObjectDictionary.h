#ifndef ENGINE_DISPLAY_IOBJECTDICTIONARY_H
#define ENGINE_DISPLAY_IOBJECTDICTIONARY_H

#include <string>
#include "engine/display/CAtomic.h"
#include "engine/display/CCamera.h"
#include "engine/display/CClump.h"
#include "engine/display/CLight.h"
#include "engine/display/CParticleEmitterDefinition.h"
#include "engine/display/CRWStream.h"
#include "engine/display/CSpline.h"

namespace DKDSP {

class IObjectDictionary {
public:
    IObjectDictionary();

    virtual void Release() = 0;
    virtual void* FindResource(std::string name) = 0;
    virtual U32 GetAtomicSize() = 0;
    virtual U32 GetClumpSize() = 0;
    virtual U32 GetGeometrySize() = 0;
    virtual U32 GetCameraSize() = 0;
    virtual U32 GetLightSize() = 0;
    virtual U32 GetSplineSize() = 0;
    virtual U32 GetParticleEmitterDefinitionSize() = 0;
    virtual U32 GetSize() = 0;
    virtual U32 GetVectorSize() = 0;
    virtual CAtomic* LoadAtomic(std::string name, IRWStream* stream) = 0;
    virtual CAtomic* LoadAtomicFromFile(std::string name, char* filename) = 0;
    virtual CAtomic* CopyAtomic(std::string, std::string) = 0;
    virtual CAtomic* CreateAtomic(std::string name) = 0;
    virtual CAtomic* FindAtomic(std::string name) = 0;
    virtual std::string* FindAtomicID(IAtomic* atomic) = 0;
    virtual U32 GetNumberOfAtomics() = 0;
    virtual void RemoveAtomic(std::string name) = 0;
    virtual void RemoveAtomic(IAtomic* atomic) = 0;
    virtual void RemoveAllAtomics() = 0;
    virtual CClump* LoadClump(std::string name, IRWStream* stream, int) = 0;
    virtual CClump* LoadClumpFromFile(std::string name, char* filename, int) = 0;
    virtual CClump* CopyClump(std::string, std::string) = 0;
    virtual CClump* CreateClump(std::string name) = 0;
    virtual BOOL WriteClump(IClump* clump, char* filename) = 0;
    virtual BOOL WriteClump(IClump* clump, IRWStream* stream) = 0;
    virtual CClump* FindClump(std::string name) = 0;
    virtual std::string* FindClumpID(IClump* clump) = 0;
    virtual U32 GetNumberOfClumps() = 0;
    virtual void RemoveClump(std::string name) = 0;
    virtual void RemoveClump(IClump* clump) = 0;
    virtual void RemoveAllClumps() = 0;
    virtual CSpline* LoadSpline(std::string name, IRWStream* stream) = 0;
    virtual CSpline* LoadSplineFromFile(std::string name, char* filename) = 0;
    virtual CSpline* CopySpline(std::string, std::string) = 0;
    virtual CSpline* CreateSpline(std::string name, U32) = 0;
    virtual BOOL WriteSpline(ISpline* spline, char* filename) = 0;
    virtual BOOL WriteSpline(ISpline* spline, IRWStream* stream) = 0;
    virtual CSpline* FindSpline(std::string name) = 0;
    virtual std::string* FindSplineID(ISpline* spline) = 0;
    virtual U32 GetNumberOfSplines() = 0;
    virtual void RemoveSpline(std::string name) = 0;
    virtual void RemoveSpline(ISpline* spline) = 0;
    virtual void RemoveAllSplines() = 0;
    virtual CParticleEmitterDefinition* LoadParticleEmitterDefinition(std::string name, IRWStream* stream) = 0;
    virtual CParticleEmitterDefinition* LoadParticleEmitterDefinitionFromFile(std::string name, char* filename) = 0;
    virtual CParticleEmitterDefinition* FindParticleEmitterDefinition(std::string name) = 0;
    virtual std::string* FindParticleEmitterDefinitionID(IParticleEmitterDefinition* ped) = 0;
    virtual U32 GetNumberOfParticleEmitterDefinitions() = 0;
    virtual void RemoveParticleEmitterDefinition(std::string name) = 0;
    virtual void RemoveParticleEmitterDefinition(IParticleEmitterDefinition* ped) = 0;
    virtual void RemoveAllParticleEmitterDefinitions() = 0;
    virtual CGeometry* LoadGeometry(std::string name, IRWStream* stream) = 0;
    virtual CGeometry* CopyGeometry(std::string, std::string) = 0;
    virtual CGeometry* CreateGeometry(std::string name) = 0;
    virtual CGeometry* FindGeometry(std::string name) = 0;
    virtual std::string* FindGeometryID(IGeometry* geometry) = 0;
    virtual U32 GetNumberOfGeometry() = 0;
    virtual void RemoveGeometry(std::string name) = 0;
    virtual void RemoveGeometry(IGeometry* geometry) = 0;
    virtual void RemoveAllGeometries() = 0;
    virtual CCamera* LoadCamera(std::string name, IRWStream* stream) = 0;
    virtual CCamera* CopyCamera(std::string, std::string) = 0;
    virtual CCamera* CreateCamera(std::string name) = 0;
    virtual CCamera* FindCamera(std::string name) = 0;
    virtual std::string* FindCameraID(ICamera* camera) = 0;
    virtual U32 GetNumberOfCamera() = 0;
    virtual void RemoveCamera(std::string name) = 0;
    virtual void RemoveAllCamera() = 0;
    virtual CLight* LoadLight(std::string name, IRWStream* stream) = 0;
    virtual CLight* CopyLight(std::string, std::string) = 0;
    virtual CLight* CreateLight(std::string name) = 0;
    virtual CLight* FindLight(std::string name) = 0;
    virtual std::string* FindLightID(ILight* light) = 0;
    virtual U32 GetNumberOfLights() = 0;
    virtual void RemoveLight(std::string name) = 0;
    virtual void RemoveAllLights() = 0;
};

}

#endif
