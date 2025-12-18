#ifndef ENGINE_WRAP_DKW_FRAME_H
#define ENGINE_WRAP_DKW_FRAME_H

#include <rwsdk/badevice.h>
#include <rwsdk/plcore/bamemory.h>
#include <rwsdk/plcore/bavector.h>
#include "engine/wrap/DKW_Node3d.h"
#include "types.h"

class CDKW_Frame : public CDKW_Node3d {
public:
    CDKW_Frame();
    CDKW_Frame(RwFrame* rw_frame);
    virtual ~CDKW_Frame();

    virtual RwFrame* GetRwFrame() { return m_rwframe; }

    void operator delete(void* ptr) {
        ms_NbObject--;
        RwEngineInstance->unk144(ms_pMemEntry, ptr);
    }

    static CDKW_Frame* GetInstance(RwFrame* rw_frame);

    BOOL PluginAttach();
    BOOL PluginDetach();

    typedef CDKW_Frame* (*FrameCallback)(CDKW_Frame*, void*);
    void ManageForAllChildren(FrameCallback callback, void*);

    static void DataConstructor(void*, int, int);
    static void* DataDestructor(void*, int, int);
    static void* DataCopier(void*, const void*, int, int);
    static RwStream* DataReadStream(RwStream*, int, void*, int, int);
    static RwStream* DataWriteStream(RwStream*, int, const void*, int, int);
    static int DataGetStreamSize(const void*, int, int);
    static BOOL FrameReadStream(void*, int, int);

public:
    RwFrame* m_rwframe;
    U32 m_unk8;

    static int m_LocalOffset;
    static int ms_NbObject;
    static RwFreeList* ms_pMemEntry;
}; // Unknown size

#endif
