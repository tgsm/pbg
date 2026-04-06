#ifndef ENGINE_WRAP_DKW_IM3DVERTEX_H
#define ENGINE_WRAP_DKW_IM3DVERTEX_H

#include "engine/display/IImBatch.h"

class CDKW_Im3DVertex : public RwIm3DVertex {
public:
    void SetPosition(const CDKW_V3d& pos) {
        this->x = pos.x;
        this->y = pos.y;
        this->z = pos.z;
    }

    void SetRGBA(const U8& r, const U8& g, const U8& b, const U8& a) {
        U8 r_ = r;
        U8 g_ = g;
        U8 b_ = b;
        U8 a_ = a;

        this->r = r_;
        this->g = g_;
        this->b = b_;
        this->a = a_;
    }
};

#endif
