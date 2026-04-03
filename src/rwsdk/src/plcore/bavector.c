#include <cstddef>
#include <rwsdk/badevice.h>
#include <rwsdk/plcore/baerr.h>
#include <rwsdk/plcore/bamatrix.h>
#include <rwsdk/plcore/bavector.h>

static RwModuleInfo vectorModule;

typedef RwV3d* (*TransformFunc)(RwV3d*, RwV3d*, int, RwMatrix*);

typedef struct UnkVectorStruct {
    float* unk0;
    float* unk4;
    TransformFunc transformPoints;
    TransformFunc transformVectors;
} UnkVectorStruct;

static int SqrtTableCreate();
static int InvSqrtTableCreate();

// Incomplete
static RwV3d* VectorMultPoint(RwV3d* a0, RwV3d* a1, int a2, RwMatrix* a3) {
    RwV3d* vec = a0;
    while (--a2 >= 0) {
        float x = a1->x;
        float y = a1->y;
        float z = a1->z;
        float mrx = a3->right.x;
        float mry = a3->right.y;
        float mrz = a3->right.z;
        float mux = a3->up.x;
        float muy = a3->up.y;
        float muz = a3->up.z;
        float max = a3->at.x;
        float may = a3->at.y;
        float maz = a3->at.z;
        a1++;
        vec->x = x * mrx + y * a3->up.x + z * a3->at.x + a3->pos.x;
        vec->y = x * mry + y * a3->up.y + z * a3->at.y + a3->pos.y;
        vec->z = x * mrz + y * a3->up.z + z * a3->at.z + a3->pos.z;
        vec++;
    }
    return vec;
}

// Incomplete
static RwV3d* VectorMultVector(RwV3d* a0, RwV3d* a1, int a2, RwMatrix* a3) {
    RwV3d* vec = a0;
    while (--a2 >= 0) {
        float x = a1->x;
        float y = a1->y;
        float z = a1->z;
        float mrx = a3->right.x;
        float mry = a3->right.y;
        float mrz = a3->right.z;
        float mux = a3->up.x;
        float muy = a3->up.y;
        float muz = a3->up.z;
        float max = a3->at.x;
        float may = a3->at.y;
        float maz = a3->at.z;
        a1++;
        vec->x = x * mrx + y * a3->up.x + z * a3->at.x;
        vec->y = x * mry + y * a3->up.y + z * a3->at.y;
        vec->z = x * mrz + y * a3->up.z + z * a3->at.z;
        vec++;
    }
    return vec;
}

int _rwVectorSetMultFn(TransformFunc pointsFunc, TransformFunc vectorsFunc) {
    if (pointsFunc == NULL) {
        pointsFunc = VectorMultPoint;
    }
    ((UnkVectorStruct*)((int)RwEngineInstance + vectorModule.globalsOffset))->transformPoints = pointsFunc;
    if (vectorsFunc == NULL) {
        vectorsFunc = VectorMultVector;
    }
    ((UnkVectorStruct*)((int)RwEngineInstance + vectorModule.globalsOffset))->transformVectors = vectorsFunc;

    return 1;
}

void _rwV3dNormalize(RwV3d* dest, RwV3d* src) {
    float invSqrt = _rwInvSqrt(src->x * src->x + src->y * src->y + src->z * src->z);
    dest->x = src->x * invSqrt;
    dest->y = src->y * invSqrt;
    dest->z = src->z * invSqrt;
}

float RwV3dNormalize(RwV3d* a, RwV3d* b) {
    float bLength2 = b->x * b->x + b->y * b->y + b->z * b->z;
    float bLength = _rwSqrt(bLength2);
    float bInvLength = _rwInvSqrt(bLength2);
    a->x = b->x * bInvLength;
    a->y = b->y * bInvLength;
    a->z = b->z * bInvLength;
    if (bLength <= 0.0f) {
        RwThrowError(1, 25);
    }
    return bLength;
}

float RwV3dLength(RwV3d* vec) {
    return _rwSqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}

float _rwSqrt(float x) {
    int* table;
    volatile unsigned int* as_int = (unsigned int*)&x;
    if (*as_int != 0) {
        *as_int += 0x800;
        table = (int*)((UnkVectorStruct*)((int)RwEngineInstance + vectorModule.globalsOffset))->unk0;
        *as_int = table[((*as_int >> 10) & 0x3FFC) >> 2] + ((*as_int >> 1) & 0x3FC00000);
    }
    return x;
}

float _rwInvSqrt(float x) {
    int* table;
    volatile unsigned int* as_int = (unsigned int*)&x;
    if (*as_int != 0) {
        *as_int += 0x800;
        table = (int*)((UnkVectorStruct*)((int)RwEngineInstance + vectorModule.globalsOffset))->unk4;
        *as_int = table[((*as_int >> 10) & 0x3FFC) >> 2] + ((~*as_int >> 1) & 0x3FC00000);
    }
    return x;
}

float RwV2dLength(RwV2d* vec) {
    return _rwSqrt(vec->x * vec->x + vec->y * vec->y);
}

// Equivalent?
float RwV2dNormalize(RwV2d* a, RwV2d* b) {
    float bLength2 = b->x * b->x + b->y * b->y;
    float bLength = _rwSqrt(bLength2);
    float bInvLength = _rwInvSqrt(bLength2);
    a->x = b->x * bInvLength;
    a->y = b->y * bInvLength;
    if (bLength <= 0.0f) {
        RwThrowError(1, 25);
    }
    return bLength;
}

void RwV3dTransformPoints(RwV3d* a0, RwV3d* a1, int a2, RwMatrix* a3) {
    ((UnkVectorStruct*)((int)RwEngineInstance + vectorModule.globalsOffset))->transformPoints(a0, a1, a2, a3);
}

void RwV3dTransformVectors(RwV3d* a0, RwV3d* a1, int a2, RwMatrix* a3) {
    ((UnkVectorStruct*)((int)RwEngineInstance + vectorModule.globalsOffset))->transformVectors(a0, a1, a2, a3);
}

void* _rwVectorClose(void* a0) {
    if (((UnkVectorStruct*)((int)RwEngineInstance + vectorModule.globalsOffset))->unk4 != NULL) {
        RwEngineInstance->memoryFuncs.rwfree(((UnkVectorStruct*)((int)RwEngineInstance + vectorModule.globalsOffset))->unk4);
        ((UnkVectorStruct*)((int)RwEngineInstance + vectorModule.globalsOffset))->unk4 = NULL;
    }

    if (((UnkVectorStruct*)((int)RwEngineInstance + vectorModule.globalsOffset))->unk0 != NULL) {
        RwEngineInstance->memoryFuncs.rwfree(((UnkVectorStruct*)((int)RwEngineInstance + vectorModule.globalsOffset))->unk0);
        ((UnkVectorStruct*)((int)RwEngineInstance + vectorModule.globalsOffset))->unk0 = NULL;
    }

    vectorModule.numInstances--;
    return a0;
}

void* _rwVectorOpen(void* a0, int offset) {
    vectorModule.globalsOffset = offset;
    ((UnkVectorStruct*)((int)RwEngineInstance + vectorModule.globalsOffset))->transformPoints = VectorMultPoint;
    ((UnkVectorStruct*)((int)RwEngineInstance + vectorModule.globalsOffset))->transformVectors = VectorMultVector;

    if (!SqrtTableCreate()) {
        return NULL;
    }
    if (!InvSqrtTableCreate()) {
        return NULL;
    }

    vectorModule.numInstances++;
    return a0;
}
