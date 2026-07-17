#ifndef ENGINE_DISPLAY_CCHARSET_H
#define ENGINE_DISPLAY_CCHARSET_H

#include "engine/display/ICharset.h"
#include "engine/wrap/DKW_Charset.h"

namespace DKDSP {

// TODO
class CCharset : public ICharset {
public:
    CDKW_Charset* m_wrap_charset;
    BOOL m_unk8;
    CDKW_RGBA m_foreground_color;
    CDKW_RGBA m_background_color;

public:
    CCharset();
    ~CCharset();

    virtual void Release();
    virtual void SetColors(const CDKW_RGBA& foreground, const CDKW_RGBA& background);
    virtual void SetForeGroundColor(F32 r, F32 g, F32 b, F32 a);
    virtual void SetForeGroundColorInt(U8 r, U8 g, U8 b, U8 a);
    virtual void SetBackGroundColor(F32 r, F32 g, F32 b, F32 a);
    virtual void SetBackGroundColorInt(U8 r, U8 g, U8 b, U8 a);
    virtual BOOL IsValid();
    virtual void PrintTextAbsolute(int, int, char* text);
    virtual void PrintText(int, int, DKW_PrintPos, char* text);
    virtual void Print(int, int, DKW_PrintPos, char*, ...);
    virtual void PrintAbsolute(int, int, char*, ...);

    void SetDkWrapCharset(CDKW_Charset* wrap_charset, BOOL);
};
REQUIRE_SIZE(CCharset, 0x14);

}

#endif
