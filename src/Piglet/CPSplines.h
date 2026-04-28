#ifndef PIGLET_CPSPLINES_H
#define PIGLET_CPSPLINES_H

#include <string>
#include "engine/wrap/DKW_V3d.h"

class CPigSpline;

// TODO
class CPigSplineBank {
public:
    CPigSpline* GetSplineByName(std::string name);
};

// TODO
class CPigSpline {
public:
    F32 m_unk0;
    U8 m_unk4[0x1C - 0x4];
    F32 m_unk1C;
    U8 m_unk20[0x2C - 0x20];
    F32 m_unk2C;
    U8 m_unk30[0x38 - 0x30];
    U32 m_unk38;
    U8 m_unk3C[4];

public:
    ~CPigSpline();

    U32 SwapMovingDirection();
    F32 NextPosition(F32, CDKW_V3d*, CDKW_V3d*, CDKW_V3d*, F32*);
};
REQUIRE_SIZE(CPigSpline, 0x40);

#endif
