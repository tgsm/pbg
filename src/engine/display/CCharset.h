#ifndef ENGINE_DISPLAY_CCHARSET_H
#define ENGINE_DISPLAY_CCHARSET_H

class CDKW_Charset;

namespace DKDSP {

// TODO
class CCharset {
public:
    U8 m_unk0[0x14];

public:
    ~CCharset();

    void SetDkWrapCharset(CDKW_Charset* wrap_charset, int);
};
REQUIRE_SIZE(CCharset, 0x14);

}

#endif
