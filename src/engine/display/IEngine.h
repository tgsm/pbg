#ifndef ENGINE_DISPLAY_IENGINE_H
#define ENGINE_DISPLAY_IENGINE_H

#include "engine/display/CAnimDictionary.h"
#include "engine/display/CImmediate.h"
#include "engine/display/CObjectDictionary.h"
#include "engine/display/CRWStream.h"
#include "engine/display/CScene.h"
#include "engine/display/CTextureDictionary.h"
#include "engine/display/CTimerGCN.h"
#include "engine/display/IEventAnimationCallback.h"

// FIXME: This maybe goes in its own header, but it's only used here.
class CDKW_Memory {
public:
    void* m_file;
    U32 m_unk4;
};

// FIXME: Put this in a RW header.
enum RwRenderState {};

namespace DKDSP {

class IShadowMapValidationCallback;

class IEngine {
public:
    virtual void fillerfunc0() = 0;
    virtual void fillerfunc1() = 0;
    virtual void fillerfunc2() = 0;
    virtual void SetCharsetCreation(BOOL) = 0;
    virtual void fillerfunc4() = 0;
    virtual void fillerfunc5() = 0;
    virtual void fillerfunc6() = 0;
    virtual void fillerfunc7() = 0;
    virtual void SetGCNVideoMode(void*) = 0;
    virtual void fillerfunc9() = 0;
    virtual void fillerfunc10() = 0;
    virtual void fillerfunc11() = 0;
    virtual void* Open() = 0; // idk what this returns
    virtual void fillerfunc13() = 0;
    virtual void Setup(int, int) = 0;
    virtual void Start() = 0;
    virtual void fillerfunc16() = 0;
    virtual BOOL Update() = 0;
    virtual void Release() = 0;
    virtual void fillerfunc19() = 0;
    virtual void fillerfunc20() = 0;
    virtual void fillerfunc21() = 0;
    virtual void fillerfunc22() = 0;
    virtual void fillerfunc23() = 0;
    virtual void fillerfunc24() = 0;
    virtual void fillerfunc25() = 0;
    virtual void fillerfunc26() = 0;
    virtual void fillerfunc27() = 0;
    virtual void fillerfunc28() = 0;
    virtual void fillerfunc29() = 0;
    virtual void fillerfunc30() = 0;
    virtual void RegisterShadowMapValidationCallback(IShadowMapValidationCallback*) = 0;
    virtual void fillerfunc32() = 0;
    virtual void fillerfunc33() = 0;
    virtual void fillerfunc34() = 0;
    virtual void fillerfunc35() = 0;
    virtual void fillerfunc36() = 0;
    virtual void AlphaAtomicBufferization(BOOL enable) = 0;
    virtual BOOL IsAlphaAtomicBufferizationEnabled() = 0;
    virtual void fillerfunc39() = 0;
    virtual void fillerfunc40() = 0;
    virtual void fillerfunc41() = 0;
    virtual void fillerfunc42() = 0;
    virtual void fillerfunc43() = 0;
    virtual void fillerfunc44() = 0;
    virtual void fillerfunc45() = 0;
    virtual void fillerfunc46() = 0;
    virtual void fillerfunc47() = 0;
    virtual void fillerfunc48() = 0;
    virtual void fillerfunc49() = 0;
    virtual void fillerfunc50() = 0;
    virtual void fillerfunc51() = 0;
    virtual void fillerfunc52() = 0;
    virtual void fillerfunc53() = 0;
    virtual void fillerfunc54() = 0;
    virtual void fillerfunc55() = 0;
    virtual void fillerfunc56() = 0;
    virtual void fillerfunc57() = 0;
    virtual void fillerfunc58() = 0;
    virtual void fillerfunc59() = 0;
    virtual void fillerfunc60() = 0;
    virtual void fillerfunc61() = 0;
    virtual void fillerfunc62() = 0;
    virtual void fillerfunc63() = 0;
    virtual CScene* CreateScene() = 0;
    virtual void fillerfunc65() = 0;
    virtual CScene* GetScene(int) = 0;
    virtual void fillerfunc67() = 0;
    virtual void fillerfunc68() = 0;
    virtual void SetImagePath(char* path) = 0;
    virtual void fillerfunc70() = 0;
    virtual void fillerfunc71() = 0;
    virtual void fillerfunc72() = 0;
    virtual CRWStream* OpenStreamMemory(U32, CDKW_Memory*) = 0; // FIXME: RwStreamAccessType, CDKW_Memory*
    virtual void fillerfunc74() = 0;
    virtual void DestroyStream(IRWStream*) = 0;
    virtual CObjectDictionary* GetObjectDictionary() = 0;
    virtual CAnimDictionary* GetAnimDictionary() = 0;
    virtual CTextureDictionary* GetTextureDictionary() = 0;
    virtual CImmediate* GetImmediate() = 0;
    virtual CTimerGCN* CreateTimer() = 0;
    virtual void fillerfunc81() = 0;
    virtual void SetRenderState(RwRenderState, void*) = 0;
    virtual void fillerfunc83() = 0;
    virtual void RegisterEvent(U32, std::string name, IEventAnimationCallback*) = 0;
    // TODO
};

}

#endif
