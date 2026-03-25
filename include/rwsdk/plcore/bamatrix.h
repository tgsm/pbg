#ifndef RWSDK_PLCORE_BAMATRIX_H
#define RWSDK_PLCORE_BAMATRIX_H

#include <rwsdk/plcore/bavector.h>

#ifdef __cplusplus
extern "C" {
#endif

struct RwMatrixTag {
    RwV3d right;
    unsigned int flags;
    RwV3d up;
    unsigned int pad1;
    RwV3d at;
    unsigned int pad2;
    RwV3d pos;
    unsigned int pad3;
};
typedef struct RwMatrixTag RwMatrix;

void RwMatrixUpdate(RwMatrix* matrix);
void RwMatrixMultiply(RwMatrix* dest, RwMatrix* a, RwMatrix* b);
RwMatrix* RwMatrixRotate(RwMatrix* matrix, RwV3d*, float, int);
void RwMatrixScale(RwMatrix* matrix, RwV3d*, int);
RwMatrix* RwMatrixTranslate(RwMatrix* matrix, RwV3d*, int);
RwMatrix* RwMatrixTransform(RwMatrix* matrix, RwMatrix*, int);
int RwMatrixDestroy(RwMatrix* matrix);
RwMatrix* RwMatrixCreate(void);

#ifdef __cplusplus
}
#endif

#endif
