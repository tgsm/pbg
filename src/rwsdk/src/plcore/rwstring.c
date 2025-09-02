#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "rwsdk/badevice.h"

static int StrICmp(const char* a, const char* b) {
    if (a != NULL && b != NULL) {
        char a_, b_;
        do {
            a_ = *a;
            b_ = *b;

            // Convert to lowercase
            if (a_ >= 'A' && a_ <= 'Z') {
                a_ += 0x20;
            }
            if (b_ >= 'A' && b_ <= 'Z') {
                b_ += 0x20;
            }

            if (a_ != b_) {
                return a_ - b_;
            }

            a++;
            b++;
        } while (a_ != '\0' && b_ != '\0');

        if (a_ != b_) {
            return a_ - b_;
        }
    }
    return 0;
}

static char* StrUpr(char* str) {
    if (str != NULL) {
        char c;
        char* str_ = str;
        for (; c = *str_, c != '\0'; str_++) {
            if (c >= 'a' && c <= 'z') {
                *str_ = c - 0x20;
            }
        }
    }
    return str;
}

static char* StrLwr(char* str) {
    if (str != NULL) {
        char c;
        char* str_ = str;
        for (; c = *str_, c != '\0'; str_++) {
            if (c >= 'A' && c <= 'Z') {
                *str_ = c + 0x20;
            }
        }
    }
    return str;
}

// Equivalent: regalloc
static char* StrChr(const char* str, int c) {
    char* ret = NULL;
    char ch;
    do {
        ch = *str;
        if (ch == (char)c) {
            ret = (char*)str;
            break;
        }
        str++;
    } while (ch != '\0');
    return ret;
}

// Equivalent: regalloc
static char* StrRChr(const char* str, int c) {
    char* ret = NULL;
    char current;
    do {
        current = *str;
        if (current == (char)c) {
            ret = (char*)str;
        }
        str++;
    } while (current != '\0');
    return ret;
}

int _rwStringOpen() {
    RwEngineInstance->stringFuncs.rwsprintf = sprintf;
    RwEngineInstance->stringFuncs.rwvsprintf = vsprintf;
    RwEngineInstance->stringFuncs.rwstrcpy = strcpy;
    RwEngineInstance->stringFuncs.rwstrncpy = strncpy;
    RwEngineInstance->stringFuncs.rwstrcat = strcat;
    RwEngineInstance->stringFuncs.rwstrncat = strncat;
    RwEngineInstance->stringFuncs.rwstrrchr = StrRChr;
    RwEngineInstance->stringFuncs.rwstrchr = StrChr;
    RwEngineInstance->stringFuncs.rwstrstr = strstr;
    RwEngineInstance->stringFuncs.rwstrcmp = strcmp;
    RwEngineInstance->stringFuncs.rwstricmp = StrICmp;
    RwEngineInstance->stringFuncs.rwstrlen = strlen;
    RwEngineInstance->stringFuncs.rwstrupr = StrUpr;
    RwEngineInstance->stringFuncs.rwstrlwr = StrLwr;
    RwEngineInstance->stringFuncs.rwstrtok = strtok;
    RwEngineInstance->stringFuncs.rwsscanf = sscanf;
    return 1;
}

void _rwStringClose() {

}
