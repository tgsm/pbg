#include "CGamePartIntroduction.h"
#include <iostream>

CGamePartIntroduction::CGamePartIntroduction() {
    m_type = GAME_PART_TYPE_INTRODUCTION;
}

CGamePartIntroduction::~CGamePartIntroduction() {

}

U32 CGamePartIntroduction::NextFrame() {
    return GAME_PART_TYPE_FRONTEND;
}
