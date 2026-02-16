#ifndef ENGINE_DISPLAY_CANIMDICTIONARY_H
#define ENGINE_DISPLAY_CANIMDICTIONARY_H

#include <vector>
#include "engine/display/CAnimationStar.h"
#include "engine/display/CAnimationStarController.h"
#include "engine/display/CController.h"
#include "engine/display/CDMorphAnimation.h"
#include "engine/display/CEventAnimation.h"
#include "engine/display/IAnimDictionary.h"

namespace DKDSP {

class CEngine;
class CMaterialAnimation;

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
    virtual void func1();
    virtual void func2();
    virtual void func3();
    virtual void func4();
    virtual void func5();
    virtual void func6();
    virtual void func7();
    virtual U32 GetSize();
    virtual void func9();
    virtual void func10();
    virtual void func11();
    virtual void func12();
    virtual void func13();
    virtual void func14();
    virtual void func15();
    virtual CAnimation* FindAnimation(std::string name);
    virtual void func17();
    virtual void func18();
    virtual void func19();
    virtual void func20();
    virtual void func21();
    virtual void func22();
    virtual void func23();
    virtual void func24();
    virtual void func25();
    virtual CDMorphAnimation* FindDMorphAnimation(std::string name);
    virtual void func27();
    virtual void func28();
    virtual void func29();
    virtual void func30();
    virtual void func31();
    virtual void func32();
    virtual void func33();
    virtual void func34();
    virtual void func35();
    virtual void func36();
    virtual void func37();
    virtual void func38();
    virtual void func39();
    virtual void func40();
    virtual void func41();
    virtual void func42();
    virtual void func43();
    virtual void func44();
    virtual void func45();
    virtual void func46();
    virtual void func47();
    virtual void func48();
    virtual void func49();
    virtual void func50();
    virtual void func51();
    virtual void func52();
    virtual void func53();
    virtual void func54();
    virtual void func55();
    virtual void func56();
    virtual void func57();
    virtual CController* CreateController(std::string, IClump*, int);
    virtual void func59();
    virtual void func60();
    virtual void func61();
    virtual void func62();
    virtual void func63();
    virtual void RemoveController(IController*);
    virtual void func65();
    virtual void func66();
    virtual void func67();
    virtual void func68();
    virtual void func69();
    virtual void func70();
    virtual void func71();
    virtual void func72();
    virtual void func73();
    virtual void func74();
    virtual void func75();
    virtual void func76();
    virtual void func77();
    virtual void func78();
    virtual void func79();
    virtual void func80();
    virtual void func81();
    virtual void func82();
    virtual void func83();
    virtual void func84();
    virtual void func85();
    virtual void func86();
};
REQUIRE_SIZE(CAnimDictionary, 0x5C);

}

#endif
