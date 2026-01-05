#ifndef PIGLET_CGCNFONT_H
#define PIGLET_CGCNFONT_H

#include <rwsdk/baimage.h>
#include "types.h"

class CGCNFont {
public:
    static RwImage* m_pImage;

public:
    static void Create();
    static void PrintText(int, int, char* text);
};

#endif
