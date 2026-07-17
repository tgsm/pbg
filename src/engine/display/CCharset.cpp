#include "engine/display/CCharset.h"
#include "engine/display/DkDisplay.h"
#include "engine/wrap/DKW_Engine.h"
#include <stdarg.h>

extern "C" {
extern void RtCharsetSetColors(RtCharset*, RwRGBA*, RwRGBA*);
}

namespace DKDSP {

CCharset::CCharset() {
    m_wrap_charset = NULL;
    m_unk8 = FALSE;
    m_foreground_color = CDKW_Engine::GetForegroundColor();
    m_background_color = CDKW_Engine::GetBackgroundColor();
}

CCharset::~CCharset() {
    Release();
}

void CCharset::Release() {
    if (m_unk8 == TRUE && m_wrap_charset != NULL) {
        delete m_wrap_charset;
        m_wrap_charset = NULL;
    }
    m_unk8 = FALSE;
}

void CCharset::SetDkWrapCharset(CDKW_Charset* wrap_charset, BOOL a2) {
    if (!m_unk8) {
        m_wrap_charset = wrap_charset;
        m_unk8 = a2;
    }
}

void CCharset::SetColors(const CDKW_RGBA& foreground, const CDKW_RGBA& background) {
    if (m_wrap_charset != NULL) {
        m_foreground_color = foreground;
        m_background_color = background;
        RtCharsetSetColors(m_wrap_charset->m_rw_charset, (RwRGBA*)&foreground, (RwRGBA*)&background);
    }
}

void CCharset::SetForeGroundColor(F32 r, F32 g, F32 b, F32 a) {
    if (m_wrap_charset != NULL) {
        m_foreground_color = CDKW_RGBA(255.0f * r, 255.0f * g, 255.0f * b, 255.0f * a);
        RtCharsetSetColors(m_wrap_charset->m_rw_charset, &m_foreground_color, &m_background_color);
    }
}

void CCharset::SetForeGroundColorInt(U8 r, U8 g, U8 b, U8 a) {
    if (m_wrap_charset != NULL) {
        m_foreground_color = CDKW_RGBA(r & 0xFF, g & 0xFF, b & 0xFF, a & 0xFF);
        RtCharsetSetColors(m_wrap_charset->m_rw_charset, &m_foreground_color, &m_background_color);
    }
}

void CCharset::SetBackGroundColor(F32 r, F32 g, F32 b, F32 a) {
    if (m_wrap_charset != NULL) {
        m_background_color = CDKW_RGBA(255.0f * r, 255.0f * g, 255.0f * b, 255.0f * a);
        RtCharsetSetColors(m_wrap_charset->m_rw_charset, &m_foreground_color, &m_background_color);
    }
}

void CCharset::SetBackGroundColorInt(U8 r, U8 g, U8 b, U8 a) {
    if (m_wrap_charset != NULL) {
        m_background_color = CDKW_RGBA(r & 0xFF, g & 0xFF, b & 0xFF, a & 0xFF);
        RtCharsetSetColors(m_wrap_charset->m_rw_charset, &m_foreground_color, &m_background_color);
    }
}

BOOL CCharset::IsValid() {
    if (m_wrap_charset != NULL) {
        return m_wrap_charset->m_rw_charset != NULL;
    }

    return FALSE;
}

void CCharset::PrintTextAbsolute(int a1, int a2, char* text) {
    if (m_wrap_charset != NULL && DkDisplayGetEngine()->IsRendering()) {
        m_wrap_charset->PrintAbsolute(a1, a2, text);
    }
}

void CCharset::PrintText(int a1, int a2, DKW_PrintPos a3, char* text) {
    if (m_wrap_charset != NULL && DkDisplayGetEngine()->IsRendering()) {
        m_wrap_charset->Print(a1, a2, a3, text);
    }
}

void CCharset::Print(int a1, int a2, DKW_PrintPos a3, char* text, ...) {
    if (m_wrap_charset != NULL && DkDisplayGetEngine()->IsRendering()) {
        va_list list;
        va_start(list, text);
        va_end(list);
        m_wrap_charset->Print(a1, a2, a3, 0, text, &list);
    }
}

void CCharset::PrintAbsolute(int a1, int a2, char* text, ...) {
    if (m_wrap_charset != NULL && DkDisplayGetEngine()->IsRendering()) {
        va_list list;
        va_start(list, text);
        va_end(list);
        m_wrap_charset->PrintAbsolute(a1, a2, 0, text, &list);
    }
}

}
