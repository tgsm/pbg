#ifndef ENGINE_DISPLAY_CMATERIALANIMATION_H
#define ENGINE_DISPLAY_CMATERIALANIMATION_H

#include <string>
#include "engine/display/IMaterialAnimation.h"

namespace DKDSP {

class CMaterialAnimation : public IMaterialAnimation {
public:
    std::string m_name;
    SMATERIALANIMKEY* m_key_table;
    int m_nb_keyframes;
    F32 m_duration;
    EMATANIMSCHEME m_interpolator_types[3];

public:
    CMaterialAnimation();
    ~CMaterialAnimation();

    virtual EANIMATIONTYPE GetType();
    virtual F32 GetDuration();
    virtual std::string* GetName();
    virtual U32 GetSize();
    virtual BOOL Create(int nb_keyframes, F32 duration, int);
    virtual void SetKey(int keyframe_nb, SMATERIALANIMKEY& key);
    virtual void GetKey(int keyframe_nb, SMATERIALANIMKEY& dest_key);
    virtual IMaterialAnimation* Copy(IMaterialAnimation*);
    virtual U32 GetNumberOfKeyframes();
    virtual void SetInterpolatorType(EMATANIMCOMPONENT component, EMATANIMSCHEME scheme);
    virtual EMATANIMSCHEME GetInterpolatorType(EMATANIMCOMPONENT component);

    void Release();
    SMATERIALANIMKEY* GetKeyTable();
    void SetName(std::string name);
};
REQUIRE_SIZE(CMaterialAnimation, 0x20);

}

#endif
