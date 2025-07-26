#ifndef PIGLET_CGCNFONT_H
#define PIGLET_CGCNFONT_H

#include "types.h"

struct RwImage;

class CGCNFont {
public:
    static RwImage* m_pImage;

public:
    static void PrintText(int, int, char* text);
};

#endif
