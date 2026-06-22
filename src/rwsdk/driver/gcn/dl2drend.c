#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/vi.h>
#include <rwsdk/bacamera.h>
#include <rwsdk/badevice.h>
#include <rwsdk/batextur.h>
#include <rwsdk/plcore/baerr.h>
#include <rwsdk/plcore/bamatrix.h>

extern RwTexture* _RwDlTexture;
extern GXRenderModeObj* _RwDlRenderMode;
extern RwBool _RwDlFSAA;
extern RwBool _RwDlFSAATop;
extern RwInt32 _RwDlHalfHeight;

static RwReal _rwDlProjectionMatrix[7];

static GXPrimitive _rwDlPrimConvTbl[] = {
    0,
    GX_LINES,
    GX_LINESTRIP,
    GX_TRIANGLES,
    GX_TRIANGLESTRIP,
    GX_TRIANGLEFAN,
    GX_POINTS,
};

extern void _rwDlTextureRasterFlush(void);

void _rw2DRenderPrimitiveInit(void) {
    static RwReal posMatrix[3][4] = {
         1.0f,  0.0f,  0.0f, 0.0f,
         0.0f,  1.0f,  0.0f, 0.0f,
         0.0f,  0.0f, -1.0f, 0.0f,
    };
    RwReal ortho[4][4];

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_POS_XYZ, GX_RGBA8, 0);
    GXSetNumTevStages(1);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);

    if (_RwDlTexture->raster != NULL) {
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
        GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        _rwDlTextureRasterFlush();
    } else {
        GXSetNumTexGens(0);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    }

    if (_RwDlRenderMode->field_rendering) {
        GXSetViewportJitter(0.0f, 0.0f, _RwDlRenderMode->fbWidth, _RwDlRenderMode->xfbHeight, 0.0f, 1.0f, VIGetNextField() ^ 1);
    } else {
        GXSetViewport(0.0f, 0.0f, _RwDlRenderMode->fbWidth, _RwDlRenderMode->xfbHeight, 0.0f, 1.0f);
    }

    if (!_RwDlFSAA) {
        GXSetScissor(0, 0, _RwDlRenderMode->fbWidth, _RwDlRenderMode->xfbHeight);
    } else if (_RwDlFSAATop) {
        GXSetScissor(0, 0, _RwDlRenderMode->fbWidth, _RwDlHalfHeight + 2);
        GXSetScissorBoxOffset(0, 0);
    } else {
        GXSetScissor(0, _RwDlHalfHeight - 2, _RwDlRenderMode->fbWidth, _RwDlHalfHeight + 2);
        GXSetScissorBoxOffset(0, _RwDlHalfHeight - 2);
    }

    C_MTXOrtho(ortho, 0.0f, _RwDlRenderMode->xfbHeight - 1, 0.0f, _RwDlRenderMode->fbWidth - 1, 0.0f, 1.0f);
    GXGetProjectionv(_rwDlProjectionMatrix);
    GXSetProjection(ortho, GX_ORTHOGRAPHIC);
    GXLoadPosMtxImm(posMatrix, 0);
    GXSetCurrentMtx(0);
}

static void _rw2DRenderPrimativeTerm(void) {
    RwRaster* frameBuffer = ((RwCamera*)RwEngineInstance->curCamera)->frameBuffer;

    if (frameBuffer != frameBuffer->parent) {
        if (!_RwDlFSAA) {
            if (_RwDlRenderMode->field_rendering) {
                GXSetViewportJitter(frameBuffer->nOffsetX, frameBuffer->nOffsetY, frameBuffer->width, frameBuffer->height, 0.0f, 1.0f, VIGetNextField() ^ 1);
            } else {
                GXSetViewport(frameBuffer->nOffsetX, frameBuffer->nOffsetY, frameBuffer->width, frameBuffer->height, 0.0f, 1.0f);
            }
            GXSetScissor(frameBuffer->nOffsetX, frameBuffer->nOffsetY, frameBuffer->width, frameBuffer->height);
        } else {
            if (_RwDlRenderMode->field_rendering) {
                GXSetViewportJitter(frameBuffer->nOffsetX, frameBuffer->nOffsetY, frameBuffer->width, frameBuffer->height, 0.0f, 1.0f, VIGetNextField() ^ 1);
            } else {
                GXSetViewport(frameBuffer->nOffsetX, frameBuffer->nOffsetY * 2, frameBuffer->width, frameBuffer->height * 2, 0.0f, 1.0f);
            }

            if (_RwDlFSAATop) {
                if ((frameBuffer->nOffsetY + frameBuffer->height) * 2 <= _RwDlHalfHeight + 2) {
                    GXSetScissor(frameBuffer->nOffsetX, frameBuffer->nOffsetY * 2, _RwDlRenderMode->fbWidth, frameBuffer->height * 2);
                } else if (frameBuffer->nOffsetY * 2 > _RwDlHalfHeight + 2) {
                    GXSetScissor(0, 0, _RwDlRenderMode->fbWidth, _RwDlHalfHeight + 2);
                } else {
                    GXSetScissor(frameBuffer->nOffsetX, frameBuffer->nOffsetY * 2, _RwDlRenderMode->fbWidth, _RwDlHalfHeight + 2);
                }
                GXSetScissorBoxOffset(0, 0);
            } else {
                if (frameBuffer->nOffsetY * 2 >= _RwDlHalfHeight - 2) {
                    GXSetScissor(frameBuffer->nOffsetX, frameBuffer->nOffsetY * 2, _RwDlRenderMode->fbWidth, frameBuffer->height * 2);
                } else if ((frameBuffer->nOffsetY + frameBuffer->height) * 2 < _RwDlHalfHeight + 2) {
                    GXSetScissor(0, _RwDlHalfHeight - 2, _RwDlRenderMode->fbWidth, _RwDlHalfHeight + 2);
                } else {
                    GXSetScissor(frameBuffer->nOffsetX, _RwDlHalfHeight - 2, _RwDlRenderMode->fbWidth, frameBuffer->height * 2);
                }
                GXSetScissorBoxOffset(0, _RwDlHalfHeight - 2);
            }
        }
    }

    GXSetProjectionv(_rwDlProjectionMatrix);
}

// Equivalent: regalloc
RwBool _rwDlIm2DRenderTriangle(struct rwGameCube2DVertex* vertices, RwInt32, RwInt32 a2, RwInt32 a3, RwInt32 a4) {
    struct rwGameCube2DVertex* v1 = &vertices[a2];
    struct rwGameCube2DVertex* v2 = &vertices[a3];
    struct rwGameCube2DVertex* v3 = &vertices[a4];

    _rw2DRenderPrimitiveInit();
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);

    if (_RwDlTexture->raster != NULL) {
        GXPosition3f32(v1->x, v1->y, v1->z);
        GXColor4u8(v1->r, v1->g, v1->b, v1->a);
        GXTexCoord2f32(v1->u, v1->v);

        GXPosition3f32(v2->x, v2->y, v2->z);
        GXColor4u8(v2->r, v2->g, v2->b, v2->a);
        GXTexCoord2f32(v2->u, v2->v);

        GXPosition3f32(v3->x, v3->y, v3->z);
        GXColor4u8(v3->r, v3->g, v3->b, v3->a);
        GXTexCoord2f32(v3->u, v3->v);
    } else {
        GXPosition3f32(v1->x, v1->y, v1->z);
        GXColor4u8(v1->r, v1->g, v1->b, v1->a);

        GXPosition3f32(v2->x, v2->y, v2->z);
        GXColor4u8(v2->r, v2->g, v2->b, v2->a);

        GXPosition3f32(v3->x, v3->y, v3->z);
        GXColor4u8(v3->r, v3->g, v3->b, v3->a);
    }

    _rw2DRenderPrimativeTerm();
    return TRUE;
}

// Equivalent: regalloc
RwBool _rwDlIm2DRenderLine(struct rwGameCube2DVertex* vertices, RwInt32, RwInt32 a2, RwInt32 a3) {
    struct rwGameCube2DVertex* v1 = &vertices[a2];
    struct rwGameCube2DVertex* v2 = &vertices[a3];

    _rw2DRenderPrimitiveInit();
    GXBegin(GX_LINES, GX_VTXFMT0, 2);

    if (_RwDlTexture->raster != NULL) {
        GXPosition3f32(v1->x, v1->y, v1->z);
        GXColor4u8(v1->r, v1->g, v1->b, v1->a);
        GXTexCoord2f32(v1->u, v1->v);

        GXPosition3f32(v2->x, v2->y, v2->z);
        GXColor4u8(v2->r, v2->g, v2->b, v2->a);
        GXTexCoord2f32(v2->u, v2->v);
    } else {
        GXPosition3f32(v1->x, v1->y, v1->z);
        GXColor4u8(v1->r, v1->g, v1->b, v1->a);

        GXPosition3f32(v2->x, v2->y, v2->z);
        GXColor4u8(v2->r, v2->g, v2->b, v2->a);
    }

    _rw2DRenderPrimativeTerm();
    return TRUE;
}

// Incomplete
RwBool _rwDlIm2DRenderPrimitive(RwPrimitiveType type, struct rwGameCube2DVertex* vertices, RwInt32 numVertices) {
    _rw2DRenderPrimitiveInit();
    GXBegin(_rwDlPrimConvTbl[type], 0, numVertices);

    switch (type) {
        case rwPRIMTYPEPOLYLINE:
        case rwPRIMTYPETRISTRIP:
        case rwPRIMTYPETRIFAN: {
            if (_RwDlTexture->raster != NULL) {
                RwInt32 i;
                for (i = 0; i != numVertices; i++) {
                    GXPosition3f32(vertices[i].x, vertices[i].y, vertices[i].z);
                    GXColor4u8(vertices[i].r, vertices[i].g, vertices[i].b, vertices[i].a);
                    GXTexCoord2f32(vertices[i].u, vertices[i].v);
                }
            } else {
                RwInt32 i;
                for (i = 0; i != numVertices; i++) {
                    GXPosition3f32(vertices[i].x, vertices[i].y, vertices[i].z);
                    GXColor4u8(vertices[i].r, vertices[i].g, vertices[i].b, vertices[i].a);
                }
            }
            break;
        }
        case rwPRIMTYPELINELIST: {
            if (_RwDlTexture->raster != NULL) {
                RwInt32 i;
                struct rwGameCube2DVertex* vertex = vertices;
                for (i = 0; i < numVertices; i += 2) {
                    struct rwGameCube2DVertex* vertex = &vertices[i];
                    GXPosition3f32(vertex->x, vertex->y, vertex->z);
                    GXColor4u8(vertex->r, vertex->g, vertex->b, vertex->a);
                    GXTexCoord2f32(vertex->u, vertex->v);
                    vertex = &vertices[i + 1];
                    GXPosition3f32(vertex->x, vertex->y, vertex->z);
                    GXColor4u8(vertex->r, vertex->g, vertex->b, vertex->a);
                    GXTexCoord2f32(vertex->u, vertex->v);
                }
            } else {
                RwInt32 i;
                for (i = 0; i < numVertices; i += 2) {
                    struct rwGameCube2DVertex* vertex = &vertices[i];
                    GXPosition3f32(vertex->x, vertex->y, vertex->z);
                    GXColor4u8(vertex->r, vertex->g, vertex->b, vertex->a);
                    vertex = &vertices[i + 1];
                    GXPosition3f32(vertex->x, vertex->y, vertex->z);
                    GXColor4u8(vertex->r, vertex->g, vertex->b, vertex->a);
                }
            }
            break;
        }
        case rwPRIMTYPETRILIST: {
            RwInt32 unk = numVertices / 3;
            if (_RwDlTexture->raster != NULL) {
                RwInt32 i;
                for (i = 0; i < numVertices; i += 3) {
                    GXPosition3f32(vertices[i].x, vertices[i].y, vertices[i].z);
                    GXColor4u8(vertices[i].r, vertices[i].g, vertices[i].b, vertices[i].a);
                    GXTexCoord2f32(vertices[i].u, vertices[i].v);

                    GXPosition3f32(vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z);
                    GXColor4u8(vertices[i + 1].r, vertices[i + 1].g, vertices[i + 1].b, vertices[i + 1].a);
                    GXTexCoord2f32(vertices[i + 1].u, vertices[i + 1].v);

                    GXPosition3f32(vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].z);
                    GXColor4u8(vertices[i + 2].r, vertices[i + 2].g, vertices[i + 2].b, vertices[i + 2].a);
                    GXTexCoord2f32(vertices[i + 2].u, vertices[i + 2].v);
                }
            } else {
                RwInt32 i;
                for (i = 0; i < numVertices; i += 3) {
                    GXPosition3f32(vertices[i].x, vertices[i].y, vertices[i].z);
                    GXColor4u8(vertices[i].r, vertices[i].g, vertices[i].b, vertices[i].a);

                    GXPosition3f32(vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z);
                    GXColor4u8(vertices[i + 1].r, vertices[i + 1].g, vertices[i + 1].b, vertices[i + 1].a);

                    GXPosition3f32(vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].z);
                    GXColor4u8(vertices[i + 2].r, vertices[i + 2].g, vertices[i + 2].b, vertices[i + 2].a);
                }
            }
            break;
        }
        default:
            RwThrowError(1, 37);
            break;
    }

    _rw2DRenderPrimativeTerm();
    return TRUE;
}

// Incomplete
RwBool _rwDlIm2DRenderIndexedPrimitive(RwPrimitiveType type, struct rwGameCube2DVertex* vertices, RwInt32 numVertices, RwUInt16* indices, RwInt32 numIndices) {
    _rw2DRenderPrimitiveInit();
    GXBegin(_rwDlPrimConvTbl[type], GX_VTXFMT0, numIndices);

    switch (type) {
        case rwPRIMTYPEPOLYLINE:
        case rwPRIMTYPETRISTRIP:
        case rwPRIMTYPETRIFAN: {
            if (_RwDlTexture->raster != NULL) {
                RwInt32 i;
                for (i = 0; i != numIndices; i++) {
                    struct rwGameCube2DVertex* vertex = &vertices[indices[i]];
                    GXPosition3f32(vertex->x, vertex->y, vertex->z);
                    GXColor4u8(vertex->r, vertex->g, vertex->b, vertex->a);
                    GXTexCoord2f32(vertex->u, vertex->v);
                }
            } else {
                RwInt32 i;
                for (i = 0; i != numIndices; i++) {
                    struct rwGameCube2DVertex* vertex = &vertices[indices[i]];
                    GXPosition3f32(vertex->x, vertex->y, vertex->z);
                    GXColor4u8(vertex->r, vertex->g, vertex->b, vertex->a);
                }
            }
            break;
        }
        case rwPRIMTYPELINELIST: {
            if (_RwDlTexture->raster != NULL) {
                RwInt32 i;
                for (i = 0; i < numIndices; i += 2) {
                    struct rwGameCube2DVertex* vertex = &vertices[indices[i]];
                    GXPosition3f32(vertex->x, vertex->y, vertex->z);
                    GXColor4u8(vertex->r, vertex->g, vertex->b, vertex->a);
                    GXTexCoord2f32(vertex->u, vertex->v);
                    vertex = &vertices[indices[i + 1]];
                    GXPosition3f32(vertex->x, vertex->y, vertex->z);
                    GXColor4u8(vertex->r, vertex->g, vertex->b, vertex->a);
                    GXTexCoord2f32(vertex->u, vertex->v);
                }
            } else {
                RwInt32 i;
                for (i = 0; i < numIndices; i += 2) {
                    struct rwGameCube2DVertex* vertex = &vertices[indices[i]];
                    GXPosition3f32(vertex->x, vertex->y, vertex->z);
                    GXColor4u8(vertex->r, vertex->g, vertex->b, vertex->a);
                    vertex = &vertices[indices[i + 1]];
                    GXPosition3f32(vertex->x, vertex->y, vertex->z);
                    GXColor4u8(vertex->r, vertex->g, vertex->b, vertex->a);
                }
            }
            break;
        }
        case rwPRIMTYPETRILIST: {
            RwInt32 unk = numVertices / 3;
            if (_RwDlTexture->raster != NULL) {
                RwInt32 i;
                for (i = 0; i < unk; i += 3) {
                    struct rwGameCube2DVertex* vertex = &vertices[indices[i]];
                    GXPosition3f32(vertex->x, vertex->y, vertex->z);
                    GXColor4u8(vertex->r, vertex->g, vertex->b, vertex->a);
                    GXTexCoord2f32(vertex->u, vertex->v);
                    vertex = &vertices[indices[i + 1]];
                    GXPosition3f32(vertex->x, vertex->y, vertex->z);
                    GXColor4u8(vertex->r, vertex->g, vertex->b, vertex->a);
                    GXTexCoord2f32(vertex->u, vertex->v);
                    vertex = &vertices[indices[i + 2]];
                    GXPosition3f32(vertex->x, vertex->y, vertex->z);
                    GXColor4u8(vertex->r, vertex->g, vertex->b, vertex->a);
                    GXTexCoord2f32(vertex->u, vertex->v);
                }
            } else {
                RwInt32 i;
                for (i = 0; i < unk; i += 3) {
                    struct rwGameCube2DVertex* vertex = &vertices[indices[i]];
                    GXPosition3f32(vertex->x, vertex->y, vertex->z);
                    GXColor4u8(vertex->r, vertex->g, vertex->b, vertex->a);
                    vertex = &vertices[indices[i + 1]];
                    GXPosition3f32(vertex->x, vertex->y, vertex->z);
                    GXColor4u8(vertex->r, vertex->g, vertex->b, vertex->a);
                    vertex = &vertices[indices[i + 2]];
                    GXPosition3f32(vertex->x, vertex->y, vertex->z);
                    GXColor4u8(vertex->r, vertex->g, vertex->b, vertex->a);
                }
            }
            break;
        }
        default:
            RwThrowError(1, 37);
            break;
    }

    _rw2DRenderPrimativeTerm();
    return TRUE;
}
