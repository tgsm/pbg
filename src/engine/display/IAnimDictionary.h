#ifndef ENGINE_DISPLAY_IANIMDICTIONARY_H
#define ENGINE_DISPLAY_IANIMDICTIONARY_H

#include <string>
#include "engine/display/IClump.h"
#include "engine/display/CAnimation.h"
#include "engine/display/CAnimationStar.h"
#include "engine/display/CAnimationStarController.h"
#include "engine/display/CController.h"
#include "engine/display/CDMorphAnimation.h"
#include "engine/display/CEventAnimation.h"
#include "engine/display/CMaterialAnimation.h"
#include "engine/display/CRWStream.h"
#include "engine/xmd/CChunkIterator.h"

namespace DKDSP {

class IAnimDictionary {
public:
    IAnimDictionary();

    virtual void Release() = 0;
    virtual U32 GetAnimationSize() = 0;
    virtual U32 GetDMorphAnimationSize() = 0;
    virtual U32 GetMaterialAnimationSize() = 0;
    virtual U32 GetEventAnimationSize() = 0;
    virtual U32 GetControllerSize() = 0;
    virtual U32 GetAnimationStarSize() = 0;
    virtual U32 GetAnimationStarControllerSize() = 0;
    virtual U32 GetSize() = 0;
    virtual U32 GetVectorSize() = 0;
    virtual CAnimation* LoadAnimation(std::string, IRWStream*) = 0;
    virtual CAnimation* LoadAnimationFromFile(std::string, char*) = 0;
    virtual CAnimation* LoadSmoothAnimation(std::string, IRWStream*) = 0;
    virtual CAnimation* LoadSmoothAnimationFromFile(std::string, char*) = 0;
    virtual BOOL WriteAnimation(IAnimation*, char*) = 0;
    virtual BOOL WriteAnimation(IAnimation*, IRWStream*) = 0;
    virtual CAnimation* FindAnimation(std::string name) = 0;
    virtual std::string* FindAnimationID(IAnimation*) = 0;
    virtual int GetNumberOfAnimations() = 0;
    virtual CAnimation* GetAnimation(int) = 0;
    virtual void RemoveAnimation(std::string) = 0;
    virtual void RemoveAnimation(IAnimation*) = 0;
    virtual void RemoveAnimation(int) = 0;
    virtual void RemoveAllAnimations() = 0;
    virtual CDMorphAnimation* LoadDMorphAnimation(std::string, IRWStream*) = 0;
    virtual CDMorphAnimation* LoadDMorphAnimationFromFile(std::string, char*) = 0;
    virtual CDMorphAnimation* FindDMorphAnimation(std::string name) = 0;
    virtual std::string* FindDMorphAnimationID(IDMorphAnimation*) = 0;
    virtual int GetNumberOfDMorphAnimations() = 0;
    virtual CDMorphAnimation* GetDMorphAnimation(int) = 0;
    virtual void RemoveDMorphAnimation(std::string) = 0;
    virtual void RemoveDMorphAnimation(IDMorphAnimation*) = 0;
    virtual void RemoveDMorphAnimation(int) = 0;
    virtual void RemoveAllDMorphAnimations() = 0;
    virtual CMaterialAnimation* LoadMaterialAnimation(std::string, IRWStream*) = 0;
    virtual CMaterialAnimation* LoadMaterialAnimationFromFile(std::string, char*) = 0;
    virtual CMaterialAnimation* LoadMaterialAnimationFromChunk(std::string, DkXmd::CChunkIterator*) = 0;
    virtual CMaterialAnimation* CreateMaterialAnimation(std::string, int nb_keyframes, F32 duration) = 0;
    virtual CMaterialAnimation* FindMaterialAnimation(std::string) = 0;
    virtual std::string* FindMaterialAnimationID(IMaterialAnimation*) = 0;
    virtual int GetNumberOfMaterialAnimations() = 0;
    virtual CMaterialAnimation* GetMaterialAnimation(int) = 0;
    virtual void RemoveMaterialAnimation(std::string) = 0;
    virtual void RemoveMaterialAnimation(IMaterialAnimation*) = 0;
    virtual void RemoveMaterialAnimation(int) = 0;
    virtual void RemoveAllMaterialAnimations() = 0;
    virtual CEventAnimation* LoadEventAnimation(std::string, IRWStream*) = 0;
    virtual CEventAnimation* LoadEventAnimationFromFile(std::string, char*) = 0;
    virtual CEventAnimation* LoadEventAnimationFromChunk(std::string, DkXmd::CChunkIterator*, DkXmd::CXmdFile*, void*) = 0;
    virtual CEventAnimation* CreateEventAnimation(std::string, int nb_keyframes, F32 duration, DkXmd::CXmdFile*) = 0;
    virtual CEventAnimation* FindEventAnimation(std::string) = 0;
    virtual std::string* FindEventAnimationID(IEventAnimation*) = 0;
    virtual int GetNumberOfEventAnimations() = 0;
    virtual CEventAnimation* GetEventAnimation(int) = 0;
    virtual void RemoveEventAnimation(std::string) = 0;
    virtual void RemoveEventAnimation(IEventAnimation*) = 0;
    virtual void RemoveEventAnimation(int) = 0;
    virtual void RemoveAllEventAnimations() = 0;
    virtual CController* CreateController(std::string, IClump*, int) = 0;
    virtual CController* FindController(std::string name) = 0;
    virtual std::string* FindControllerID(IController*) = 0;
    virtual int GetNumberOfControllers() = 0;
    virtual CController* GetController(int) = 0;
    virtual void RemoveController(std::string) = 0;
    virtual void RemoveController(IController*) = 0;
    virtual void RemoveController(int) = 0;
    virtual void RemoveAllControllers() = 0;
    virtual CAnimationStar* LoadAnimationStar(std::string, IRWStream*) = 0;
    virtual CAnimationStar* LoadAnimationStarFromFile(std::string, char*) = 0;
    virtual CAnimationStar* LoadAnimationStarFromChunk(std::string, DkXmd::CChunkIterator*) = 0;
    virtual CAnimationStar* FindAnimationStar(std::string) = 0;
    virtual std::string* FindAnimationStarID(IAnimationStar*) = 0;
    virtual int GetNumberOfAnimationStars() = 0;
    virtual CAnimationStar* GetAnimationStar(int) = 0;
    virtual void RemoveAnimationStar(std::string) = 0;
    virtual void RemoveAnimationStar(IAnimationStar*) = 0;
    virtual void RemoveAnimationStar(int) = 0;
    virtual void RemoveAllAnimationStars() = 0;
    virtual CAnimationStarController* CreateAnimationStarController(std::string, IAnimationStar*, IController*) = 0;
    virtual CAnimationStarController* FindAnimationStarController(std::string) = 0;
    virtual std::string* FindAnimationStarControllerID(IAnimationStarController*) = 0;
    virtual int GetNumberOfAnimationStarControllers() = 0;
    virtual CAnimationStarController* GetAnimationStarController(int) = 0;
    virtual void RemoveAnimationStarController(std::string) = 0;
    virtual void RemoveAnimationStarController(IAnimationStarController*) = 0;
    virtual void RemoveAnimationStarController(int) = 0;
    virtual void RemoveAllAnimationStarControllers() = 0;
};

}

#endif
