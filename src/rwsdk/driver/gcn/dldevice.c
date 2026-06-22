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
RwBool _RwDlFSAATop = TRUE;
RwInt32 _RwDlFifoSize = 0x40000;
static RwBool _RwDlFirstFrame = TRUE;
static RwBool _RwDlCopyClear;
RwInt32 _RwDlPixelFormat;
RwInt32 _RwDlCurPixelFormat;
RwInt32 _RwGameCubeVideoMode;
RwBool _RwDlFSAA;
static void* _RwDl_FIFO_XFB;
static void* _RwGCXFB1;
static void* _RwGCXFB2;
static void* _RwGCXFBCopy;
static void* _RwGCXFBDisp;
volatile static RwInt32 _RwDlFrameCurrent;
volatile static RwInt32 _RwDlFrameNew;
volatile static RwInt32 _RwDlFrameTokenNew;
volatile static RwInt32 _RwDlFrameTokenCurrent;
static RwBool _RwDlBreakPointEnabled;
volatile static RwBool _RwDlFrameReadyOnToken;
static RwBool _RwDlFrameWait;
static RwBool _RwDlFrameGo;
static OSThreadQueue _RwDlWaitingDoneRender;
static RwUInt16 _RwDlFrameSwap[3]; // struct?
static void* _RwDlDefaultFifo;
static GXFifoObj* _RwDlDefaultFifoObj;
RwInt32 _RwDlHalfHeight;

struct UnkRwStandardFnLoad {
    RwInt32 id;
    RwStandardFunc func;
};

struct UnkRwGCFrameQueueSub {
    void* unkXfb;
    void* breakpoint;
};
static struct UnkRwGCFrameQueueSub _RwGCFrameQueue[3];
static GXRenderModeObj _RwGameCubeRenderModeObj;

static void _rwDlBreakNext(void) {
    static RwBool swap;
    RwUInt32 currentFrame;

    DONT_INLINE_HACK();

    currentFrame = (_RwDlFrameCurrent + 1) % 3;
    if (currentFrame == _RwDlFrameNew) {
        GXDisableBreakPt();
        _RwDlBreakPointEnabled = FALSE;
    } else {
        GXEnableBreakPt(_RwGCFrameQueue[currentFrame].breakpoint);
    }
    _RwDlFrameCurrent = currentFrame;

    OSWakeupThread(&_RwDlWaitingDoneRender);

    if (!_RwDlFSAA) {
        _RwDlFrameReadyOnToken = TRUE;
    } else if (swap == TRUE) {
        _RwDlFrameReadyOnToken = TRUE;
        swap = FALSE;
    } else {
        swap = TRUE;
    }
}

static void _rwDlBreakPtCallback(void) {
    if (_RwGCFrameQueue[_RwDlFrameCurrent].unkXfb != _RwGCXFBDisp) {
        _rwDlBreakNext();
    } else {
        _RwDlFrameWait = TRUE;
    }
}

extern int _rwDlTokenQueryDone(RwUInt16);
GXRenderModeObj* _RwDlRenderMode;

// Equivalent: _RwGCXFBDisp reload
static void _rwDlVIPreRetraceCallback(u32 retraceCount) {
    RwInt16 token = _RwDlFrameSwap[_RwDlFrameTokenCurrent];
    if (_RwDlFrameReadyOnToken == TRUE && _rwDlTokenQueryDone(token)) {
        _RwGCXFBDisp = (_RwGCXFBDisp == _RwGCXFB1) ? _RwGCXFB2 : _RwGCXFB1;
        VISetNextFrameBuffer(_RwGCXFBDisp);

        if (_RwDlFirstFrame) {
            VISetBlack(FALSE);
            _RwDlFirstFrame = FALSE;
        }

        VIFlush();

        _RwDlFrameTokenCurrent = (_RwDlFrameTokenCurrent + 1) % 3;
        _RwDlFrameReadyOnToken = FALSE;
        _RwDlFrameGo = TRUE;
    }
}

static void _rwDlVIPostRetraceCallback(u32 retraceCount) {
    if (_RwDlFrameWait && _RwDlFrameGo) {
        _rwDlBreakNext();
        _RwDlFrameWait = FALSE;
    }
    _RwDlFrameGo = FALSE;
}

static RwBool _rwDlNullStandard(void*, void*, RwInt32) {
    return FALSE;
}

// Equivalent?: _RwGameCubeRenderModeObj copy scheduling
void _rwDlRenderModeSelect(GXRenderModeObj* renderMode, RwInt32 pixelFormat) {
    if (renderMode != NULL) {
        RwInt32 pf;
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

void _rwDlRenderModeInit(GXRenderModeObj* renderMode, RwInt32 pixelFormat) {
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
        _RwDlFSAA = TRUE;
        _RwDlFSAATop = TRUE;
    } else {
        GXSetDispCopyYScale((f32)renderMode->xfbHeight / (f32)renderMode->efbHeight);
        _RwDlFSAA = FALSE;
    }

    GXSetFieldMode(renderMode->field_rendering, renderMode->xfbHeight < renderMode->viHeight);
    GXSetPixelFmt(pixelFormat, GX_ZC_LINEAR);

    _RwDlFirstFrame = TRUE;
}

struct {
    void* unk0;
    void* unk4;
} dgGGlobals;
struct UnkRenderModeStruct {
    GXRenderModeObj* gxobj;
    RwInt32 pixelformat;
    RwInt32 fifosize;
};
extern void _rwDlRenderStateOpen(void);
extern void _rwDlRenderStateClose(void);
extern void vectorASMMultPoint();
extern void vectorASMMultVector();
extern void _rwVectorSetMultFn(void*, void*);
extern volatile RwUInt16 _RwDlTokenLastSeen;

// Equivalent: regalloc
// This function is affected by `-lang c` vs. `-lang c++`, the _RwDlVideoModes copies
// and the dgGGlobals write produce different codegen patterns.
RwBool _rwDlSystem(RwInt32 fn, void* dest, void* a2, RwInt32 param) {
    switch (fn) {
        case rwDEVICESYSTEMUSEMODE:
            return (param < 1);
        case rwDEVICESYSTEMGETNUMMODES:
            *(RwInt32*)dest = 1;
            return TRUE;
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
                return TRUE;
            } else {
                return FALSE;
            }
            break;
        case rwDEVICESYSTEMGETMODE:
            *(RwInt32*)dest = 0;
            return TRUE;
        case rwDEVICESYSTEMFOCUS:
            return TRUE;
        case rwDEVICESYSTEMREGISTER:
            *(RwDevice*)dest = *_rwDeviceGetHandle();
            dgGGlobals.unk4 = a2;
            return TRUE;
        case rwDEVICESYSTEMOPEN: {
            RwInt32 iVar15;
            struct UnkRenderModeStruct* unk = *(struct UnkRenderModeStruct**)a2;
            if (unk != NULL) {
                _rwDlRenderModeSelect(unk->gxobj, unk->pixelformat);
                _RwDlFifoSize = unk->fifosize;
            } else {
                _rwDlRenderModeSelect(NULL, _RwDlPixelFormat);
            }

            iVar15 = ((_RwDlRenderMode->fbWidth + 0xF) & 0xFFF0) * _RwDlRenderMode->xfbHeight * 2;
            _RwDl_FIFO_XFB = RwMalloc(_RwDlFifoSize + iVar15 * 2 + 0x1F);
            _RwDlDefaultFifo = (void*)(((RwInt32)_RwDl_FIFO_XFB + 0x1F) & ~0x1F);
            DCInvalidateRange(_RwDlDefaultFifo, _RwDlFifoSize);
            _RwGCXFBDisp = (void*)((RwInt32)_RwDlDefaultFifo + _RwDlFifoSize);
            _RwGCXFB1 = _RwGCXFBDisp;
            _RwGCXFB2 = (void*)((RwInt32)_RwGCXFB1 + iVar15);
            _RwGCXFBCopy = _RwGCXFB2;

            return TRUE;
        }
        case rwDEVICESYSTEMCLOSE:
            GXFlush();
            RwFree(_RwDl_FIFO_XFB);
            _RwGCXFBDisp = NULL;
            _RwDlDefaultFifoObj = NULL;
            _RwGCXFB1 = NULL;
            _RwGCXFB2 = NULL;
            _RwGCXFBCopy = NULL;
            _RwDlDefaultFifo = NULL;
            _RwDl_FIFO_XFB = NULL;
            return TRUE;
        case rwDEVICESYSTEMSTART: {
            static RwBool gxInit;
            if (!gxInit) {
                _RwDlDefaultFifoObj = GXInit(_RwDlDefaultFifo, _RwDlFifoSize);
                gxInit = TRUE;
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

            return TRUE;
        }
        case rwDEVICESYSTEMFINALIZESTART:
            _rwVectorSetMultFn(vectorASMMultPoint, vectorASMMultVector);
            _rwDlRenderStateOpen();
            return TRUE;
        case rwDEVICESYSTEMINITIATESTOP:
            _rwDlRenderStateClose();
            return TRUE;
        case rwDEVICESYSTEMSTOP:
            return TRUE;
        case rwDEVICESYSTEMINITPIPELINE:
            return TRUE;
        case rwDEVICESYSTEMSTANDARDS: {
#define EXTERNFUNC(name) \
    extern RwBool name(void*, void*, RwInt32)

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

            RwInt32 i;
            RwInt32 n = 27;

            for (i = 0; i < param; i++) {
                ((RwStandardFunc*)dest)[i] = _rwDlNullStandard;
            }

            while (n--) {
                if (stdFuncs[0].id < param && stdFuncs[0].id >= 0) {
                    ((RwStandardFunc*)dest)[stdFuncs[n].id] = stdFuncs[n].func;
                }
            }

            return TRUE;
        }
        case rwDEVICESYSTEMGETMAXTEXTURESIZE:
            *(RwInt32*)dest = 1024;
            return TRUE;
        default:
            return FALSE;
    }
}

extern RwUInt16 _RwDlTokenCurrent; // initialized to 1
// Incomplete
RwBool _rwDlRasterShowRaster(void*, void*, RwInt32) {
    void* auStack_10[4]; // is this actually 4?
    void* breakpoint;
    RwBool oldIntr;
    volatile RwBool shouldSleep;

    oldIntr = OSDisableInterrupts();
    shouldSleep = (_RwDlFrameNew - _RwDlFrameCurrent == -1 || _RwDlFrameNew - _RwDlFrameCurrent == 2) != FALSE;
    OSRestoreInterrupts(oldIntr);

    if (shouldSleep == TRUE) {
        OSSleepThread(&_RwDlWaitingDoneRender);
    }

    if (!_RwDlFSAA) {
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

        if (!_RwDlBreakPointEnabled) {
            _RwDlBreakPointEnabled = TRUE;
            GXEnableBreakPt(breakpoint);
        }

        GXCopyDisp(_RwGCXFBCopy, _RwDlCopyClear);
        GXSetDrawSync(_RwDlTokenCurrent);
        _RwDlTokenCurrent = (_RwDlTokenCurrent + 1) % 0xE000;
        GXFlush();

        _RwGCXFBCopy = (_RwGCXFBCopy == _RwGCXFB1) ? _RwGCXFB2 : _RwGCXFB1;
    } else if (_RwDlFSAATop) {
        GXFlush();
        GXGetFifoPtrs(GXGetCPUFifo(), auStack_10, &breakpoint);
        oldIntr = OSDisableInterrupts();

        // Nonmatching
        _RwGCFrameQueue[_RwDlFrameNew].breakpoint = breakpoint;
        _RwGCFrameQueue[_RwDlFrameNew].unkXfb = _RwGCXFBCopy;
        _RwDlFrameNew = (_RwDlFrameNew + 1) % 3;

        OSRestoreInterrupts(oldIntr);
        if (!_RwDlBreakPointEnabled) {
            _RwDlBreakPointEnabled = TRUE;
            GXEnableBreakPt(breakpoint);
        }
        GXCopyDisp(_RwGCXFBCopy, _RwDlCopyClear);
        GXSetDrawSync(_RwDlTokenCurrent);
        _RwDlTokenCurrent = (_RwDlTokenCurrent + 1) % 0xE000;
        GXFlush();

        _RwDlFSAATop = FALSE;
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

        if (!_RwDlBreakPointEnabled) {
            _RwDlBreakPointEnabled = TRUE;
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
        _RwDlFSAATop = TRUE;
    }

    return TRUE;
}

extern RwBool _rwDlSetRenderState(RwRenderState, void*);
extern RwBool _rwDlGetRenderState(RwRenderState, void*);

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
