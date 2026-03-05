#ifndef ENGINE_DISPLAY_CSPLINE_H
#define ENGINE_DISPLAY_CSPLINE_H

#include "engine/display/ISpline.h"
#include "engine/wrap/DKW_Spline.h"

namespace DKDSP {

// TODO
class CSpline : public ISpline {
public:
    CDKW_Spline* m_wrap_spline;
    std::string m_unk8;

public:
    CSpline(CDKW_Spline* wrap_spline) : m_unk8("") {
        Construct(wrap_spline);
    }
    ~CSpline();

    virtual int GetNumberOfControlPoints();
    virtual CDKW_V3d* GetControlPoint(int, CDKW_V3d*);

    void Construct(CDKW_Spline* wrap_spline);
    void Release();
};
REQUIRE_SIZE(CSpline, 0xC);

}

#endif
