#include "engine/wrap/DKW_Charset.h"
#include "engine/wrap/DKW_Engine.h"
#include <cstring>

void MakeFormat(char* buf, char* format, va_list*);

CDKW_Charset::CDKW_Charset() {
    RwRGBA foreground = CDKW_Engine::GetForegroundColor();
    RwRGBA background = CDKW_Engine::GetBackgroundColor();
    m_rw_charset = RtCharsetCreate(&foreground, &background);
}

CDKW_Charset::~CDKW_Charset() {
    RtCharsetDestroy(m_rw_charset);
}

int CDKW_Charset::Print(int a1, int a2, DKW_PrintPos a3, char* text, ...) {
    va_list list;
    va_start(list, text);
    va_end(list);
    return Print(a1, a2, a3, 1, text, &list);
}

int CDKW_Charset::Print(int a1, int a2, DKW_PrintPos a3, int a4, char* text, va_list* args) {
    va_list list;
    char buf[256];
    RtCharsetDesc desc;
    memset(buf, 0, sizeof(buf) - 1);
    list = *args;

    if (strlen(text) > 256 - 1) {
        char error[32];
        RwSprintf(error, "Number of caracters > %d", 256 - 1);
        return -1;
    }

    list = *args;
    MakeFormat(buf, text, &list);
    int display_width = CDKW_Engine::GetVideoMode().width;
    int display_height = CDKW_Engine::GetVideoMode().height;
    RtCharsetGetDesc(m_rw_charset, &desc);

    int len = RwStrlen(buf);
    a1 *= desc.width;
    a2 *= desc.height;

    if (a3 & PRINT_POS_UNK0) {
        a1 += desc.width * 5;
    } else if (a3 & PRINT_POS_UNK1) {
        a1 += display_width - desc.width * (len + 5);
    }

    if (a3 == PRINT_POS_UNK0 || a3 == PRINT_POS_UNK1 || a3 == 0) {
        a2 += (display_height - desc.height) / 2;
    }

    if (a3 & PRINT_POS_UNK2) {
        a2 += desc.height * 2;
    } else if (a3 & PRINT_POS_UNK3) {
        a2 += display_height - desc.height * 5;
    }

    if (a3 == PRINT_POS_UNK2 || a3 == PRINT_POS_UNK3 || a3 == 0) {
        a1 += (display_width - desc.width * len) / 2;
    }

    RtCharsetPrintBuffered(m_rw_charset, buf, a1, a2, 1);
    return len;
}

int CDKW_Charset::PrintAbsolute(int a1, int a2, int a3, char* text, va_list* args) {
    va_list list;
    char buf[256];
    memset(buf, 0, sizeof(buf) - 1);
    list = *args;

    if (strlen(text) > 256 - 1) {
        char error[32];
        RwSprintf(error, "Number of caracters > %d", 256 - 1);
        return -1;
    }

    list = *args;
    MakeFormat(buf, text, &list);
    RtCharsetPrintBuffered(m_rw_charset, buf, a1, a2, 1);
    return 0;
}

int CDKW_Charset::PrintAbsolute(int a1, int a2, char* text) {
    if (RtCharsetPrintBuffered(m_rw_charset, text, a1, a2, 1) == NULL) {
        return -1;
    }
    return strlen(text);
}

void MakeFormat(char* dest, char* src, va_list* args) {
    static char sstringtemp[256 - 1];

    BOOL got_format_tag;
    BOOL is_flag_character_maybe;
    BOOL bVar2;
    BOOL left_align;

    int num_decimal_places = 0;
    int padding_length = 0;

    if (dest == NULL || src == NULL) {
        return;
    }

    got_format_tag = FALSE;
    is_flag_character_maybe = FALSE;
    bVar2 = FALSE;
    left_align = FALSE;

    for (; *src != '\0'; src++) {
        if (*src == '%') {
            if (got_format_tag == TRUE) {
                *(dest++) = '%';
                got_format_tag = FALSE;
            } else {
                got_format_tag = TRUE;
            }
        } else if (got_format_tag == TRUE) {
            switch (*src) {
                case 'd': {
                    int arg = va_arg(args, int);
                    sstringtemp[0] = '\0';
                    RwSprintf(sstringtemp, "%d", arg);
                    got_format_tag = FALSE;
                    is_flag_character_maybe = FALSE;
                    break;
                }
                case 's': {
                    sstringtemp[0] = '\0';
                    char* arg = va_arg(args, char*);
                    RwStrcat(dest, arg);
                    dest += strlen(arg);
                    got_format_tag = FALSE;
                    break;
                }
                case 'f': {
                    double arg = va_arg(args, double);
                    sstringtemp[0] = '\0';
                    RwSprintf(sstringtemp, "%f", (float)arg);
                    RwStrcpy(dest, sstringtemp);
                    got_format_tag = FALSE;
                    break;
                }
                case 'x': {
                    int arg = va_arg(args, int);
                    sstringtemp[0] = '\0';
                    RwSprintf(sstringtemp, "%x", arg);
                    RwStrcpy(dest, sstringtemp);
                    got_format_tag = FALSE;
                    break;
                }
                default:
                    is_flag_character_maybe = TRUE;
                    sstringtemp[0] = '\0';
                    break;
            }

            if (is_flag_character_maybe && got_format_tag) {
                if (!bVar2) {
                    if (*src == '.') {
                        bVar2 = TRUE;
                    } else if (*src >= '0' && *src <= '9') {
                        padding_length = *src + padding_length * 10;
                        padding_length -= '0';
                    } else if (*src == '-') {
                        left_align = TRUE;
                    }
                } else {
                    if (*src >= '0' && *src <= '9') {
                        num_decimal_places = *src + num_decimal_places * 10;
                        num_decimal_places -= '0';
                    }
                }
            }

            if (!left_align && sstringtemp[0] != '\0') {
                int len = RwStrlen(sstringtemp);
                if (len < padding_length) {
                    while (len < padding_length) {
                        *(dest++) = ' ';
                        len++;
                    }
                }
            }

            int i = 0;
            int iVar14 = 0;
            while (sstringtemp[i] != '\0' && (!is_flag_character_maybe || iVar14 < num_decimal_places)) {
                *(dest++) = sstringtemp[i];
                i++;
                if (!bVar2) {
                    iVar14++;
                }
                if (i > 0 && sstringtemp[i - 1] == '.') {
                    bVar2 = FALSE;
                }
            }

            if (left_align && sstringtemp[0] != '\0') {
                int len = RwStrlen(sstringtemp);
                if (len < padding_length) {
                    while (len < padding_length) {
                        *(dest++) = ' ';
                        len++;
                    }
                }
            }

            if (is_flag_character_maybe && !got_format_tag) {
                is_flag_character_maybe = FALSE;
                left_align = FALSE;
                num_decimal_places = 0;
                padding_length = 0;
            }
        } else {
            *(dest++) = *src;
        }
    }

    *dest = '\0';
}
