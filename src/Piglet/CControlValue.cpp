#include "engine/display/DkDisplay.h"
#include "CControlValue.h"
#include <cstring>
#include <iostream>

CControlValue::CControlValue() {
    m_texture = NULL;
}

CControlValue::~CControlValue() {
    // Really not sure what they were going for here. Did they mean to take the reference of m_value?
    if (m_type == CONTROL_VALUE_TYPE_STRING && m_value[0] != NULL) {
        delete[] (void*)m_value[0];
    }
}

int CControlValue::GetS32Value() {
    switch (m_type) {
        case CONTROL_VALUE_TYPE_INT: {
            int ret;
            memcpy(&ret, &m_value, sizeof(int));
            return ret;
        }
        case CONTROL_VALUE_TYPE_FLOAT: {
            F32 ret;
            memcpy(&ret, &m_value, sizeof(F32));
            return ret;
        }
        case CONTROL_VALUE_TYPE_STRING:
            return -1;
        default:
            return -1;
    }
}

void CControlValue::SetS32Value(int value) {
    m_type = CONTROL_VALUE_TYPE_INT;
    memcpy(&m_value, &value, sizeof(int));
}

BOOL CControlValue::Init(DkXmd::CChunkIterator iter) {
    if (strcmp(iter.GetName(), "ValueEntry") != 0) {
        return FALSE;
    }

    DkXmd::CChunkIterator dest;
    if (iter.GetFirstChildChunk(dest)) {
        do {
            if (strcmp(dest.GetName(), "Value") == 0) {
                switch (dest.GetType()) {
                    case 2: {
                        F32 value = dest.GetFloatValue();
                        m_type = CONTROL_VALUE_TYPE_FLOAT;
                        memcpy(&m_value, &value, sizeof(F32));
                        break;
                    }
                    case 3: {
                        int value = dest.GetS32Value();
                        m_type = CONTROL_VALUE_TYPE_INT;
                        memcpy(&m_value, &value, sizeof(int));
                        break;
                    }
                    case 4: {
                        char* value = dest.GetStringValue();
                        m_type = CONTROL_VALUE_TYPE_STRING;
                        char* buf = new char[strlen(value)];
                        strcpy(buf, value);
                        memcpy(&m_value, &buf, sizeof(int));
                        break;
                    }
                }
            } else if (strcmp(dest.GetName(), "Image") == 0) {
                SetTexture(dest.GetStringValue());
            }
        } while (dest.GetNextSiblingChunk(dest));
    }

    return TRUE;
}

void CControlValue::SetTexture(char* name) {
    m_texture = NULL;
    m_texture = DkDisplayGetEngine()->GetTextureDictionary()->FindTexture(name);
    if (m_texture == NULL) {
        m_texture = DkDisplayGetEngine()->GetTextureDictionary()->LoadTextureFromFile(name, 0x10, 0x100);
    }
}
