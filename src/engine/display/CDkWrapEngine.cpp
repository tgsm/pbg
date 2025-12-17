#include "engine/display/CDkWrapEngine.h"
#include "display/CEngine.h"
#include "display/CTextureDictionary.h"

namespace DKDSP {

BOOL CDkWrapEngine::m_bEnableShowWindow = TRUE;

CDkWrapEngine::CDkWrapEngine(RwEngineOpenParams* params, void* a1) : CDKW_GCNEngine(params, 0x90000) {
    m_unk20 = a1;
    m_unk34 = 0;
    m_unk48 = 0;
}

CDkWrapEngine::~CDkWrapEngine() {

}

BOOL CDkWrapEngine::Update() {
    return CDKW_Engine::EventHandler(DKW_EVENT_18, NULL) != FALSE;
}

RwRect CDkWrapEngine::GetRect() {
    return m_rect;
}

BOOL CDkWrapEngine::OnPlugAttach() {
    CTextureDictionary::RegisterCallbacks();
    if (CEngine::ms_pInitCallback != NULL) {
        CEngine::ms_pInitCallback->func8(CEngine::ms_pEngine);
    }
    return TRUE;
}

BOOL CDkWrapEngine::OnResize(RwRect* rect) {
    if (rect != NULL) {
        m_rect = *rect;
    }
    return TRUE;
}

extern "C" {
typedef struct RwEngineVideoModeInfo {
    char unk0[24];
} RwEngineVideoModeInfo;

extern int RwEngineGetCurrentVideoMode(void);
extern void RwEngineGetVideoModeInfo(RwEngineVideoModeInfo* info, int);
}

BOOL CDkWrapEngine::OnInit(int argc, char** argv) {
    RwEngineVideoModeInfo info;
    RwEngineGetVideoModeInfo(&info, RwEngineGetCurrentVideoMode());
    return TRUE;
}

BOOL CDkWrapEngine::OnIdle() {
    return TRUE;
}

BOOL CDkWrapEngine::OnTerm() {
    return TRUE;
}

}
