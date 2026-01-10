#ifndef ENGINE_WRAP_DKW_MATRIX_H
#define ENGINE_WRAP_DKW_MATRIX_H

#include <rwsdk/plcore/bamatrix.h>
#include "engine/wrap/DKW_V3d.h"

// TODO
class CDKW_Matrix : public RwMatrix {
public:
    CDKW_Matrix();
    CDKW_Matrix(const CDKW_Matrix& other);
    CDKW_Matrix(const CDKW_V3d&, const CDKW_V3d&, const CDKW_V3d&, const CDKW_V3d&);
    CDKW_Matrix LookAt(CDKW_V3d, CDKW_V3d, CDKW_V3d);
    CDKW_Matrix operator*(const CDKW_Matrix& other) const;

    static CDKW_Matrix IDENTITY;
};
REQUIRE_SIZE(CDKW_Matrix, 0x40);

#endif
