#ifndef ENGINE_DISPLAY_ISHADOWMAP_H
#define ENGINE_DISPLAY_ISHADOWMAP_H

#include "engine/display/CCamera.h"
#include "engine/display/IClump.h"

namespace DKDSP {

// TODO
class IShadowMap {
public:
    IShadowMap();

    virtual void func8();
    virtual void AddShadowEmitter(IClump* shadow_emitter) = 0;
    virtual void func10();
    virtual void func14();
    virtual void func18();
    virtual void func1C();
    virtual void func20();
    virtual void func24();
    virtual void func28();
    virtual void func2C();
    virtual void func30();
    virtual void func34();
    virtual void func38();
    virtual void func3C();
    virtual void func40();
    virtual void func44();
    virtual BOOL EnableDisplay(BOOL enable) = 0;
    virtual BOOL IsDisplayEnabled() = 0;
    virtual BOOL IsEnabled() = 0;
    virtual void SetShadowColor(F32 red, F32 green, F32 blue) = 0;
    virtual F32 GetShadowColorRed() = 0;
    virtual F32 GetShadowColorGreen() = 0;
    virtual F32 GetShadowColorBlue() = 0;
    virtual void func64();
    virtual void func68();
    virtual void SetBlurLevel(U32) = 0;
    virtual void func70();
    virtual void func74();
    virtual void func78();
    virtual void Prepare() = 0;
    virtual void Unprepare() = 0;
    virtual void func84();
    virtual CCamera* GetCamera() = 0;
    virtual void func8C();
    virtual void func90();
};

}

#endif
