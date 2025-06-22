#include <dolphin/vi.h>
#include <THP/THPPlayer.h>
#include "engine/video/CVideoEngineGCN.h"

namespace DKVIDEO {

CVideoEngineGCN::CVideoEngineGCN() {
    m_fb1 = NULL;
    m_fb2 = NULL;
    m_active_fb = NULL;
    m_render_mode_obj = NULL;
    m_stopped = FALSE;
    m_unk20 = 0;
}

CVideoEngineGCN::~CVideoEngineGCN() {

}

extern "C" void RwaCoreSuspend(void);
extern "C" void RwaCoreResume(void);

void CVideoEngineGCN::Play(char* filename) {
    RwaCoreSuspend();

    u8* data = NULL;
    THPVideoInfo video_info;
    THPAudioInfo audio_info;

    m_render_mode_obj = &GXNtsc480IntDf;
    Init(m_render_mode_obj);
    InitGX();
    InitVI();

    THPPlayerInit(1);
    m_unk20 = TRUE;
    m_stopped = FALSE;

    BOOL bVar1 = TRUE;
    BOOL playing = FALSE;
    int x, y;
    while (!m_stopped) {
        BeforeRender();
        if (playing) {
            THPPlayerDrawCurrentFrame(m_render_mode_obj, x, y, video_info.mXSize, video_info.mYSize);
        }
        THPPlayerDrawDone();
        DoneRender();

        int state = THPPlayerGetState();
        if (state == 3) {
            m_stopped = TRUE;
        } else if (state == 5) {
            break;
        }

        if (bVar1 && playing && data != NULL) {
            THPPlayerStop();
            THPPlayerClose();
            OSFree(data);
            playing = FALSE;
        }

        if (bVar1 && !playing) {
            if (!THPPlayerOpen(filename, FALSE)) {
                break;
            }

            THPPlayerSetVolume(127.0f * m_volume, 0);

            THPPlayerGetVideoInfo(&video_info);
            THPPlayerGetAudioInfo(&audio_info);

            x = (m_render_mode_obj->fbWidth - video_info.mXSize) / 2;
            y = (m_render_mode_obj->efbHeight - video_info.mYSize) / 2;

            data = (u8*)OSAlloc(THPPlayerCalcNeedMemory());
            if (data == NULL) {
                break;
            }
            THPPlayerSetBuffer(data);

            int audio_track;
            if (audio_info.mSndNumTracks != 1) {
                audio_track = OSGetTick() % audio_info.mSndNumTracks;
            } else {
                audio_track = 0;
            }

            if (!THPPlayerPrepare(0, 0, audio_track)) {
                break;
            }
            THPPlayerPlay();

            bVar1 = FALSE;
            playing = TRUE;
        }

        if (m_callback != NULL) {
            m_callback();
        }
    }

    if (data != NULL) {
        THPPlayerStop();
        THPPlayerClose();
        OSFree(data);
    }

    THPPlayerQuit();
    VIWaitForRetrace();

    BeforeRender();
    DoneRender();

    RwaCoreResume();
}

void CVideoEngineGCN::Stop() {
    m_stopped = TRUE;
}

extern "C" void RwGameCubeGetXFBs(void*, void*);

void CVideoEngineGCN::Init(GXRenderModeObj*) {
    RwGameCubeGetXFBs(&m_fb1, &m_fb2);
    m_active_fb = m_fb2;
}

void CVideoEngineGCN::InitGX() {
    GXSetViewport(0.0f, 0.0f, m_render_mode_obj->fbWidth, m_render_mode_obj->efbHeight, 0.0f, 1.0f);
    GXSetScissor(0, 0, m_render_mode_obj->fbWidth, m_render_mode_obj->efbHeight);

    U16 lines = GXSetDispCopyYScale(GXGetYScaleFactor(m_render_mode_obj->efbHeight, m_render_mode_obj->xfbHeight));
    GXSetDispCopySrc(0, 0, m_render_mode_obj->fbWidth, m_render_mode_obj->efbHeight);
    GXSetDispCopyDst(m_render_mode_obj->fbWidth, lines);

    GXSetCopyFilter(m_render_mode_obj->aa, m_render_mode_obj->sample_pattern, GX_TRUE, m_render_mode_obj->vfilter);
    if (m_render_mode_obj->aa) {
        GXSetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
    } else {
        GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
    }

    GXSetDispCopyGamma(GX_GM_1_0);
    GXColor clear_color = { 0x00, 0x00, 0x00, 0xFF };
    GXSetCopyClear(clear_color, 0xFFFFFF);
    GXCopyDisp(m_active_fb, GX_TRUE);

    GXSetCoPlanar(GX_FALSE);
    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);

    GXSetScissor(0, 0, m_render_mode_obj->fbWidth, m_render_mode_obj->efbHeight);
    GXSetScissorBoxOffset(0, 0);

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD2, GX_TEXMAP2, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD3, GX_TEXMAP3, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE4, GX_TEXCOORD4, GX_TEXMAP4, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE5, GX_TEXCOORD5, GX_TEXMAP5, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE6, GX_TEXCOORD6, GX_TEXMAP6, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE7, GX_TEXCOORD7, GX_TEXMAP7, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE8, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE9, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE10, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE11, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE12, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE13, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE14, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE15, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);

    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z8, 0);

    for (int i = GX_TEVSTAGE0; i < GX_MAX_TEVSTAGE; i++) {
        GXSetTevKColorSel((GXTevStageID)i, GX_TEV_KCSEL_1_4);
        GXSetTevKAlphaSel((GXTevStageID)i, GX_TEV_KASEL_1);
        GXSetTevSwapMode((GXTevStageID)i, GX_TEV_SWAP0, GX_TEV_SWAP0);
    }

    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
}

void CVideoEngineGCN::InitVI() {
    VISetNextFrameBuffer(m_fb1);
    m_active_fb = m_fb2;

    VIFlush();
    VIWaitForRetrace();

    if ((u32)m_render_mode_obj->viTVmode & VI_NON_INTERLACE) {
        VIWaitForRetrace();
    }
}

void CVideoEngineGCN::BeforeRender() {
    if (m_render_mode_obj->field_rendering) {
        GXSetViewportJitter(0.0f, 0.0f, m_render_mode_obj->fbWidth, m_render_mode_obj->efbHeight, 0.0f, 1.0f, VIGetNextField() ^ 1);
    } else {
        GXSetViewport(0.0f, 0.0f, m_render_mode_obj->fbWidth, m_render_mode_obj->efbHeight, 0.0f, 1.0f);
    }
}

void CVideoEngineGCN::DoneRender() {
    GXFlush();
    GXDrawDone();

    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetColorUpdate(GX_TRUE);
    GXCopyDisp(m_active_fb, GX_TRUE);
    GXDrawDone();

    SwapBuffers();
}

void CVideoEngineGCN::SwapBuffers() {
    VISetNextFrameBuffer(m_active_fb);
    if (m_unk20) {
        VISetBlack(FALSE);
        m_unk20 = FALSE;
    }

    VIFlush();
    VIWaitForRetrace();

    m_active_fb = (m_active_fb == m_fb1) ? m_fb2 : m_fb1;
}

void CVideoEngineGCN::SetTevColorIn(GXTevStageID stage_id, GXTevColorArg a2, GXTevColorArg a3, GXTevColorArg a4, GXTevColorArg a5) {
    U32 tex_sel_var = 0; // idk what to call this

    if (a2 == GX_CC_TEXC) {
        tex_sel_var = GX_CC_ZERO;
    } else if (a2 >= GX_CC_TEXRRR) {
        tex_sel_var = a2;
        a2 = GX_CC_TEXC;
    }

    if (a3 == GX_CC_TEXC) {
        tex_sel_var = GX_CC_ZERO;
    } else if (a3 >= GX_CC_TEXRRR) {
        tex_sel_var = a3;
        a3 = GX_CC_TEXC;
    }

    if (a4 == GX_CC_TEXC) {
        tex_sel_var = GX_CC_ZERO;
    } else if (a4 >= GX_CC_TEXRRR) {
        tex_sel_var = a4;
        a4 = GX_CC_TEXC;
    }

    if (a5 == GX_CC_TEXC) {
        tex_sel_var = GX_CC_ZERO;
    } else if (a5 >= GX_CC_TEXRRR) {
        tex_sel_var = a5;
        a5 = GX_CC_TEXC;
    }

    GXSetTevColorIn(stage_id, a2, a3, a4, a5);
    if (tex_sel_var != 0) {
        GXSetTevSwapMode(stage_id, GX_TEV_SWAP0, (GXTevSwapSel)(tex_sel_var - GX_CC_ZERO));
    }
}

}
