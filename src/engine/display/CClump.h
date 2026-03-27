#ifndef ENGINE_DISPLAY_CCLUMP_H
#define ENGINE_DISPLAY_CCLUMP_H

#include "engine/display/IClump.h"
#include "engine/wrap/DKW_Clump.h"

namespace DKDSP {

class CClump : public IClump {
public:
    U8 m_unk8[4];
    CDKW_Clump* m_wrap_clump;
    U8 m_unk10[0x44 - 0x10];
    CDKW_Frame* m_wrap_frame;
    U8 m_unk48[0x64 - 0x48];

public:
    CClump(CDKW_Clump* wrap_clump);
    CClump(CDKW_Clump* wrap_clump, int);
    ~CClump();

    virtual CDKW_Frame* GetFrame() {
        if (m_wrap_frame != NULL) {
            return m_wrap_frame;
        }

        if (m_wrap_clump != NULL) {
            RwFrame* rw_frame = m_wrap_clump->GetRwFrame();
            if (rw_frame != NULL) {
                return CDKW_Frame::GetInstance(rw_frame);
            } else {
                return NULL;
            }
        } else {
            return NULL;
        }
    }

    virtual void SetFrame(CDKW_Frame* frame);
    virtual void func2();
    virtual BOOL LoadRenderStates(DkXmd::CChunkIterator* iter);
    virtual void func4();
    virtual void func5();
    virtual BOOL IsReoriented();
    virtual void func7();
    virtual int GetNumberOfAtomics();
    virtual CAtomic* GetAtomic(int id);
    virtual void func10();
    virtual void func11();
    virtual void func12();
    virtual void func13();
    virtual void func14();
    virtual void func15();
    virtual int GetAtomicIndexFromID(int id);
    virtual int GetAtomicIndexFromNodeIndex(int node_index);
    virtual void func18();
    virtual void func19();
    virtual U32 GetSize();
    virtual void func21();
    virtual void func22();
    virtual void func23();
    virtual CHierarchy* GetHierarchy();
    virtual void func25();
    virtual void func26();
    virtual void SetController(IController*);
    virtual void func28();
    virtual void func29();
    virtual void func30();
    virtual void Update(F32 dt_maybe);
    virtual void func31();
    virtual void SetAnimation(IAnimation*);
    virtual void func33();
    virtual void func34();
    virtual void func35();
    // TODO

    CDKW_Clump* GetDkWrapClump() {
        return m_wrap_clump;
    }

    void Release();
    void Render();
    void RenderFast();
    CClump* Clone();
};
REQUIRE_SIZE(CClump, 0x64);

}

#endif
