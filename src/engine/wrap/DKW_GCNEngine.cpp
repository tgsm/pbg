#include <cstdlib>
#include <dolphin/dvd.h>
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <dolphin/pad.h>
#include <dolphin/vi.h>
#include "engine/wrap/DKW_GCNEngine.h"

extern "C" {
extern void DolphinInitMemorySystem(MemoryFunctionsStruct*);
extern void DolphinInstallFileSystem(void);
}

MemoryFunctionsStruct MemoryFunctions;

s64 CDKW_GCNEngine::ms_InitialTime;
static BOOL IsHeapInitialized;

void _printMessages(const char*) {

}

void* operator new(size_t size) {
    if (!IsHeapInitialized) {
        void* temp = malloc(1024);
        if (temp != NULL) {
            free(temp);
        }
    }

    return OSAllocFromHeap(0, size);
}

void* operator new[](size_t size) {
    return operator new(size);
}

void operator delete(void* ptr) {
    OSFreeToHeap(0, ptr);
}

void operator delete[](void* ptr) {
    operator delete(ptr);
}

void CDKW_GCNEngine::GCNInit() {
    GXSetMisc(GX_MT_XF_FLUSH, 8);
    OSInit();
    DVDInit();
    VIInit();
    DolphinInitMemorySystem(&MemoryFunctions);
    ms_InitialTime = ::OSGetTime();
}

MemoryFunctionsStruct* CDKW_GCNEngine::MemorySetting() {
    return &MemoryFunctions;
}

void CDKW_GCNEngine::InstallFileSystem() {
    DolphinInstallFileSystem();
}

s64 CDKW_GCNEngine::OSGetTime() {
    return ::OSGetTime() - ms_InitialTime;
}

s32 CDKW_GCNEngine::Timer() {
    return OSGetTime() / (OS_TIMER_CLOCK / 1000);
}

f32 CDKW_GCNEngine::GetTimer() {
    return (f32)(OSGetTime()) / OS_TIMER_CLOCK;
}

f32 CDKW_GCNEngine::GetDeltaTimer() {
    static s64 oldtime;
    s64 currenttime = OSGetTime();
    s64 delta = currenttime - oldtime;
    oldtime = currenttime;
    return (f32)(delta) / OS_TIMER_CLOCK;
}

void CDKW_GCNEngine::SaveTimer() {
    m_saved_time = ::OSGetTime() - ms_InitialTime;
}

f32 CDKW_GCNEngine::GetDeltaTimerSinceSaved() {
    return (f32)(OSGetTime() - m_saved_time) / OS_TIMER_CLOCK;
}

void CDKW_GCNEngine::OnPadButtonDown(DKW_PadButtonStatus*) {

}

void CDKW_GCNEngine::OnPadButtonUp(DKW_PadButtonStatus*) {

}

void CDKW_GCNEngine::OnPadAnalogRight(CDKW_V2d*) {

}

void CDKW_GCNEngine::OnPadAnalogLeft(CDKW_V2d*) {

}

void CDKW_GCNEngine::InstallDebugMessage() {

}

BOOL CDKW_GCNEngine::PS_Init() {
    PADInit();
    return TRUE;
}

void CDKW_GCNEngine::EngineInit(int argc, char** argv) {
    GXSetMisc(GX_MT_XF_FLUSH, 8);
    OSInit();
    DVDInit();
    VIInit();

    CDKW_Engine::EngineInit(argc, argv);
}

void CDKW_GCNEngine::PreInit(RwEngineOpenParams* params) {
    GXSetMisc(GX_MT_XF_FLUSH, 8);
    OSInit();
    DVDInit();
    VIInit();

    CDKW_Engine::PreInit(params);
}
