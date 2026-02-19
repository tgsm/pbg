#ifndef ENGINE_DISPLAY_CANIMDICTIONARY_H
#define ENGINE_DISPLAY_CANIMDICTIONARY_H

#include <vector>
#include "engine/display/IAnimDictionary.h"

namespace DKDSP {

class CEngine;

// TODO
class CAnimDictionary : public IAnimDictionary {
public:
    CEngine* m_engine;
    std::vector<CAnimation*> m_unk8;
    std::vector<CDMorphAnimation*> m_unk14;
    std::vector<CMaterialAnimation*> m_unk20;
    std::vector<CEventAnimation*> m_unk2C;
    std::vector<CController*> m_unk38;
    std::vector<CAnimationStar*> m_unk44;
    std::vector<CAnimationStarController*> m_unk50;

public:
    CAnimDictionary() {
        DONT_INLINE_HACK();

        m_engine = NULL;
        AS_ULONG_VECTOR_HACK(m_unk8).reserve(16);
        AS_ULONG_VECTOR_HACK(m_unk14).reserve(16);
        AS_ULONG_VECTOR_HACK(m_unk20).reserve(16);
        AS_ULONG_VECTOR_HACK(m_unk2C).reserve(16);
        AS_ULONG_VECTOR_HACK(m_unk38).reserve(16);
        AS_ULONG_VECTOR_HACK(m_unk44).reserve(16);
        AS_ULONG_VECTOR_HACK(m_unk50).reserve(16);
    }
    ~CAnimDictionary();

    virtual void Release();
    virtual U32 GetAnimationSize();
    virtual U32 GetDMorphAnimationSize();
    virtual U32 GetMaterialAnimationSize();
    virtual U32 GetEventAnimationSize();
    virtual U32 GetControllerSize();
    virtual U32 GetAnimationStarSize();
    virtual U32 GetAnimationStarControllerSize();
    virtual U32 GetSize();
    virtual U32 GetVectorSize();
    virtual CAnimation* LoadAnimation(std::string, IRWStream*);
    virtual CAnimation* LoadAnimationFromFile(std::string, char*);
    virtual CAnimation* LoadSmoothAnimation(std::string, IRWStream*);
    virtual CAnimation* LoadSmoothAnimationFromFile(std::string, char*);
    virtual BOOL WriteAnimation(IAnimation*, char*);
    virtual BOOL WriteAnimation(IAnimation*, IRWStream*);
    virtual CAnimation* FindAnimation(std::string name);
    virtual std::string* FindAnimationID(IAnimation*); // Skeptical about the return type. Same for other Find[...]ID functions
    virtual U32 GetNumberOfAnimations();
    virtual CAnimation* GetAnimation(int);
    virtual void RemoveAnimation(std::string);
    virtual void RemoveAnimation(IAnimation*);
    virtual void RemoveAnimation(int);
    virtual void RemoveAllAnimations();
    virtual CDMorphAnimation* LoadDMorphAnimation(std::string, IRWStream*);
    virtual CDMorphAnimation* LoadDMorphAnimationFromFile(std::string, char*);
    virtual CDMorphAnimation* FindDMorphAnimation(std::string name);
    virtual std::string* FindDMorphAnimationID(IDMorphAnimation*);
    virtual U32 GetNumberOfDMorphAnimations();
    virtual CDMorphAnimation* GetDMorphAnimation(int);
    virtual void RemoveDMorphAnimation(std::string);
    virtual void RemoveDMorphAnimation(IDMorphAnimation*);
    virtual void RemoveDMorphAnimation(int);
    virtual void RemoveAllDMorphAnimations();
    virtual CMaterialAnimation* LoadMaterialAnimation(std::string, IRWStream*);
    virtual CMaterialAnimation* LoadMaterialAnimationFromFile(std::string, char*);
    virtual CMaterialAnimation* LoadMaterialAnimationFromChunk(std::string, DkXmd::CChunkIterator*);
    virtual CMaterialAnimation* CreateMaterialAnimation(std::string, int nb_keyframes, F32 duration);
    virtual CMaterialAnimation* FindMaterialAnimation(std::string);
    virtual std::string* FindMaterialAnimationID(IMaterialAnimation*);
    virtual U32 GetNumberOfMaterialAnimations();
    virtual CMaterialAnimation* GetMaterialAnimation(int);
    virtual void RemoveMaterialAnimation(std::string);
    virtual void RemoveMaterialAnimation(IMaterialAnimation*);
    virtual void RemoveMaterialAnimation(int);
    virtual void RemoveAllMaterialAnimations();
    virtual CEventAnimation* LoadEventAnimation(std::string, IRWStream*);
    virtual CEventAnimation* LoadEventAnimationFromFile(std::string, char*);
    virtual CEventAnimation* LoadEventAnimationFromChunk(std::string, DkXmd::CChunkIterator*, DkXmd::CXmdFile*, void*);
    virtual CEventAnimation* CreateEventAnimation(std::string, int nb_keyframes, F32 duration, DkXmd::CXmdFile*);
    virtual CEventAnimation* FindEventAnimation(std::string);
    virtual std::string* FindEventAnimationID(IEventAnimation*);
    virtual U32 GetNumberOfEventAnimations();
    virtual CEventAnimation* GetEventAnimation(int);
    virtual void RemoveEventAnimation(std::string);
    virtual void RemoveEventAnimation(IEventAnimation*);
    virtual void RemoveEventAnimation(int);
    virtual void RemoveAllEventAnimations();
    virtual CController* CreateController(std::string, IClump*, int);
    virtual CController* FindController(std::string name);
    virtual std::string* FindControllerID(IController*);
    virtual U32 GetNumberOfControllers();
    virtual CController* GetController(int);
    virtual void RemoveController(std::string);
    virtual void RemoveController(IController*);
    virtual void RemoveController(int);
    virtual void RemoveAllControllers();
    virtual CAnimationStar* LoadAnimationStar(std::string, IRWStream*);
    virtual CAnimationStar* LoadAnimationStarFromFile(std::string, char*);
    virtual CAnimationStar* LoadAnimationStarFromChunk(std::string, DkXmd::CChunkIterator*, DkXmd::CXmdFile*, void*);
    virtual CAnimationStar* FindAnimationStar(std::string);
    virtual std::string* FindAnimationStarID(IAnimationStar*);
    virtual U32 GetNumberOfAnimationStars();
    virtual CAnimationStar* GetAnimationStar(int);
    virtual void RemoveAnimationStar(std::string);
    virtual void RemoveAnimationStar(IAnimationStar*);
    virtual void RemoveAnimationStar(int);
    virtual void RemoveAllAnimationStars();
    virtual CAnimationStarController* CreateAnimationStarController(std::string, IAnimationStar*, IController*);
    virtual CAnimationStarController* FindAnimationStarController(std::string);
    virtual std::string* FindAnimationStarControllerID(IAnimationStarController*);
    virtual U32 GetNumberOfAnimationStarControllers();
    virtual CAnimationStarController* GetAnimationStarController(int);
    virtual void RemoveAnimationStarController(std::string);
    virtual void RemoveAnimationStarController(IAnimationStarController*);
    virtual void RemoveAnimationStarController(int);
    virtual void RemoveAllAnimationStarControllers();
};
REQUIRE_SIZE(CAnimDictionary, 0x5C);

}

#endif
