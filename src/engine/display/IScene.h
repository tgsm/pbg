#ifndef ENGINE_DISPLAY_ISCENE_H
#define ENGINE_DISPLAY_ISCENE_H

#include "types.h"
#include "engine/display/CMirror.h"
#include "engine/display/ICamera.h"
#include "engine/display/IWarp.h"

namespace DKDSP {

class IObjectDictionary;

// TODO
class IScene {
public:
    virtual void func0();
    virtual void func1();
    virtual void func2();
    virtual void func3();
    virtual void func4();
    virtual void func5();
    virtual void func6();
    virtual void func7();
    virtual void func8();
    virtual void func9();
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
    virtual void func39();
    virtual void func40();
    virtual void func41();
    virtual void func42();
    virtual void func43();
    virtual void func44();
    virtual void func45();
    virtual void func46();
    virtual void func47();
    virtual CMirror* CreateMirror(U32 texture_width, U32 texture_height, U32 texture_depth_maybe) = 0;
    virtual void func49();
    virtual void func50();
    virtual void func51();
    virtual void func52();
    virtual void func53();
    virtual void func54();
    virtual void func55();
    virtual void EnableMirrorRendering(IMirror* mirror, BOOL enable) = 0;
    virtual void func57();
    virtual void func58();
    virtual void AssociateClumpToMirror(IMirror* mirror, IClump* clump) = 0;
    virtual void func60();
    virtual void func61();
    virtual void func62();
    virtual void func63();
    virtual IWarp* CreateWarp() = 0;
    virtual void func65();
    virtual void func66();
    virtual void func67();
    virtual void func68();
    virtual void RemoveWarp(IWarp*) = 0;
    virtual void func70();
    virtual void func71();
    virtual void func72();
    virtual void func73();
    virtual void RenderWarp(IWarp*) = 0;
    virtual void func75();
    virtual void func76();
    virtual CClump* CloneClump(std::string, IObjectDictionary*) = 0;
    virtual void func78();
    virtual void func79();
    virtual void func80();
    virtual void func81();
    virtual void func82();
    virtual void func83();
    virtual void func84();
    virtual void func85();
    virtual void func86();
    virtual void func87();
    virtual void func88();
    virtual void func89();
    virtual void func90();
    virtual void func91();
    virtual void func92();
    virtual void func93();
    virtual void func94();
    virtual void func95();
    virtual void func96();
    virtual void func97();
    virtual void func98();
    virtual void func99();
    virtual void func100();
    virtual void func101();
    virtual void func102();
    virtual void func103();
    virtual void func104();
    virtual void func105();
    virtual void func106();
    virtual void func107();
    virtual void func108();
    virtual void func109();
    virtual void func110();
    virtual void func111();
    virtual void func112();
    virtual void func113();
    virtual void SelectCamera(ICamera*) = 0;
    virtual void func115();
    virtual void func116();
    virtual void Clear(int, F32, F32, F32) = 0;
    virtual void BeginRender() = 0;
    virtual void func119();
    virtual void func120();
    virtual void func121();
    virtual void func122();
    virtual void func123();
    virtual void func124();
    virtual void func125();
    virtual void RenderClump(IClump* clump) = 0;
    virtual void func127();
    virtual void func128();
    virtual void func129();
    virtual void func130();
    virtual void func131();
    virtual void func132();
    virtual void func133();
    virtual void func134();
    virtual void func135();
    virtual void func136();
    virtual void Flush() = 0;
    virtual void func138();
    virtual void func139();
    virtual void func140();
    virtual void EndRender() = 0;
    virtual void Flip(int) = 0;
    virtual void func143();
};

}

#endif
