#include "engine/wrap/DKW_Matrix.h"

// Equivalent: weird scheduling. Are one of the constructors a fakematch?
CDKW_Matrix CDKW_Matrix::ZERO(CDKW_V3d::ZERO, CDKW_V3d::ZERO, CDKW_V3d::ZERO, CDKW_V3d::ZERO);
CDKW_Matrix CDKW_Matrix::IDENTITY(CDKW_V3d::XAXIS, CDKW_V3d::YAXIS, CDKW_V3d::ZAXIS, CDKW_V3d::ZERO);
CDKW_Matrix CDKW_Matrix::MAXTORW(CDKW_V3d::XAXIS, CDKW_V3d(0.0f, 0.0f, -1.0f), CDKW_V3d::YAXIS, CDKW_V3d::ZERO);

CDKW_Matrix::CDKW_Matrix() {

}

CDKW_Matrix::CDKW_Matrix(const CDKW_Matrix& other) {
    *this = other;
}

CDKW_Matrix::CDKW_Matrix(const CDKW_V3d& a, const CDKW_V3d& b, const CDKW_V3d& c, const CDKW_V3d& d) {
    right = a;
    up = b;
    at = c;
    pos = d;
}

void CDKW_Matrix::LookAt(CDKW_V3d a, CDKW_V3d b, CDKW_V3d c) {
    RwV3d local_38 = c;
    F32 dVar2 = RwV3dLength(&at);
    RwV3dLength(&right); // unused
    RwV3dLength(&up); // unused

    at.x = b.x - a.x;
    at.y = b.y - a.y;
    at.z = b.z - a.z;
    if (RwV3dLength(&at) != 0.0f) {
        RwV3dNormalize(&at, &at);
    }
    if (RwV3dLength(&local_38) != 0.0f) {
        RwV3dNormalize(&local_38, &local_38);
    }

    right.x = local_38.y * at.z - local_38.z * at.y;
    right.y = local_38.z * at.x - local_38.x * at.z;
    right.z = local_38.x * at.y - local_38.y * at.x;
    if (RwV3dLength(&right) != 0.0f) {
        RwV3dNormalize(&right, &right);
    } else {
        local_38.x += 0.0001f;
        if (RwV3dLength(&local_38) != 0.0f) {
            RwV3dNormalize(&local_38, &local_38);
        } else {
            local_38.z += 0.0001f;
            if (RwV3dLength(&local_38) != 0.0f) {
                RwV3dNormalize(&local_38, &local_38);
            }
        }

        right.x = local_38.y * at.z - local_38.z * at.y;
        right.y = local_38.z * at.x - local_38.x * at.z;
        right.z = local_38.x * at.y - local_38.y * at.x;
        if (RwV3dLength(&right) != 0.0f) {
            RwV3dNormalize(&right, &right);
        }
    }

    up.x = at.y * right.z - at.z * right.y;
    up.y = at.z * right.x - at.x * right.z;
    up.z = at.x * right.y - at.y * right.x;
    if (RwV3dLength(&up) != 0.0f) {
        RwV3dNormalize(&up, &up);
    }

    at.x *= dVar2;
    at.y *= dVar2;
    at.z *= dVar2;
    right.x *= dVar2;
    right.y *= dVar2;
    right.z *= dVar2;
    up.x *= dVar2;
    up.y *= dVar2;
    up.z *= dVar2;
    pos = a;
}

CDKW_Matrix CDKW_Matrix::operator*(const CDKW_Matrix& other) const {
    RwMatrix result;
    RwMatrixMultiply(&result, (RwMatrix*)this, (RwMatrix*)&other);
    return *(CDKW_Matrix*)&result;
}
