#include <stddef.h>
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <dolphin/vi.h>
#include <rwsdk/badevice.h>
#include <rwsdk/plcore/baerr.h>
#include <rwsdk/driver/gcn/dl2drend.h>
#include "dont_inline_hack.h"

#ifdef __cplusplus
extern "C" {
#endif

RwDevice* _rwDeviceGetHandle(void);

static RwVideoMode _RwDlVideoModes[4] = {
    { 640, 480, 24, rwVIDEOMODEEXCLUSIVE },
    { 640, 528, 24, rwVIDEOMODEEXCLUSIVE },
    { 640, 480, 24, rwVIDEOMODEEXCLUSIVE },
    { 0, 0, 0, rwVIDEOMODEEXCLUSIVE },
};

// FIXME: `swap` and `gxInit` static variables are placed incorrectly.
// FIXME: Whole .sbss section is reversed. Compile with `-lang c++` to fix this.
// **However**, before you do, please see _rwDlSystem.
int _RwDlFSAATop = 1;
int _RwDlFifoSize = 0x40000;
static int _RwDlFirstFrame = 1;
static int _RwDlCopyClear;
int _RwDlPixelFormat;
int _RwDlCurPixelFormat;
int _RwGameCubeVideoMode;
int _RwDlFSAA;
static void* _RwDl_FIFO_XFB;
static void* _RwGCXFB1;
static void* _RwGCXFB2;
static void* _RwGCXFBCopy;
static void* _RwGCXFBDisp;
volatile static int _RwDlFrameCurrent;
volatile static int _RwDlFrameNew;
volatile static int _RwDlFrameTokenNew;
volatile static int _RwDlFrameTokenCurrent;
static int _RwDlBreakPointEnabled;
volatile static int _RwDlFrameReadyOnToken;
static int _RwDlFrameWait;
static int _RwDlFrameGo;
static OSThreadQueue _RwDlWaitingDoneRender;
static unsigned short _RwDlFrameSwap[3]; // struct?
static void* _RwDlDefaultFifo;
static GXFifoObj* _RwDlDefaultFifoObj;
int _RwDlHalfHeight;

struct UnkRwStandardFnLoad {
    int id;
    RwStandardFunc func;
};

struct UnkRwGCFrameQueueSub {
    void* unkXfb;
    void* breakpoint;
};
static struct UnkRwGCFrameQueueSub _RwGCFrameQueue[3];
static GXRenderModeObj _RwGameCubeRenderModeObj;

static void _rwDlBreakNext(void) {
    static int swap;
    unsigned int currentFrame;

    DONT_INLINE_HACK();

    currentFrame = (_RwDlFrameCurrent + 1) % 3;
    if (currentFrame == _RwDlFrameNew) {
        GXDisableBreakPt();
        _RwDlBreakPointEnabled = 0;
    } else {
        GXEnableBreakPt(_RwGCFrameQueue[currentFrame].breakpoint);
    }
    _RwDlFrameCurrent = currentFrame;

    OSWakeupThread(&_RwDlWaitingDoneRender);

    if (_RwDlFSAA == 0) {
        _RwDlFrameReadyOnToken = 1;
    } else if (swap == 1) {
        _RwDlFrameReadyOnToken = 1;
        swap = 0;
    } else {
        swap = 1;
    }
}

static void _rwDlBreakPtCallback(void) {
    if (_RwGCFrameQueue[_RwDlFrameCurrent].unkXfb != _RwGCXFBDisp) {
        _rwDlBreakNext();
    } else {
        _RwDlFrameWait = 1;
    }
}

extern int _rwDlTokenQueryDone(unsigned short);
GXRenderModeObj* _RwDlRenderMode;

// Equivalent: _RwGCXFBDisp reload
static void _rwDlVIPreRetraceCallback(u32 retraceCount) {
    short token = _RwDlFrameSwap[_RwDlFrameTokenCurrent];
    if (_RwDlFrameReadyOnToken == 1 && _rwDlTokenQueryDone(token)) {
        _RwGCXFBDisp = (_RwGCXFBDisp == _RwGCXFB1) ? _RwGCXFB2 : _RwGCXFB1;
        VISetNextFrameBuffer(_RwGCXFBDisp);

        if (_RwDlFirstFrame) {
            VISetBlack(0);
            _RwDlFirstFrame = 0;
        }

        VIFlush();

        _RwDlFrameTokenCurrent = (_RwDlFrameTokenCurrent + 1) % 3;
        _RwDlFrameReadyOnToken = 0;
        _RwDlFrameGo = 1;
    }
}

static void _rwDlVIPostRetraceCallback(u32 retraceCount) {
    if (_RwDlFrameWait && _RwDlFrameGo) {
        _rwDlBreakNext();
        _RwDlFrameWait = 0;
    }
    _RwDlFrameGo = 0;
}

static int _rwDlNullStandard(void*, void*, int) {
    return 0;
}

// Equivalent?: _RwGameCubeRenderModeObj copy scheduling
void _rwDlRenderModeSelect(GXRenderModeObj* renderMode, int pixelFormat) {
    if (renderMode != NULL) {
        int pf;
        _RwGameCubeRenderModeObj = *renderMode;
        _RwDlRenderMode = &_RwGameCubeRenderModeObj;
        _RwGameCubeVideoMode = 42;
        pf = pixelFormat;
        if (_RwGameCubeRenderModeObj.aa) {
            pf = 2;
        }
        _RwDlPixelFormat = pf;
        _RwDlCurPixelFormat = _RwDlPixelFormat;
        _RwDlVideoModes[3].width = _RwGameCubeRenderModeObj.fbWidth;
        _RwDlVideoModes[3].height = _RwGameCubeRenderModeObj.efbHeight;

        switch (pixelFormat) {
            case 0:
            case 1:
                _RwDlVideoModes[3].depth = 24;
                break;
            case 2:
                _RwDlVideoModes[3].depth = 16;
                break;
            default:
                RwThrowErrorParams(1, 2, "Invalid pixel format");
                break;
        }
    } else {
        switch (VIGetTvFormat()) {
            case VI_NTSC:
                _RwDlRenderMode = &GXNtsc480IntDf;
                _RwGameCubeVideoMode = 0;
                break;
            case VI_PAL:
            case VI_EURGB60:
                _RwDlRenderMode = &GXPal528IntDf;
                _RwGameCubeVideoMode = 1;
                break;
            case VI_MPAL:
                _RwDlRenderMode = &GXMpal480IntDf;
                _RwGameCubeVideoMode = 2;
                break;
            default:
                RwThrowErrorParams(1, 2, "Invalid TV format");
                break;
        }
    }
}

void _rwDlRenderModeInit(GXRenderModeObj* renderMode, int pixelFormat) {
    DONT_INLINE_HACK();

    VIConfigure(renderMode);
    VISetBlack(TRUE);
    VIFlush();

    if (renderMode->field_rendering) {
        GXSetViewportJitter(0.0f, 0.0f, renderMode->fbWidth, renderMode->xfbHeight, 0.0f, 1.0f, VIGetNextField() ^ 1);
    } else {
        GXSetViewport(0.0f, 0.0f, renderMode->fbWidth, renderMode->xfbHeight, 0.0f, 1.0f);
    }

    GXSetScissor(0, 0, renderMode->fbWidth, renderMode->efbHeight);
    GXSetScissorBoxOffset(0, 0);
    GXSetDispCopySrc(0, 0, renderMode->fbWidth, renderMode->efbHeight);
    GXSetDispCopyDst(renderMode->fbWidth, renderMode->xfbHeight);
    GXSetCopyFilter(renderMode->aa, renderMode->sample_pattern, GX_TRUE, renderMode->vfilter);

    if (renderMode->aa && renderMode->xfbHeight == renderMode->viHeight) {
        GXSetDispCopyYScale(1.0f);
        _RwDlHalfHeight = renderMode->xfbHeight / 2;
        _RwDlFSAA = 1;
        _RwDlFSAATop = 1;
    } else {
        GXSetDispCopyYScale((f32)renderMode->xfbHeight / (f32)renderMode->efbHeight);
        _RwDlFSAA = 0;
    }

    GXSetFieldMode(renderMode->field_rendering, renderMode->xfbHeight < renderMode->viHeight);
    GXSetPixelFmt(pixelFormat, GX_ZC_LINEAR);

    _RwDlFirstFrame = 1;
}

struct {
    void* unk0;
    void* unk4;
} dgGGlobals;
struct UnkRenderModeStruct {
    GXRenderModeObj* gxobj;
    int pixelformat;
    int fifosize;
};
extern void _rwDlRenderStateOpen(void);
extern void _rwDlRenderStateClose(void);
extern void vectorASMMultPoint();
extern void vectorASMMultVector();
extern void _rwVectorSetMultFn(void*, void*);
extern volatile unsigned short _RwDlTokenLastSeen;

// Equivalent: regalloc
// This function is affected by `-lang c` vs. `-lang c++`, the _RwDlVideoModes copies
// and the dgGGlobals write produce different codegen patterns.
int _rwDlSystem(int fn, void* dest, void* a2, int param) {
    switch (fn) {
        case rwDEVICESYSTEMUSEMODE:
            return (param < 1);
        case rwDEVICESYSTEMGETNUMMODES:
            *(int*)dest = 1;
            return 1;
        case rwDEVICESYSTEMGETMODEINFO:
            if (param < 1) {
                switch (_RwGameCubeVideoMode) {
                    case 0:
                        *(RwVideoMode*)dest = _RwDlVideoModes[0];
                        break;
                    case 1:
                    case 5:
                        *(RwVideoMode*)dest = _RwDlVideoModes[1];
                        break;
                    case 2:
                        *(RwVideoMode*)dest = _RwDlVideoModes[2];
                        break;
                    case 0x2A:
                        *(RwVideoMode*)dest = _RwDlVideoModes[3];
                        break;
                }
                return 1;
            } else {
                return 0;
            }
            break;
        case rwDEVICESYSTEMGETMODE:
            *(int*)dest = 0;
            return 1;
        case rwDEVICESYSTEMFOCUS:
            return 1;
        case rwDEVICESYSTEMREGISTER:
            *(RwDevice*)dest = *_rwDeviceGetHandle();
            dgGGlobals.unk4 = a2;
            return 1;
        case rwDEVICESYSTEMOPEN: {
            int iVar15;
            struct UnkRenderModeStruct* unk = *(struct UnkRenderModeStruct**)a2;
            if (unk != NULL) {
                _rwDlRenderModeSelect(unk->gxobj, unk->pixelformat);
                _RwDlFifoSize = unk->fifosize;
            } else {
                _rwDlRenderModeSelect(NULL, _RwDlPixelFormat);
            }

            iVar15 = ((_RwDlRenderMode->fbWidth + 0xF) & 0xFFF0) * _RwDlRenderMode->xfbHeight * 2;
            _RwDl_FIFO_XFB = RwEngineInstance->memoryFuncs.rwmalloc(_RwDlFifoSize + iVar15 * 2 + 0x1F);
            _RwDlDefaultFifo = (void*)(((int)_RwDl_FIFO_XFB + 0x1F) & ~0x1F);
            DCInvalidateRange(_RwDlDefaultFifo, _RwDlFifoSize);
            _RwGCXFBDisp = (void*)((int)_RwDlDefaultFifo + _RwDlFifoSize);
            _RwGCXFB1 = _RwGCXFBDisp;
            _RwGCXFB2 = (void*)((int)_RwGCXFB1 + iVar15);
            _RwGCXFBCopy = _RwGCXFB2;

            return 1;
        }
        case rwDEVICESYSTEMCLOSE:
            GXFlush();
            RwEngineInstance->memoryFuncs.rwfree(_RwDl_FIFO_XFB);
            _RwGCXFBDisp = NULL;
            _RwDlDefaultFifoObj = NULL;
            _RwGCXFB1 = NULL;
            _RwGCXFB2 = NULL;
            _RwGCXFBCopy = NULL;
            _RwDlDefaultFifo = NULL;
            _RwDl_FIFO_XFB = NULL;
            return 1;
        case rwDEVICESYSTEMSTART: {
            static int gxInit;
            if (!gxInit) {
                _RwDlDefaultFifoObj = GXInit(_RwDlDefaultFifo, _RwDlFifoSize);
                gxInit = 1;
            } else {
                GXFifoObj fifoobj;
                GXInitFifoBase(&fifoobj, _RwDlDefaultFifo, _RwDlFifoSize);
                GXSetCPUFifo(&fifoobj);
                GXSetGPFifo(&fifoobj);

                GXInitFifoBase(_RwDlDefaultFifoObj, _RwDlDefaultFifo, _RwDlFifoSize);
                GXSetCPUFifo(_RwDlDefaultFifoObj);
                GXSetGPFifo(_RwDlDefaultFifoObj);
            }

            while (_RwDlTokenLastSeen != GXReadDrawSync()) {
                GXSetDrawSync(_RwDlTokenLastSeen);
            }

            _rwDlRenderModeInit(_RwDlRenderMode, _RwDlPixelFormat);

            VIWaitForRetrace();
            if (_RwDlRenderMode->viTVmode & VI_NON_INTERLACE) {
                VIWaitForRetrace();
            }

            GXSetDispCopyGamma(GX_GM_1_0);
            VISetPreRetraceCallback(_rwDlVIPreRetraceCallback);
            VISetPostRetraceCallback(_rwDlVIPostRetraceCallback);
            GXSetBreakPtCallback(_rwDlBreakPtCallback);
            OSInitThreadQueue(&_RwDlWaitingDoneRender);

            return 1;
        }
        case rwDEVICESYSTEMFINALIZESTART:
            _rwVectorSetMultFn(vectorASMMultPoint, vectorASMMultVector);
            _rwDlRenderStateOpen();
            return 1;
        case rwDEVICESYSTEMINITIATESTOP:
            _rwDlRenderStateClose();
            return 1;
        case rwDEVICESYSTEMSTOP:
            return 1;
        case rwDEVICESYSTEMINITPIPELINE:
            return 1;
        case rwDEVICESYSTEMSTANDARDS: {
#define EXTERNFUNC(name) \
    extern int name(void*, void*, int)

            EXTERNFUNC(_rwDlCameraBeginUpdate);
            EXTERNFUNC(_rwDlCameraEndUpdate);
            EXTERNFUNC(_rwDlCameraClear);
            EXTERNFUNC(_rwDlRasterShowRaster);
            EXTERNFUNC(_rwDlRGBToPixel);
            EXTERNFUNC(_rwDlPixelToRGB);
            EXTERNFUNC(_rwDlRasterSetFromImage);
            EXTERNFUNC(_rwDlImageGetFromRaster);
            EXTERNFUNC(_rwDlRasterDestroy);
            EXTERNFUNC(_rwDlRasterCreate);
            EXTERNFUNC(_rwDlImageFindRasterFormat);
            EXTERNFUNC(_rwDlTextureSetRaster);
            EXTERNFUNC(_rwDlRasterLock);
            EXTERNFUNC(_rwDlRasterUnlock);
            EXTERNFUNC(_rwDlRasterLockPalette);
            EXTERNFUNC(_rwDlRasterUnlockPalette);
            EXTERNFUNC(_rwDlRasterClear);
            EXTERNFUNC(_rwDlRasterClearRect);
            EXTERNFUNC(_rwDlRasterRender);
            EXTERNFUNC(_rwDlRasterRenderScaled);
            EXTERNFUNC(_rwDlRasterRenderFast);
            EXTERNFUNC(_rwDlSetRasterContext);
            EXTERNFUNC(_rwDlRasterSubRaster);
            EXTERNFUNC(_rwDlNativeTextureGetSize);
            EXTERNFUNC(_rwDlNativeTextureWrite);
            EXTERNFUNC(_rwDlNativeTextureRead);
            EXTERNFUNC(_rwDlRasterGetNumMipLevels);

            struct UnkRwStandardFnLoad stdFuncs[] = {
                { 1, _rwDlCameraBeginUpdate },
                { 10, _rwDlCameraEndUpdate },
                { 21, _rwDlCameraClear },
                { 20, _rwDlRasterShowRaster },
                { 2, _rwDlRGBToPixel },
                { 3, _rwDlPixelToRGB },
                { 7, _rwDlRasterSetFromImage },
                { 6, _rwDlImageGetFromRaster },
                { 5, _rwDlRasterDestroy },
                { 4, _rwDlRasterCreate },
                { 9, _rwDlImageFindRasterFormat },
                { 8, _rwDlTextureSetRaster },
                { 15, _rwDlRasterLock },
                { 16, _rwDlRasterUnlock },
                { 23, _rwDlRasterLockPalette },
                { 24, _rwDlRasterUnlockPalette },
                { 14, _rwDlRasterClear },
                { 13, _rwDlRasterClearRect },
                { 17, _rwDlRasterRender },
                { 18, _rwDlRasterRenderScaled },
                { 19, _rwDlRasterRenderFast },
                { 11, _rwDlSetRasterContext },
                { 12, _rwDlRasterSubRaster },
                { 25, _rwDlNativeTextureGetSize },
                { 27, _rwDlNativeTextureWrite },
                { 26, _rwDlNativeTextureRead },
                { 28, _rwDlRasterGetNumMipLevels },
            };

            int i;
            int n = 27;

            for (i = 0; i < param; i++) {
                ((RwStandardFunc*)dest)[i] = _rwDlNullStandard;
            }

            while (n--) {
                if (stdFuncs[0].id < param && stdFuncs[0].id >= 0) {
                    ((RwStandardFunc*)dest)[stdFuncs[n].id] = stdFuncs[n].func;
                }
            }

            return 1;
        }
        case rwDEVICESYSTEMGETMAXTEXTURESIZE:
            *(int*)dest = 1024;
            return 1;
        default:
            return 0;
    }
}

extern unsigned short _RwDlTokenCurrent; // initialized to 1
// Incomplete
int _rwDlRasterShowRaster(void) {
    void* auStack_10[4]; // is this actually 4?
    void* breakpoint;
    int oldIntr;
    volatile int shouldSleep;

    oldIntr = OSDisableInterrupts();
    shouldSleep = (_RwDlFrameNew - _RwDlFrameCurrent == -1 || _RwDlFrameNew - _RwDlFrameCurrent == 2) != 0;
    OSRestoreInterrupts(oldIntr);

    if (shouldSleep == 1) {
        OSSleepThread(&_RwDlWaitingDoneRender);
    }

    if (_RwDlFSAA == 0) {
        GXFlush();
        GXGetFifoPtrs(GXGetCPUFifo(), auStack_10, &breakpoint);

        oldIntr = OSDisableInterrupts();

        // Nonmatching
        _RwGCFrameQueue[_RwDlFrameNew].breakpoint = breakpoint;
        _RwGCFrameQueue[_RwDlFrameNew].unkXfb = _RwGCXFBCopy;
        _RwDlFrameSwap[_RwDlFrameTokenNew] = _RwDlTokenCurrent;
        _RwDlFrameNew = (_RwDlFrameNew + 1) % 3;

        _RwDlFrameTokenNew = (_RwDlFrameTokenNew + 1) % 3;
        OSRestoreInterrupts(oldIntr);

        if (_RwDlBreakPointEnabled == 0) {
            _RwDlBreakPointEnabled = 1;
            GXEnableBreakPt(breakpoint);
        }

        GXCopyDisp(_RwGCXFBCopy, _RwDlCopyClear);
        GXSetDrawSync(_RwDlTokenCurrent);
        _RwDlTokenCurrent = (_RwDlTokenCurrent + 1) % 0xE000;
        GXFlush();

        _RwGCXFBCopy = (_RwGCXFBCopy == _RwGCXFB1) ? _RwGCXFB2 : _RwGCXFB1;
    } else if (_RwDlFSAATop != 0) {
        GXFlush();
        GXGetFifoPtrs(GXGetCPUFifo(), auStack_10, &breakpoint);
        oldIntr = OSDisableInterrupts();

        // Nonmatching
        _RwGCFrameQueue[_RwDlFrameNew].breakpoint = breakpoint;
        _RwGCFrameQueue[_RwDlFrameNew].unkXfb = _RwGCXFBCopy;
        _RwDlFrameNew = (_RwDlFrameNew + 1) % 3;

        OSRestoreInterrupts(oldIntr);
        if (_RwDlBreakPointEnabled == 0) {
            _RwDlBreakPointEnabled = 1;
            GXEnableBreakPt(breakpoint);
        }
        GXCopyDisp(_RwGCXFBCopy, _RwDlCopyClear);
        GXSetDrawSync(_RwDlTokenCurrent);
        _RwDlTokenCurrent = (_RwDlTokenCurrent + 1) % 0xE000;
        GXFlush();

        _RwDlFSAATop = 0;
    } else {
        GXFlush();
        GXGetFifoPtrs(GXGetCPUFifo(), auStack_10, &breakpoint);

        oldIntr = OSDisableInterrupts();

        // Nonmatching
        _RwGCFrameQueue[_RwDlFrameNew].breakpoint = breakpoint;
        _RwGCFrameQueue[_RwDlFrameNew].unkXfb = _RwGCXFBCopy;
        _RwDlFrameSwap[_RwDlFrameTokenNew] = _RwDlTokenCurrent;
        _RwDlFrameNew = (_RwDlFrameNew + 1) % 3;

        _RwDlFrameTokenNew = (_RwDlFrameTokenNew + 1) % 3;
        OSRestoreInterrupts(oldIntr);

        if (_RwDlBreakPointEnabled == 0) {
            _RwDlBreakPointEnabled = 1;
            GXEnableBreakPt(breakpoint);
        }
        GXSetCopyClamp(GX_CLAMP_BOTTOM);
        GXSetDispCopySrc(0, 2, _RwDlRenderMode->fbWidth, _RwDlRenderMode->efbHeight - 2);
        GXCopyDisp((void*)((int)_RwGCXFBCopy + ((_RwDlRenderMode->fbWidth + 0xF) & ~0xFFFF000F) * (_RwDlRenderMode->efbHeight - 2) * 2), _RwDlCopyClear);
        GXSetDrawSync(_RwDlTokenCurrent);
        _RwDlTokenCurrent = (_RwDlTokenCurrent + 1) % 0xE000;
        GXSetCopyClamp((GXFBClamp)(GX_CLAMP_TOP | GX_CLAMP_BOTTOM));
        GXSetDispCopySrc(0, 0, _RwDlRenderMode->fbWidth, _RwDlRenderMode->efbHeight);
        GXFlush();

        _RwGCXFBCopy = (_RwGCXFBCopy == _RwGCXFB1) ? _RwGCXFB2 : _RwGCXFB1;
        _RwDlFSAATop = 1;
    }

    return 1;
}

extern int _rwDlSetRenderState(RwRenderState, void*);
extern int _rwDlGetRenderState(RwRenderState, void*);

RwDevice* _rwDeviceGetHandle(void) {
    static RwDevice rwDlDriverDevice = {
        1.0f,
        _rwDlSystem,
        0.0f,
        0.99999994, // ?
        _rwDlSetRenderState,
        _rwDlGetRenderState,
        _rwDlIm2DRenderLine,
        _rwDlIm2DRenderTriangle,
        _rwDlIm2DRenderPrimitive,
        _rwDlIm2DRenderIndexedPrimitive,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return &rwDlDriverDevice;
}

#ifdef __cplusplus
}
#endif
