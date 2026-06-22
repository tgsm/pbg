#ifndef RWSDK_PLCORE_BAMATRIX_H
#define RWSDK_PLCORE_BAMATRIX_H

#include <rwsdk/plcore/bavector.h>

#ifdef __cplusplus
extern "C" {
#endif

struct RwMatrixTag {
    RwV3d right;
    RwUInt32 flags;
    RwV3d up;
    RwUInt32 pad1;
    RwV3d at;
    RwUInt32 pad2;
    RwV3d pos;
    RwUInt32 pad3;
};
typedef struct RwMatrixTag RwMatrix;

void* _rwMatrixOpen(void*, RwInt32 offset, RwInt32);
void* _rwMatrixClose(void*, RwInt32, RwInt32);
void RwMatrixUpdate(RwMatrix* matrix);
void RwMatrixMultiply(RwMatrix* dest, RwMatrix* a, RwMatrix* b);
RwMatrix* RwMatrixRotate(RwMatrix* matrix, RwV3d*, RwReal, RwInt32);
void RwMatrixScale(RwMatrix* matrix, RwV3d*, RwInt32);
RwMatrix* RwMatrixTranslate(RwMatrix* matrix, RwV3d*, RwInt32);
RwMatrix* RwMatrixTransform(RwMatrix* matrix, RwMatrix*, RwInt32);
RwBool RwMatrixDestroy(RwMatrix* matrix);
RwMatrix* RwMatrixCreate(void);

#ifdef __cplusplus
}
#endif

#endif
