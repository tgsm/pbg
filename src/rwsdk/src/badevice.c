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

unsigned char _rwMsbBit[] = {
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
static int engineInstancesOpened;
RwGlobals* RwEngineInstance;

extern void* _rwColorOpen(void*, int offset, int);
extern void* _rwColorClose(void*, int, int);
extern void* _rwRenderPipelineOpen(void*, int offset, int);
extern void* _rwRenderPipelineClose(void*, int, int);
extern void* _rwIm3DOpen(void*, int offset, int);
extern void* _rwIm3DClose(void*, int, int);
extern void* _rwResourcesOpen(void*, int offset, int);
extern void* _rwResourcesClose(void*, int, int);
extern int _rwPipeAttach(void);
extern int _rwDeviceRegisterPlugin(void);

static int CorePluginAttach(void) {
    int ret = 0;
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
        return 1;
    }

    return 0;
}

static void* MallocWrapper(RwFreeList* freeListMaybe) {
    return RwMalloc(freeListMaybe->entrySize);
}

static RwFreeList* FreeWrapper(RwFreeList* freeListMaybe, void* ptr) {
    RwFree(ptr);
    return freeListMaybe;
}

static int EngineOpen(RwDevice* device, void* a1) {
    void* globals;
    DONT_INLINE_HACK();
    RwEngineInstance = (RwGlobals*)RwMalloc(engineTKList.sizeOfStruct);
    globals = RwEngineInstance;
    if (globals != NULL) {
        memcpy(globals, &staticGlobals, sizeof(RwGlobals));
        _rwDeviceSystemRequest(device, rwDEVICESYSTEMREGISTER, &RwEngineInstance->dOpenDevice, &RwEngineInstance->memoryFuncs, 0);

        if (_rwDeviceSystemRequest(device, rwDEVICESYSTEMOPEN, NULL, a1, 0)) {
            _rwDeviceSystemRequest(device, rwDEVICESYSTEMSTANDARDS, &RwEngineInstance->unk48, NULL, 29);
            engineInstancesOpened++;
            return 1;
        } else {
            RwEngineInstance = &staticGlobals;
            memcpy(&staticGlobals, globals, sizeof(RwGlobals));
            RwFree(globals);
            return 0;
        }
    }

    RwThrowErrorParams(1, E_RW_NOMEM, engineTKList.sizeOfStruct);
    return 0;
}

int _rwDeviceSystemRequest(RwDevice* device, int systemFn, void* dest, void* a3, int a4) {
    int ret = device->fpSystem(systemFn, dest, a3, a4);
    DONT_INLINE_HACK();
    if (ret == 0) {
        switch (systemFn) {
            case rwDEVICESYSTEMFINALIZESTART:
            case rwDEVICESYSTEMINITIATESTOP:
                ret = 1;
                break;
            case rwDEVICESYSTEMGETNUMSUBSYSTEMS:
                *((int*)dest) = 1;
                ret = 1;
                break;
            case rwDEVICESYSTEMGETSUBSYSTEMINFO:
                ret = (a4 == 0);
                if (ret != 0) {
                    RwEngineInstance->stringFuncs.rwstrcpy((char*)dest, "Only rendering sub system");
                    break;
                }
                break;
            case rwDEVICESYSTEMGETCURRENTSUBSYSTEM:
                *((int*)dest) = 0;
                ret = 1;
                break;
            case rwDEVICESYSTEMSETSUBSYSTEM:
                ret = (a4 == 0);
                break;
        }
    }

    if (ret == 0) {
        RwThrowErrorParams(1, 24, systemFn);
    }

    return ret;
}

unsigned int _rwGetNumEngineInstances(void) {
    return engineInstancesOpened;
}

unsigned int RwEngineGetVersion() {
    return 0x34003; // version 3.4.0.3
}

int RwEngineRegisterPlugin(int size, int pluginID, RwPluginObjectConstructor constructCB, RwPluginObjectDestructor destructCB) {
    return _rwPluginRegistryAddPlugin(&engineTKList, size, pluginID, constructCB, destructCB, NULL);
}

unsigned int RwEngineGetPluginOffset(int pluginID) {
    return _rwPluginRegistryGetPluginOffset(&engineTKList, pluginID);
}

unsigned int RwEngineGetNumSubSystems(void) {
    int subSystems = 1;
    _rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMGETNUMSUBSYSTEMS, &subSystems, NULL, 0);
    return subSystems;
}

RwSubSystemInfo* RwEngineGetSubSystemInfo(RwSubSystemInfo* subSystemInfo, int subSystem) {
    if (_rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMGETSUBSYSTEMINFO, subSystemInfo, NULL, subSystem) == 0) {
        subSystemInfo = NULL;
    }
    return subSystemInfo;
}

int RwEngineGetCurrentSubSystem(void) {
    int subSystem;
    if (_rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMGETCURRENTSUBSYSTEM, &subSystem, NULL, 0) != 0) {
        return subSystem;
    }
    return -1;
}

int RwEngineSetSubSystem(int subSystem) {
    return _rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMSETSUBSYSTEM, NULL, NULL, subSystem) != 0;
}

unsigned int RwEngineGetNumVideoModes(void) {
    int videoModes;
    if (_rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMGETNUMMODES, &videoModes, NULL, 0) != 0) {
        return videoModes;
    }
    return -1;
}

RwVideoMode* RwEngineGetVideoModeInfo(RwVideoMode* videoModeInfo, int videoMode) {
    if (_rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMGETMODEINFO, videoModeInfo, NULL, videoMode) == 0) {
        videoModeInfo = NULL;
    }
    return videoModeInfo;
}

int RwEngineGetCurrentVideoMode(void) {
    int videoMode;
    if (_rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMGETMODE, &videoMode, NULL, 0) != 0) {
        return videoMode;
    }
    return -1;
}

int RwEngineSetVideoMode(int subsystem) {
    return _rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMUSEMODE, NULL, NULL, subsystem) != 0;
}

int RwEngineGetTextureMemorySize(void) {
    int size;
    if (_rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMGETTEXMEMSIZE, &size, NULL, 0) != 0) {
        return size;
    }
    return -1;
}

int RwEngineGetMaxTextureSize(void) {
    int size;
    if (_rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMGETMAXTEXTURESIZE, &size, NULL, 0) != 0) {
        return size;
    }
    return -1;
}

int RwEngineStop(void) {
    int ret;
    RwDevice* device = &RwEngineInstance->dOpenDevice;

    _rwDeviceSystemRequest(device, rwDEVICESYSTEMINITIATESTOP, NULL, NULL, 0);
    _rwPluginRegistryDeInitObject(&engineTKList, RwEngineInstance);
    ret = _rwDeviceSystemRequest(device, rwDEVICESYSTEMSTOP, NULL, NULL, 0);
    if (ret != 0) {
        RwEngineInstance->engineStatus = rwENGINESTATUSOPENED;
    }
    return ret;
}

int RwEngineStart(void) {
    RwDevice* device = &RwEngineInstance->dOpenDevice;

    if (_rwDeviceSystemRequest(device, rwDEVICESYSTEMSTART, NULL, NULL, 0) != 0) {
        if (_rwPluginRegistryInitObject(&engineTKList, RwEngineInstance)) {
            RwImageSetGamma(RwEngineInstance->dOpenDevice.gammaCorrection);
            _rwDeviceSystemRequest(device, rwDEVICESYSTEMFINALIZESTART, NULL, NULL, 0);
            RwEngineInstance->engineStatus = rwENGINESTATUSSTARTED;
            return 1;
        }

        _rwDeviceSystemRequest(device, rwDEVICESYSTEMSTOP, NULL, NULL, 0);
    }

    return 0;
}

int RwEngineClose(void) {
    int ret = _rwDeviceSystemRequest(&RwEngineInstance->dOpenDevice, rwDEVICESYSTEMCLOSE, NULL, NULL, 0);
    if (ret != 0) {
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

int RwEngineOpen(void* a0) {
    int ret;

    if (RwEngineInstance == NULL) {
        RwEngineInstance = &staticGlobals;
    }

    ret = (RwEngineInstance->engineStatus == rwENGINESTATUSINITED);
    if (ret != 0) {
        ret = (a0 != NULL);
        if (ret != 0) {
            RwDevice* device = _rwDeviceGetHandle();
            ret = (device != NULL);
            if (ret != 0) {
                ret = EngineOpen(device, a0);
                if (ret != 0) {
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

int RwEngineTerm(void) {
    int ret = (engineInstancesOpened == 0);
    if (ret != 0) {
        _rwPluginRegistryClose();
        _rwFileSystemClose();
        _rwMemoryClose();
        RwEngineInstance->engineStatus = rwENGINESTATUSIDLE;
    }
    return ret;
}

int RwEngineInit(RwMemoryFunctions* memoryFuncs, RwEngineInitFlag initFlags, unsigned int arenaSize) {
    int ret = 0;
    RwEngineInstance = &staticGlobals;

    if (initFlags & rwENGINEINITNOFREELISTS) {
        RwEngineInstance->memoryAlloc = MallocWrapper;
        RwEngineInstance->memoryFree = FreeWrapper;
        _rwFreeListEnable(0);
    } else {
        RwEngineInstance->memoryAlloc = _rwFreeListAllocReal;
        RwEngineInstance->memoryFree = _rwFreeListFreeReal;
        _rwFreeListEnable(1);
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
