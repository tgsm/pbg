#ifndef ENGINE_DISPLAY_COBJECTDICTIONARY_H
#define ENGINE_DISPLAY_COBJECTDICTIONARY_H

#include <vector>
#include "engine/display/IObjectDictionary.h"

namespace DKDSP {

class CEngine;

// TODO
struct SCLUMPENTRY {};
struct SSPLINEENTRY {};

class CObjectDictionary : public IObjectDictionary {
public:
    CEngine* m_engine;
    std::vector<SCLUMPENTRY> m_clump_entries;
    std::vector<SSPLINEENTRY> m_spline_entries;
    std::vector<unsigned long> m_unk20;

public:
    CObjectDictionary() {
        m_engine = NULL;
        m_clump_entries.reserve(16);
        m_spline_entries.reserve(16);
        m_unk20.reserve(16);
    }
    ~CObjectDictionary();

    virtual void Release();
    virtual void func1();
    virtual void func2();
    virtual void func3();
    virtual void func4();
    virtual void func5();
    virtual void func6();
    virtual void func7();
    virtual void func8();
    virtual U32 GetSize();
    virtual void func10();
    virtual void func11();
    virtual void func12();
    virtual void func13();
    virtual void func14();
    virtual void func15();
    virtual void func16();
    virtual void func17();
    virtual void func18();
    virtual void func19();
    virtual void func20();
    virtual void func21();
    virtual void func22();
    virtual void func23();
    virtual void func24();
    virtual void func25();
    virtual void func26();
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
    virtual CSpline* FindSpline(std::string name);
    virtual void func40();
    virtual void func41();
    virtual void func42();
    virtual void func43();
    virtual void func44();
    virtual void func45();
    virtual void func46();
    virtual CParticleEmitterDefinition* FindParticleEmitterDefinition(std::string);
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
    virtual void func58();
    virtual void func59();
    virtual void func60();
    virtual void func61();
    virtual void func62();
    virtual void func63();
    virtual void func64();
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
};
REQUIRE_SIZE(CObjectDictionary, 0x2C);

}

#endif
