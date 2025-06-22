#ifndef ENGINE_VIDEO_CVIDEOENGINEGCN_H
#define ENGINE_VIDEO_CVIDEOENGINEGCN_H

#include <dolphin/gx.h>
#include "engine/video/CVideoEngine.h"

namespace DKVIDEO {

class CVideoEngineGCN : public CVideoEngine {
private:
    void* m_fb1;
    void* m_fb2;
    void* m_active_fb;
    GXRenderModeObj* m_render_mode_obj;
    BOOL m_stopped;
    BOOL m_unk20;

public:
    CVideoEngineGCN();
    virtual ~CVideoEngineGCN();

    virtual void Play(char* filename);
    virtual void Stop();
    virtual void Init(GXRenderModeObj*);
    virtual void InitGX();
    virtual void InitVI();
    virtual void SwapBuffers();
    virtual void BeforeRender();
    virtual void DoneRender();
    virtual void SetTevColorIn(GXTevStageID stage_id, GXTevColorArg, GXTevColorArg, GXTevColorArg, GXTevColorArg);
};
REQUIRE_SIZE(CVideoEngineGCN, 0x24);

}

#endif
