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

RwMatrix* RwMatrixCreate(void);
int RwMatrixDestroy(RwMatrix* matrix);
void RwMatrixUpdate(RwMatrix* matrix);
void RwMatrixScale(RwMatrix* matrix, RwV3d*, int);

#ifdef __cplusplus
}
#endif

#endif
