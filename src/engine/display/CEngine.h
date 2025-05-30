#ifndef ENGINE_DISPLAY_CENGINE_H
#define ENGINE_DISPLAY_CENGINE_H

#include "types.h"

// FIXME: This maybe goes in its own header, but it's only used here.
class CDKW_Memory {
public:
    void* m_file;
    u32 m_unk4;
};

namespace DKDSP {

class IRWStream;

class CEngine {
public:
    virtual void fillerfunc0();
    virtual void fillerfunc1();
    virtual void fillerfunc2();
    virtual void fillerfunc3();
    virtual void fillerfunc4();
    virtual void fillerfunc5();
    virtual void fillerfunc6();
    virtual void fillerfunc7();
    virtual void fillerfunc8();
    virtual void fillerfunc9();
    virtual void fillerfunc10();
    virtual void fillerfunc11();
    virtual void fillerfunc12();
    virtual void fillerfunc13();
    virtual void fillerfunc14();
    virtual void fillerfunc15();
    virtual void fillerfunc16();
    virtual BOOL Update();
    virtual void Release();
    virtual void fillerfunc19();
    virtual void fillerfunc20();
    virtual void fillerfunc21();
    virtual void fillerfunc22();
    virtual void fillerfunc23();
    virtual void fillerfunc24();
    virtual void fillerfunc25();
    virtual void fillerfunc26();
    virtual void fillerfunc27();
    virtual void fillerfunc28();
    virtual void fillerfunc29();
    virtual void fillerfunc30();
    virtual void fillerfunc31();
    virtual void fillerfunc32();
    virtual void fillerfunc33();
    virtual void fillerfunc34();
    virtual void fillerfunc35();
    virtual void fillerfunc36();
    virtual void fillerfunc37();
    virtual void fillerfunc38();
    virtual void fillerfunc39();
    virtual void fillerfunc40();
    virtual void fillerfunc41();
    virtual void fillerfunc42();
    virtual void fillerfunc43();
    virtual void fillerfunc44();
    virtual void fillerfunc45();
    virtual void fillerfunc46();
    virtual void fillerfunc47();
    virtual void fillerfunc48();
    virtual void fillerfunc49();
    virtual void fillerfunc50();
    virtual void fillerfunc51();
    virtual void fillerfunc52();
    virtual void fillerfunc53();
    virtual void fillerfunc54();
    virtual void fillerfunc55();
    virtual void fillerfunc56();
    virtual void fillerfunc57();
    virtual void fillerfunc58();
    virtual void fillerfunc59();
    virtual void fillerfunc60();
    virtual void fillerfunc61();
    virtual void fillerfunc62();
    virtual void fillerfunc63();
    virtual void fillerfunc64();
    virtual void fillerfunc65();
    virtual void fillerfunc66();
    virtual void fillerfunc67();
    virtual void fillerfunc68();
    virtual void fillerfunc69();
    virtual void fillerfunc70();
    virtual void fillerfunc71();
    virtual void fillerfunc72();
    virtual IRWStream* OpenStreamMemory(u32, CDKW_Memory*); // FIXME: RwStreamAccessType, CDKW_Memory*
    virtual void fillerfunc74();
    virtual void DestroyStream(IRWStream*);
};

}

#endif
