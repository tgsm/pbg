#include <cstddef>
#include <rwsdk/badevice.h>
#include <rwsdk/plcore/baerr.h>
#include <rwsdk/plcore/bamatrix.h>
#include <rwsdk/plcore/bavector.h>

static RwModuleInfo vectorModule;

typedef RwV3d* (*TransformFunc)(RwV3d*, RwV3d*, RwInt32, RwMatrix*);

typedef struct UnkVectorStruct {
    RwReal* unk0;
    RwReal* unk4;
    TransformFunc transformPoints;
    TransformFunc transformVectors;
} UnkVectorStruct;

static RwBool SqrtTableCreate(void);
static RwBool InvSqrtTableCreate(void);

// Incomplete
static RwV3d* VectorMultPoint(RwV3d* a0, RwV3d* a1, RwInt32 a2, RwMatrix* a3) {
    RwV3d* vec = a0;
    while (--a2 >= 0) {
        RwReal x = a1->x;
        RwReal y = a1->y;
        RwReal z = a1->z;
        RwReal mrx = a3->right.x;
        RwReal mry = a3->right.y;
        RwReal mrz = a3->right.z;
        RwReal mux = a3->up.x;
        RwReal muy = a3->up.y;
        RwReal muz = a3->up.z;
        RwReal max = a3->at.x;
        RwReal may = a3->at.y;
        RwReal maz = a3->at.z;
        a1++;
        vec->x = x * mrx + y * a3->up.x + z * a3->at.x + a3->pos.x;
        vec->y = x * mry + y * a3->up.y + z * a3->at.y + a3->pos.y;
        vec->z = x * mrz + y * a3->up.z + z * a3->at.z + a3->pos.z;
        vec++;
    }
    return vec;
}

// Incomplete
static RwV3d* VectorMultVector(RwV3d* a0, RwV3d* a1, RwInt32 a2, RwMatrix* a3) {
    RwV3d* vec = a0;
    while (--a2 >= 0) {
        RwReal x = a1->x;
        RwReal y = a1->y;
        RwReal z = a1->z;
        RwReal mrx = a3->right.x;
        RwReal mry = a3->right.y;
        RwReal mrz = a3->right.z;
        RwReal mux = a3->up.x;
        RwReal muy = a3->up.y;
        RwReal muz = a3->up.z;
        RwReal max = a3->at.x;
        RwReal may = a3->at.y;
        RwReal maz = a3->at.z;
        a1++;
        vec->x = x * mrx + y * a3->up.x + z * a3->at.x;
        vec->y = x * mry + y * a3->up.y + z * a3->at.y;
        vec->z = x * mrz + y * a3->up.z + z * a3->at.z;
        vec++;
    }
    return vec;
}

RwBool _rwVectorSetMultFn(TransformFunc pointsFunc, TransformFunc vectorsFunc) {
    if (pointsFunc == NULL) {
        pointsFunc = VectorMultPoint;
    }
    ((UnkVectorStruct*)((RwInt32)RwEngineInstance + vectorModule.globalsOffset))->transformPoints = pointsFunc;
    if (vectorsFunc == NULL) {
        vectorsFunc = VectorMultVector;
    }
    ((UnkVectorStruct*)((RwInt32)RwEngineInstance + vectorModule.globalsOffset))->transformVectors = vectorsFunc;

    return TRUE;
}

void _rwV3dNormalize(RwV3d* dest, RwV3d* src) {
    RwReal invSqrt = _rwInvSqrt(src->x * src->x + src->y * src->y + src->z * src->z);
    dest->x = src->x * invSqrt;
    dest->y = src->y * invSqrt;
    dest->z = src->z * invSqrt;
}

RwReal RwV3dNormalize(RwV3d* a, RwV3d* b) {
    RwReal bLength2 = b->x * b->x + b->y * b->y + b->z * b->z;
    RwReal bLength = _rwSqrt(bLength2);
    RwReal bInvLength = _rwInvSqrt(bLength2);
    a->x = b->x * bInvLength;
    a->y = b->y * bInvLength;
    a->z = b->z * bInvLength;
    if (bLength <= 0.0f) {
        RwThrowError(1, 25);
    }
    return bLength;
}

RwReal RwV3dLength(RwV3d* vec) {
    return _rwSqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}

RwReal _rwSqrt(RwReal x) {
    RwInt32* table;
    volatile RwUInt32* as_int = (RwUInt32*)&x;
    if (*as_int != 0) {
        *as_int += 0x800;
        table = (RwInt32*)((UnkVectorStruct*)((RwInt32)RwEngineInstance + vectorModule.globalsOffset))->unk0;
        *as_int = table[((*as_int >> 10) & 0x3FFC) >> 2] + ((*as_int >> 1) & 0x3FC00000);
    }
    return x;
}

RwReal _rwInvSqrt(RwReal x) {
    RwInt32* table;
    volatile RwUInt32* as_int = (RwUInt32*)&x;
    if (*as_int != 0) {
        *as_int += 0x800;
        table = (RwInt32*)((UnkVectorStruct*)((RwInt32)RwEngineInstance + vectorModule.globalsOffset))->unk4;
        *as_int = table[((*as_int >> 10) & 0x3FFC) >> 2] + ((~*as_int >> 1) & 0x3FC00000);
    }
    return x;
}

RwReal RwV2dLength(RwV2d* vec) {
    return _rwSqrt(vec->x * vec->x + vec->y * vec->y);
}

// Equivalent?
RwReal RwV2dNormalize(RwV2d* a, RwV2d* b) {
    RwReal bLength2 = b->x * b->x + b->y * b->y;
    RwReal bLength = _rwSqrt(bLength2);
    RwReal bInvLength = _rwInvSqrt(bLength2);
    a->x = b->x * bInvLength;
    a->y = b->y * bInvLength;
    if (bLength <= 0.0f) {
        RwThrowError(1, 25);
    }
    return bLength;
}

void RwV3dTransformPoints(RwV3d* a0, RwV3d* a1, RwInt32 a2, RwMatrix* a3) {
    ((UnkVectorStruct*)((RwInt32)RwEngineInstance + vectorModule.globalsOffset))->transformPoints(a0, a1, a2, a3);
}

void RwV3dTransformVectors(RwV3d* a0, RwV3d* a1, RwInt32 a2, RwMatrix* a3) {
    ((UnkVectorStruct*)((RwInt32)RwEngineInstance + vectorModule.globalsOffset))->transformVectors(a0, a1, a2, a3);
}

void* _rwVectorClose(void* a0, RwInt32, RwInt32) {
    if (((UnkVectorStruct*)((RwInt32)RwEngineInstance + vectorModule.globalsOffset))->unk4 != NULL) {
        RwFree(((UnkVectorStruct*)((RwInt32)RwEngineInstance + vectorModule.globalsOffset))->unk4);
        ((UnkVectorStruct*)((RwInt32)RwEngineInstance + vectorModule.globalsOffset))->unk4 = NULL;
    }

    if (((UnkVectorStruct*)((RwInt32)RwEngineInstance + vectorModule.globalsOffset))->unk0 != NULL) {
        RwFree(((UnkVectorStruct*)((RwInt32)RwEngineInstance + vectorModule.globalsOffset))->unk0);
        ((UnkVectorStruct*)((RwInt32)RwEngineInstance + vectorModule.globalsOffset))->unk0 = NULL;
    }

    vectorModule.numInstances--;
    return a0;
}

void* _rwVectorOpen(void* a0, RwInt32 offset, RwInt32) {
    vectorModule.globalsOffset = offset;
    ((UnkVectorStruct*)((RwInt32)RwEngineInstance + vectorModule.globalsOffset))->transformPoints = VectorMultPoint;
    ((UnkVectorStruct*)((RwInt32)RwEngineInstance + vectorModule.globalsOffset))->transformVectors = VectorMultVector;

    if (!SqrtTableCreate()) {
        return NULL;
    }
    if (!InvSqrtTableCreate()) {
        return NULL;
    }

    vectorModule.numInstances++;
    return a0;
}
