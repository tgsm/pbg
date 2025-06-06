#include "CGamePartIntroduction.h"
#include <iostream>

CGamePartIntroduction::CGamePartIntroduction() {
    m_unk0 = 8;
}

CGamePartIntroduction::~CGamePartIntroduction() {

}

U32 CGamePartIntroduction::NextFrame() {
    return 6;
}
