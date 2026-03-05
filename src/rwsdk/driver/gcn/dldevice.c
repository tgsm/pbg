#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <dolphin/vi.h>

int _RwDlFSAATop = 1;
static int _RwDlFirstFrame = 1;
extern unsigned short _RwDlTokenCurrent; // initialized to 1
static int _RwDlCopyClear;
int _RwDlFSAA;
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
GXRenderModeObj* _RwDlRenderMode;

struct UnkRwGCFrameQueueSub {
    void* unkXfb;
    void* breakpoint;
};
struct UnkRwGCFrameQueueSub _RwGCFrameQueue[3];

static void _rwDlBreakNext(void) {
    static int swap;
    unsigned int currentFrame;

    // FIXME: Hack to prevent inlining
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;

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

// Equivalent: _RwGCXFBDisp reload
static void _rwDlVIPreRetraceCallback(void) {
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

static void _rwDlVIPostRetraceCallback(void) {
    if (_RwDlFrameWait && _RwDlFrameGo) {
        _rwDlBreakNext();
        _RwDlFrameWait = 0;
    }
    _RwDlFrameGo = 0;
}

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
        GXSetCopyClamp(GX_CLAMP_TOP | GX_CLAMP_BOTTOM);
        GXSetDispCopySrc(0, 0, _RwDlRenderMode->fbWidth, _RwDlRenderMode->efbHeight);
        GXFlush();

        _RwGCXFBCopy = (_RwGCXFBCopy == _RwGCXFB1) ? _RwGCXFB2 : _RwGCXFB1;
        _RwDlFSAATop = 1;
    }

    return 1;
}
