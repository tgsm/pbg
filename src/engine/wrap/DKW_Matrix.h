#ifndef ENGINE_WRAP_DKW_MATRIX_H
#define ENGINE_WRAP_DKW_MATRIX_H

#include <rwsdk/plcore/bamatrix.h>
#include "engine/wrap/DKW_V3d.h"

class CDKW_Matrix : public RwMatrix {
public:
    CDKW_Matrix();
    CDKW_Matrix(const CDKW_Matrix& other);
    CDKW_Matrix(const CDKW_V3d& a);
    CDKW_Matrix(const CDKW_V3d& a, const CDKW_V3d& b, const CDKW_V3d& c, const CDKW_V3d& d);
    void LookAt(CDKW_V3d a, CDKW_V3d b, CDKW_V3d c);
    CDKW_Matrix operator*(const CDKW_Matrix& other) const;

    static CDKW_Matrix ZERO;
    static CDKW_Matrix IDENTITY;
    static CDKW_Matrix MAXTORW;
};
REQUIRE_SIZE(CDKW_Matrix, 0x40);

#endif
