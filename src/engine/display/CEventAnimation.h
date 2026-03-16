#ifndef ENGINE_DISPLAY_CEVENTANIMATION_H
#define ENGINE_DISPLAY_CEVENTANIMATION_H

#include <string>
#include "engine/display/IEventAnimation.h"
#include "engine/xmd/CXmdFile.h"

namespace DKDSP {

class CEventAnimation : public IEventAnimation {
public:
    std::string m_name;
    SEVENTANIMKEY* m_key_table;
    int m_nb_events;
    F32 m_duration;
    void* m_unk14;
    DkXmd::CXmdFile* m_xmd;

public:
    CEventAnimation();
    ~CEventAnimation();

    virtual EANIMATIONTYPE GetType();
    virtual F32 GetDuration();
    virtual std::string* GetName();
    virtual U32 GetSize();
    virtual U32 GetNumberOfEvents();

    void Release();
    void SetName(std::string name);
    void SetDuration(F32 duration);
    BOOL Create(int nb_events, F32 duration, DkXmd::CXmdFile* xmd, int);
    void SetKey(int key_no, SEVENTANIMKEY& key);
    SEVENTANIMKEY* GetKeyTable();

    void Started(void* data);
    void Stopped(void* data);
    void Paused(void* data);
    void Resumed(void* data);
};
REQUIRE_SIZE(CEventAnimation, 0x1C);

}

#endif
