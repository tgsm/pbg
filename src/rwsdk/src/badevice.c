#include <string.h>
#include <rwsdk/plcore/baerr.h>
#include <rwsdk/plcore/bafsys.h>
#include <rwsdk/plcore/bamatrix.h>
#include <rwsdk/plcore/bamemory.h>
#include <rwsdk/plcore/bastream.h>
#include <rwsdk/plcore/bavector.h>
#include <rwsdk/plcore/rwstring.h>
#include <rwsdk/bacamera.h>
#include <rwsdk/badevice.h>
#include <rwsdk/baframe.h>
#include <rwsdk/baimage.h>
#include <rwsdk/baraster.h>
#include <rwsdk/batextur.h>
#include "dont_inline_hack.h"

#ifdef __cplusplus
extern "C" {
#endif

static struct RwPluginRegistry engineTKList = {
    sizeof(RwGlobals),
    sizeof(RwGlobals),
    0,
    0,
    NULL,
    NULL,
};

RwUInt8 _rwMsbBit[] = {
    0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
};

static RwGlobals staticGlobals;
static RwInt32 engineInstancesOpened;
RwGlobals* RwEngineInstance;

extern void* _rwColorOpen(void*, RwInt32 offset, RwInt32);
extern void* _rwColorClose(void*, RwInt32, RwInt32);
extern void* _rwRenderPipelineOpen(void*, RwInt32 offset, RwInt32);
extern void* _rwRenderPipelineClose(void*, RwInt32, RwInt32);
extern void* _rwIm3DOpen(void*, RwInt32 offset, RwInt32);
extern void* _rwIm3DClose(void*, RwInt32, RwInt32);
extern void* _rwResourcesOpen(void*, RwInt32 offset, RwInt32);
extern void* _rwResourcesClose(void*, RwInt32, RwInt32);
extern RwBool _rwPipeAttach(void);
extern RwBool _rwDeviceRegisterPlugin(void);

static RwBool CorePluginAttach(void) {
    RwInt32 ret = 0;
    DONT_INLINE_HACK();
    ret |= RwEngineRegisterPlugin(8, 0x40F, _rwErrorOpen, _rwErrorClose);
    ret |= RwEngineRegisterPlugin(16, 0x401, _rwVectorOpen, _rwVectorClose);
    ret |= RwEngineRegisterPlugin(0, 0x40D, _rwColorOpen, _rwColorClose);
    ret |= RwEngineRegisterPlugin(24, 0x402, _rwMatrixOpen, _rwMatrixClose);
    ret |= RwEngineRegisterPlugin(4, 0x403, _rwFrameOpen, _rwFrameClose);
    ret |= RwEngineRegisterPlugin(4, 0x404, _rwStreamModuleOpen, _rwStreamModuleClose);
    ret |= RwEngineRegisterPlugin(4, 0x405, _rwCameraOpen, _rwCameraClose);
    ret |= RwEngineRegisterPlugin(544, 0x406, _rwImageOpen, _rwImageClose);
    ret |= RwEngineRegisterPlugin(100, 0x407, _rwRasterOpen, _rwRasterClose);
    ret |= RwEngineRegisterPlugin(48, 0x408, _rwTextureOpen, _rwTextureClose);
    ret |= RwEngineRegisterPlugin(96, 0x409, _rwRenderPipelineOpen, _rwRenderPipelineClose);
    ret |= _rwPipeAttach();
    ret |= RwEngineRegisterPlugin(116, 0x40A, _rwIm3DOpen, _rwIm3DClose);
    ret |= RwEngineRegisterPlugin(40, 0x40B, _rwResourcesOpen, _rwResourcesClose);
    if (ret >= 0) {
        return TRUE;
    }

    return FALSE;
}

static void* MallocWrapper(RwFreeList* freeListMaybe) {
    return RwMalloc(freeListMaybe->entrySize);
}

static RwFreeList* FreeWrapper(RwFreeList* freeListMaybe, void* ptr) {
    RwFree(ptr);
    return freeListMaybe;
}

static RwBool EngineOpen(RwDevice* device, void* a1) {
    void* globals;
    DONT_INLINE_HACK();
    RwEngineInstance = (RwGlobals*)RwMalloc(engineTKList.sizeOfStruct);
    globals = RwEngineInstance;
    if (globals != NULL) {
        memcpy(globals, &staticGlobals, sizeof(RwGlobals));
        _rwDeviceSystemRequest(device, rwDEVICESYSTEMREGISTER, &RwEngineInstance->dOpenDevice, &RwEngineInstance->memoryFuncs, 0);

        if (_rwDeviceSystemRequest(device, rwDEVICESYSTEMOPEN, NULL, a1, 0)) {
            _rwDeviceSystemRequest(device, rwDEVICESYSTEMSTANDARDS, &RwEngineInstance->stdFunc, NULL, 29);
            engineInstancesOpened++;
            return TRUE;
        } else {
            RwEngineInstance = &staticGlobals;
            memcpy(&staticGlobals, globals, sizeof(RwGlobals));
            RwFree(globals);
            return FALSE;
        }
    }

    RwThrowErrorParams(1, E_RW_NOMEM, engineTKList.sizeOfStruct);
    return FALSE;
}

RwBool _rwDeviceSystemRequest(RwDevice* device, RwInt32 systemFn, void* dest, void* a3, RwInt32 a4) {
    RwBool ret = device->fpSystem(systemFn, dest, a3, a4);
    DONT_INLINE_HACK();
    if (!ret) {
        switch (systemFn) {
            case rwDEVICESYSTEMFINALIZESTART:
            case rwDEVICESYSTEMINITIATESTOP:
                ret = TRUE;
                break;
            case rwDEVICESYSTEMGETNUMSUBSYSTEMS:
                *((RwInt32*)dest) = 1;
                ret = TRUE;
                break;
            case rwDEVICESYSTEMGETSUBSYSTEMINFO:
                ret = (a4 == 0);
                if (ret) {
                    RwEngineInstance->stringFuncs.rwstrcpy((RwChar*)dest, "Only rendering sub system");
                    break;
                }
                break;
            case rwDEVICESYSTEMGETCURRENTSUBSYSTEM:
                *((RwInt32*)dest) = 0;
                ret = TRUE;
                break;
            case rwDEVICESYSTEMSETSUBSYSTEM:
                ret = (a4 == 0);
                break;
        }
    }

    if (!ret) {
        RwThrowErrorParams(1, 24, systemFn);
    }

    return ret;
}

RwUInt32 _rwGetNumEngineInstances(void) {
    return engineInstancesOpened;
}

RwUInt32 RwEngineGetVersion() {
    return 0x34003; // version 3.4.0.3
}

RwInt32 RwEngineRegisterPlugin(RwInt32 size, RwInt32 pluginID, RwPluginObjectConstructor constructCB, RwPluginObjectDestructor destructCB) {
    return _rwPluginRegistryAddPlugin(&engineTKList, size, pluginID, constructCB, destructCB, NULL);
}

RwUInt32 RwEngineGetPluginOffset(RwInt32 pluginID) {
    return _rwPluginRegistryGetPluginOffset(&engineTKList, pluginID);
}

RwUInt32 RwEngineGetNumSubSystems(void) {
    RwInt32 subSystems = 1;
    _rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMGETNUMSUBSYSTEMS, &subSystems, NULL, 0);
    return subSystems;
}

RwSubSystemInfo* RwEngineGetSubSystemInfo(RwSubSystemInfo* subSystemInfo, RwInt32 subSystem) {
    if (!_rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMGETSUBSYSTEMINFO, subSystemInfo, NULL, subSystem)) {
        subSystemInfo = NULL;
    }
    return subSystemInfo;
}

RwInt32 RwEngineGetCurrentSubSystem(void) {
    RwInt32 subSystem;
    if (_rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMGETCURRENTSUBSYSTEM, &subSystem, NULL, 0)) {
        return subSystem;
    }
    return -1;
}

RwBool RwEngineSetSubSystem(RwInt32 subSystem) {
    return _rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMSETSUBSYSTEM, NULL, NULL, subSystem) != FALSE;
}

RwUInt32 RwEngineGetNumVideoModes(void) {
    RwInt32 videoModes;
    if (_rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMGETNUMMODES, &videoModes, NULL, 0)) {
        return videoModes;
    }
    return -1;
}

RwVideoMode* RwEngineGetVideoModeInfo(RwVideoMode* videoModeInfo, RwInt32 videoMode) {
    if (!_rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMGETMODEINFO, videoModeInfo, NULL, videoMode)) {
        videoModeInfo = NULL;
    }
    return videoModeInfo;
}

RwInt32 RwEngineGetCurrentVideoMode(void) {
    RwInt32 videoMode;
    if (_rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMGETMODE, &videoMode, NULL, 0)) {
        return videoMode;
    }
    return -1;
}

RwBool RwEngineSetVideoMode(RwInt32 subsystem) {
    return _rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMUSEMODE, NULL, NULL, subsystem) != FALSE;
}

RwInt32 RwEngineGetTextureMemorySize(void) {
    RwInt32 size;
    if (_rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMGETTEXMEMSIZE, &size, NULL, 0)) {
        return size;
    }
    return -1;
}

RwInt32 RwEngineGetMaxTextureSize(void) {
    RwInt32 size;
    if (_rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMGETMAXTEXTURESIZE, &size, NULL, 0)) {
        return size;
    }
    return -1;
}

RwBool RwEngineStop(void) {
    RwBool ret;
    RwDevice* device = &RwEngineInstance->dOpenDevice;

    _rwDeviceSystemRequest(device, rwDEVICESYSTEMINITIATESTOP, NULL, NULL, 0);
    _rwPluginRegistryDeInitObject(&engineTKList, RwEngineInstance);
    ret = _rwDeviceSystemRequest(device, rwDEVICESYSTEMSTOP, NULL, NULL, 0);
    if (ret) {
        RwEngineInstance->engineStatus = rwENGINESTATUSOPENED;
    }
    return ret;
}

RwBool RwEngineStart(void) {
    RwDevice* device = &RwEngineInstance->dOpenDevice;

    if (_rwDeviceSystemRequest(device, rwDEVICESYSTEMSTART, NULL, NULL, 0)) {
        if (_rwPluginRegistryInitObject(&engineTKList, RwEngineInstance)) {
            RwImageSetGamma(RwEngineInstance->dOpenDevice.gammaCorrection);
            _rwDeviceSystemRequest(device, rwDEVICESYSTEMFINALIZESTART, NULL, NULL, 0);
            RwEngineInstance->engineStatus = rwENGINESTATUSSTARTED;
            return TRUE;
        }

        _rwDeviceSystemRequest(device, rwDEVICESYSTEMSTOP, NULL, NULL, 0);
    }

    return FALSE;
}

RwBool RwEngineClose(void) {
    RwBool ret = _rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMCLOSE, NULL, NULL, 0);
    if (ret) {
        void* globals = RwEngineInstance;
        RwEngineInstance = &staticGlobals;
        memcpy(&staticGlobals, globals, sizeof(RwGlobals));
        RwFree(globals);
        engineInstancesOpened--;
        RwEngineInstance->engineStatus = rwENGINESTATUSINITED;
    }
    return ret;
}

extern RwDevice* _rwDeviceGetHandle(void);

RwBool RwEngineOpen(void* a0) {
    RwBool ret;

    if (RwEngineInstance == NULL) {
        RwEngineInstance = &staticGlobals;
    }

    ret = (RwEngineInstance->engineStatus == rwENGINESTATUSINITED);
    if (ret) {
        ret = (a0 != NULL);
        if (ret) {
            RwDevice* device = _rwDeviceGetHandle();
            ret = (device != NULL);
            if (ret) {
                ret = EngineOpen(device, a0);
                if (ret) {
                    RwEngineInstance->engineStatus = rwENGINESTATUSOPENED;
                }
            }
        } else {
            RwThrowError(1, E_RW_NULLP);
        }
    } else {
        RwThrowError(1, E_RW_BADENGINESTATE);
    }

    return ret;
}

RwBool RwEngineTerm(void) {
    RwBool ret = (engineInstancesOpened == 0);
    if (ret) {
        _rwPluginRegistryClose();
        _rwFileSystemClose();
        _rwMemoryClose();
        RwEngineInstance->engineStatus = rwENGINESTATUSIDLE;
    }
    return ret;
}

RwBool RwEngineInit(RwMemoryFunctions* memoryFuncs, RwEngineInitFlag initFlags, RwUInt32 arenaSize) {
    RwBool ret = FALSE;
    RwEngineInstance = &staticGlobals;

    if (initFlags & rwENGINEINITNOFREELISTS) {
        RwEngineInstance->memoryAlloc = MallocWrapper;
        RwEngineInstance->memoryFree = FreeWrapper;
        _rwFreeListEnable(FALSE);
    } else {
        RwEngineInstance->memoryAlloc = _rwFreeListAllocReal;
        RwEngineInstance->memoryFree = _rwFreeListFreeReal;
        _rwFreeListEnable(TRUE);
    }

    RwEngineInstance->resArenaInitSize = arenaSize;

    if (RwEngineInstance->engineStatus == rwENGINESTATUSIDLE) {
        ret = _rwStringOpen();
        if (ret) {
            ret = _rwMemoryOpen(memoryFuncs);
            if (ret) {
                ret = _rwFileSystemOpen();
                if (ret) {
                    ret = _rwPluginRegistryOpen();
                    if (ret) {
                        ret = CorePluginAttach();
                        if (ret) {
                            ret = _rwDeviceRegisterPlugin();
                            if (ret) {
                                RwEngineInstance->engineStatus = rwENGINESTATUSINITED;
                                return ret;
                            }
                        }
                        _rwPluginRegistryClose();
                    }
                    _rwFileSystemClose();
                }
                _rwMemoryClose();
            }
            _rwStringClose();
        }
    }
    return ret;
}

#ifdef __cplusplus
}
#endif
