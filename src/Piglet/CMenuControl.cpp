#include "CDebugMenu.h"
#include "engine/display/DkDisplay.h"
#include "CMenuControl.h"
#include <cstring>
#include <iostream>

CMenuControl::CMenuControl(CDebugMenu* debug_menu)
    : m_unk20(0.0f, 1.0f),
      m_unk28(1.0f, 1.0f),
      m_unk30(1.0f, 0.0f),
      m_unk38(0.0f, 0.0f),
      m_unk48(0xFF, 0xFF, 0xFF, 0xFF),
      m_unk4C(0xFF, 0xFF, 0xFF, 0x7F)
{
    m_debug_menu = debug_menu;
    m_id = NULL;
    m_value = 0;
    m_min = 0;
    m_max = 0;
    m_path = NULL;
    m_brush = NULL;
}

CMenuControl::~CMenuControl() {
    if (m_id != NULL) {
        delete[] m_id;
    }

    for (U32 i = 0; i < m_control_values.size(); i++) {
        delete m_control_values[i];
    }

    if (m_path != NULL) {
        delete m_path;
        m_path = NULL;
    }

    if (m_brush != NULL) {
        delete m_brush;
        m_brush = NULL;
    }

    // FIXME: m_control_values.clear();
    m_control_values.m_size = 0;
    if (m_control_values.m_data != NULL) {
        delete m_control_values.m_data;
    }
    m_control_values.m_data = NULL;
    m_control_values.m_capacity = 0;
}

void CMenuControl::DestroyValueList() {
    for (U32 i = 0; i < m_control_values.size(); i++) {
        delete m_control_values[i];
    }
    // FIXME: m_control_values.clear();
    m_control_values.m_size = 0;
    if (m_control_values.m_data != NULL) {
        delete m_control_values.m_data;
    }
    m_control_values.m_data = NULL;
    m_control_values.m_capacity = 0;

    m_value = 0;
    m_min = 0;
    m_max = 0;
}

BOOL CMenuControl::Init(DkXmd::CChunkIterator iter) {
    if (strcmp(iter.GetName(), "ControlEntry") != 0) {
        return FALSE;
    }

    DkXmd::CChunkIterator dest;
    if (iter.GetFirstChildChunk(dest)) {
        do {
            if (strcmp(dest.GetName(), "ID") == 0) {
                m_id = new char[strlen(dest.GetStringValue()) + 1];
                strcpy(m_id, dest.GetStringValue());
            } else if (strcmp(dest.GetName(), "DrawingLocation") == 0) {
                int x_, y_, width_, height_;
                DkXmd::CChunkIterator location_chunk_iter;
                if (dest.GetFirstChildChunk(location_chunk_iter)) {
                    do {
                        if (strcmp(location_chunk_iter.GetName(), "x") == 0) {
                            x_ = location_chunk_iter.GetS32Value();
                        } else if (strcmp(location_chunk_iter.GetName(), "y") == 0) {
                            y_ = location_chunk_iter.GetS32Value();
                        } else if (strcmp(location_chunk_iter.GetName(), "width") == 0) {
                            width_ = location_chunk_iter.GetS32Value();
                        } else if (strcmp(location_chunk_iter.GetName(), "height") == 0) {
                            height_ = location_chunk_iter.GetS32Value();
                        }
                    } while (location_chunk_iter.GetNextSiblingChunk(location_chunk_iter));
                }

                int debug2C = m_debug_menu->GetUnk2C();
                int debug28 = m_debug_menu->GetUnk28();

                F32 x = (F32)x_ / (F32)debug28;
                F32 y = (F32)y_ / (F32)debug2C;
                F32 width = (F32)width_ / (F32)debug28;
                F32 height = (F32)height_ / (F32)debug2C;

                m_path = new CDKW_2dPath;
                m_path->Lock();
                m_path->Rect(x, 1.0f - y, width, -height);
                m_path->Unlock();

                m_brush = new CDKW_2dBrush;
                m_brush->Create();
                m_brush->SetUV(&m_unk20, &m_unk28, &m_unk30, &m_unk38);
            } else if (strcmp(dest.GetName(), "ValueList") == 0) {
                DkXmd::CChunkIterator list_chunk_iter;
                if (dest.GetFirstChildChunk(list_chunk_iter)) {
                    do {
                        CControlValue* control_value = new CControlValue;
                        if (control_value->Init(list_chunk_iter)) {
                            AS_ULONG_VECTOR_HACK(m_control_values).push_back(reinterpret_cast<unsigned long>(control_value));
                            m_max++;
                        } else {
                            delete control_value;
                        }
                    } while (list_chunk_iter.GetNextSiblingChunk(list_chunk_iter));
                }
            }
        } while (dest.GetNextSiblingChunk(dest));
    }

    return TRUE;
}

void CMenuControl::SetRange(int min, int max) {
    m_min = min;
    m_max = max + 1;
    if (m_value < m_min) {
        m_value = m_min;
    } else if (m_value >= m_max) {
        m_value = m_max - 1;
    }
}

void CMenuControl::NextValue() {
    if (m_value + 1 < m_max) {
        m_value++;
    } else {
        m_value = m_min;
    }
}

void CMenuControl::PrevValue() {
    if (m_value - 1 >= m_min) {
        m_value--;
    } else {
        m_value = m_max - 1;
    }
}

void CMenuControl::Render(BOOL a1) {
    DkDisplayGetEngine(); // unused

    if (a1) {
        m_brush->SetRGBA(&m_unk48, &m_unk48, &m_unk48, &m_unk48);
    } else {
        m_brush->SetRGBA(&m_unk4C, &m_unk4C, &m_unk4C, &m_unk4C);
    }
    m_brush->SetTexture(m_control_values[m_value]->m_texture->m_wrap_texture);

    m_path->Fill(m_brush);
}
