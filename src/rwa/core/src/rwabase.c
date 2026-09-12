#include "dont_inline_hack.h"
#include <rwa/core/rwabase.h>
#include <rwa/core/rwamemory.h>
#include <rwa/core/rwaobj.h>
#include <rwa/core/rwaobjdef.h>
#include <rwa/core/rwaobjinterface.h>
#include <rwa/core/rwavoicemanager.h>
#include <rwa/core/rwawavedef.h>
#include <rwa/objects/rwalistenerobj.h>

RwBool _rwaGcoreOpen = FALSE;
RwBool _rwaGcoreClosing = FALSE;
static RwMemoryFunctions* _rwMemFuncs = NULL;
static RwInt32 _tickTime = 0;
static RwBool _updating = FALSE;
static RwBool _usingRwG = FALSE;
RwInt32 _rwaGblockAlign = 0;
RwBool _rwaGUseReleaseLibrary = TRUE;

static RwBool _RwaCoreOpenPlatform(void) {
    if (!_usingRwG) {
        RwInt32 unk;

        if (!RwEngineInit(_rwMemFuncs, rwENGINEINITFREELISTS, 0x400)) {
            return FALSE;
        }

        unk = 1;
        if (!RwEngineOpen(&unk)) {
            RwEngineTerm();
            return FALSE;
        }

        if (!(RwEngineStart())) {
            RwEngineClose();
            RwEngineTerm();
            return FALSE;
        }
    }

    return TRUE;
}

static void _RwaCoreClosePlatform(void) {
    if (!_usingRwG) {
        RwEngineStop();
        RwEngineClose();
        RwEngineTerm();
    }
}

extern RwBool _rwaFileModuleOpen(void);
extern void _rwaFileModuleClose(void);
extern RwBool _rwaFreeListModuleOpen(void);
extern void _rwaFreeListModuleClose(void);
extern RwBool _rwaHeapModuleOpen(void);
extern void _rwaHeapModuleClose(void);
extern RwBool _rwaUniqueIDModuleOpen(void);
extern void _rwaUniqueIDModuleClose(void);
extern RwBool _rwaLListModuleOpen(void);
extern void _rwaLListModuleClose(void);
extern RwBool _rwaParamTypeOpenModule(void);
extern void _rwaParamTypeCloseModule(void);
extern RwBool _rwaObjDefOpenModule(void);
extern void _rwaObjDefCloseModule(void);
extern RwBool _rwaObjOpenModule(void);
extern void _rwaObjCloseModule(void);
extern RwBool _rwaObjInterfaceOpenModule(void);
extern void _rwaObjInterfaceCloseModule(void);
extern RwBool _rwaWaveDefOpenModule(void);
extern void _rwaWaveDefCloseModule(void);
extern RwBool _rwaWaveDictOpenModule(void);
extern void _rwaWaveCloseModule(void);
extern RwBool _rwaWaveOpenModule(void);
extern void _rwaWaveDictCloseModule(void);
extern RwBool _rwaVoiceManagerOpenModule(void);
extern void _rwaVoiceManagerCloseModule(void);
extern RwBool _rwaObjHandleOpenModule(void);
extern void _rwaObjHandleCloseModule(void);
extern RwBool _rwaTimerModuleOpen(void);
extern void _rwaTimerModuleClose(void);

static RwaCoreCodeModule _codeModules[] = {
    { _RwaCoreOpenPlatform, _RwaCoreClosePlatform },
    {_rwaFileModuleOpen, _rwaFileModuleClose },
    {_rwaFreeListModuleOpen, _rwaFreeListModuleClose },
    {_rwaHeapModuleOpen, _rwaHeapModuleClose },
    {_rwaUniqueIDModuleOpen, _rwaUniqueIDModuleClose },
    {_rwaLListModuleOpen, _rwaLListModuleClose },
    {_rwaParamTypeOpenModule, _rwaParamTypeCloseModule },
    {_rwaObjDefOpenModule, _rwaObjDefCloseModule },
    {_rwaObjOpenModule, _rwaObjCloseModule },
    {_rwaObjInterfaceOpenModule, _rwaObjInterfaceCloseModule },
    {_rwaWaveDefOpenModule, _rwaWaveDefCloseModule },
    {_rwaWaveDictOpenModule, _rwaWaveCloseModule },
    {_rwaWaveOpenModule, _rwaWaveDictCloseModule },
    {_rwaVoiceManagerOpenModule, _rwaVoiceManagerCloseModule },
    {_rwaObjHandleOpenModule, _rwaObjHandleCloseModule },
    {_rwaTimerModuleOpen, _rwaTimerModuleClose },
};

typedef RwBool (*UnkOpenCallback)(RwaCoreCodeModule* module, RwInt32 i);
typedef void (*UnkCloseCallback)(RwaCoreCodeModule* module, RwInt32 i);

static RwBool _RwaCoreOpenCodeModules(RwaCoreCodeModule* modules, RwUInt32 numModules, UnkOpenCallback openCallback, UnkCloseCallback closeCallback) {
    RwUInt32 i = 0;
    DONT_INLINE_HACK();
    while (i < numModules) {
        RwBool opened = modules[i].openFunc();
        if (opened) {
            if (openCallback != NULL) {
                opened = openCallback(&modules[i], i);
            }
            i++;
        }
        if (!opened) {
            for (; i != 0; i--) {
                modules[i - 1].closeFunc();
                if (closeCallback != 0) {
                    closeCallback(&modules[i - 1], i - 1);
                }
            }
            return FALSE;
        }
    }

    return TRUE;
}

static RwBool _RwaCoreOpenModuleCallback(RwaCoreCodeModule* module, RwInt32 i) {
    if (module->openFunc == _RwaCoreOpenPlatform) {
        if (_rwMemFuncs == NULL) {
            _rwMemFuncs = RwOsGetMemoryInterface();
        }

        if (_rwaMemoryOpen(_rwMemFuncs) == TRUE) {
            return TRUE;
        }

        return FALSE;
    }

    return TRUE;
}

static void _RwaCoreCloseModuleCallback(RwaCoreCodeModule* module, RwInt32 i) {
    if (module->closeFunc == _RwaCoreClosePlatform) {
        _rwaMemoryClose();
    }
}

RwBool _rwaCoreOpen(RwMemoryFunctions* memFuncs, RwBool usingRwG) {
    RwaSetBlockAlign(16);

    _usingRwG = usingRwG;
    _rwMemFuncs = memFuncs;

    if (_RwaCoreOpenCodeModules(_codeModules, 16, _RwaCoreOpenModuleCallback, _RwaCoreCloseModuleCallback) == TRUE) {
        _updating = FALSE;
        _tickTime = 0;
        _rwaGcoreOpen = TRUE;
        return TRUE;
    }

    return FALSE;
}

void RwaCoreClose(void) {
    RwUInt32 i;

    _rwaGcoreClosing = TRUE;
    if (_updating == TRUE) {
        RwaCoreUpdateFinish();
    }

    for (i = 16; i != 0; i--) {
        _codeModules[i - 1].closeFunc();
        if (_RwaCoreCloseModuleCallback != NULL) {
            RwaCoreCodeModule* module = &_codeModules[i - 1];
            _RwaCoreCloseModuleCallback(module, i);
        }
    }

    _rwaGcoreOpen = FALSE;
    _rwaGcoreClosing = FALSE;
}

extern void RwaOsCoreSuspend(void);
extern void RwaOsCoreResume(void);

void RwaCoreSuspend(void) {
    RwaOsCoreSuspend();
}

void RwaCoreResume(void) {
    RwaOsCoreResume();
}

void RwaCoreUpdateStart(void) {
    _updating = TRUE;
    _rwaVoiceManagerUpdate();
    RwaObjStartExecute(RwaObjGetTopLevelObject(), FALSE);
}

void RwaCoreUpdateFinish(void) {
    RwaObjFinishExecute(RwaObjGetTopLevelObject(), FALSE);
    _updating = FALSE;
    _tickTime++;
}

extern RwaObjInterfaceDef* RwaOutputInterfaceRegister(void);
extern RwaObjInterfaceDef* RwaVoiceInterfaceRegister(void);
extern RwaObjInterfaceDef* RwaVoice3DInterfaceRegister(void);

static RwaObjInterfaceRegisterFunc _interfaces[] = {
    RwaOutputInterfaceRegister,
    RwaVoiceInterfaceRegister,
    RwaVoice3DInterfaceRegister,
};

static RwaObjDefRegisterFunc _objDefs[] = {
    RwaListenerRegisterUsingInterface,
};

extern RwaWaveDef* _rwaNULLWaveRegister(void);
static RwaWaveDefRegisterFunc _waveDefs[] = {
    _rwaNULLWaveRegister,
};

RwBool RwaCoreRegisterDefaultObjects(void) {
    if (_rwaObjInterfaceRegister(_interfaces, 3) == TRUE &&
        _rwaObjDefRegister(_objDefs, 1) == TRUE &&
        _rwaWaveDefRegister(_waveDefs, 1) == TRUE) {
        return TRUE;
    }

    return FALSE;
}

RwUInt32 RwaSetBlockAlign(RwUInt32 align) {
    if (align < 16) {
        _rwaGblockAlign = 16;
        return _rwaGblockAlign;
    } else {
        _rwaGblockAlign = align;
        return _rwaGblockAlign;
    }
}

RwUInt32 RwaSizeOf(RwaType type) {
    // FIXME: Use sizeof constants here?
    switch (type) {
        case rwaTYPE_UINT8:
            return 1;
        case rwaTYPE_INT8:
            return 1;
        case rwaTYPE_UINT16:
            return 2;
        case rwaTYPE_INT16:
            return 2;
        case rwaTYPE_UINT32:
            return 4;
        case rwaTYPE_INT32:
            return 4;
        case rwaTYPE_UINT64:
            return 8;
        case rwaTYPE_INT64:
            return 8;
        case rwaTYPE_UINT128:
            return 16;
        case rwaTYPE_INT128:
            return 16;
        case rwaTYPE_FLOAT:
            return 4;
        case rwaTYPE_DOUBLE:
            return 4;
        case rwaTYPE_ARRAY:
            return 4;
        case rwaTYPE_POINTER:
            return 4;
    }

    return 0;
}
