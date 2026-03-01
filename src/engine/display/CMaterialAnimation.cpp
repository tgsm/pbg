#include "engine/display/CMaterialAnimation.h"

namespace DKDSP {

CMaterialAnimation::CMaterialAnimation() : m_name("") {
    m_duration = 0.0f;
    m_key_table = NULL;
    m_nb_keyframes = 0;
    m_interpolator_types[0] = MAT_ANIM_SCHEME_0;
    m_interpolator_types[1] = MAT_ANIM_SCHEME_0;
    m_interpolator_types[2] = MAT_ANIM_SCHEME_0;
}

CMaterialAnimation::~CMaterialAnimation() {
    Release();
}

void CMaterialAnimation::Release() {
    m_duration = 0.0f;
    m_nb_keyframes = 0;
    if (m_key_table != NULL) {
        delete[] m_key_table;
    }
    m_key_table = NULL;
}

EANIMATIONTYPE CMaterialAnimation::GetType() {
    return ANIMATION_TYPE_TAN;
}

BOOL CMaterialAnimation::Create(int nb_keyframes, F32 duration, int) {
    m_duration = duration;
    m_nb_keyframes = nb_keyframes;
    m_key_table = new SMATERIALANIMKEY[m_nb_keyframes];
    return TRUE;
}

void CMaterialAnimation::SetKey(int keyframe_nb, SMATERIALANIMKEY& key) {
    if (keyframe_nb < 0 || keyframe_nb >= m_nb_keyframes) {
        return;
    }

    m_key_table[keyframe_nb].unk0 = key.unk0;
    m_key_table[keyframe_nb].unk4 = key.unk4;
    m_key_table[keyframe_nb].unk8 = key.unk8;
    m_key_table[keyframe_nb].unkC = key.unkC;
    m_key_table[keyframe_nb].unk10 = key.unk10;
    m_key_table[keyframe_nb].unk14 = key.unk14;
}

void CMaterialAnimation::GetKey(int keyframe_nb, SMATERIALANIMKEY& dest_key) {
    if (keyframe_nb < 0 || keyframe_nb >= m_nb_keyframes) {
        return;
    }

    dest_key.unk0 = m_key_table[keyframe_nb].unk0;
    dest_key.unk4 = m_key_table[keyframe_nb].unk4;
    dest_key.unk8 = m_key_table[keyframe_nb].unk8;
    dest_key.unkC = m_key_table[keyframe_nb].unkC;
    dest_key.unk10 = m_key_table[keyframe_nb].unk10;
    dest_key.unk14 = m_key_table[keyframe_nb].unk14;
}

IMaterialAnimation* CMaterialAnimation::Copy(IMaterialAnimation*) {
    // Is this correct?
    return this;
}

F32 CMaterialAnimation::GetDuration() {
    return m_duration;
}

U32 CMaterialAnimation::GetNumberOfKeyframes() {
    return m_nb_keyframes;
}

U32 CMaterialAnimation::GetSize() {
    return sizeof(CMaterialAnimation) + m_nb_keyframes * sizeof(SMATERIALANIMKEY);
}

void CMaterialAnimation::SetInterpolatorType(EMATANIMCOMPONENT component, EMATANIMSCHEME interpolator_type) {
    m_interpolator_types[component] = interpolator_type;
}

EMATANIMSCHEME CMaterialAnimation::GetInterpolatorType(EMATANIMCOMPONENT component) {
    return m_interpolator_types[component];
}

SMATERIALANIMKEY* CMaterialAnimation::GetKeyTable() {
    return m_key_table;
}

std::string* CMaterialAnimation::GetName() {
    return &m_name;
}

void CMaterialAnimation::SetName(std::string name) {
    m_name.assign(name, 0);
}

}
