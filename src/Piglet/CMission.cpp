#include "engine/xmd/CChunkIterator.h"
#include "engine/xmd/CXmdFile.h"
#include "CMission.h"
#include "CGame.h"
#include <cstring>
#include <iostream>

u32 CMission::m_MaxNbCookies = 0;

BOOL CMission::LoadConfigFile(u32 a1) {
    u32 unk1 = m_game->m_resource_factory->m_unkC;
    m_game->m_resource_factory->m_unkC = 1;

    char filename[128];
    sprintf(filename, "Missions/Mission%d/Mission%0d.XMD", m_mission_no, m_mission_no);
    void* file = m_game->m_resource_factory->LoadPureFile(filename, NULL);
    if (file == NULL) {
        m_game->m_resource_factory->m_unkC = unk1;
        return FALSE;
    }

    DkXmd::CXmdFile xmd;
    if (!xmd.Parse(file)) {
        m_game->m_resource_factory->m_unkC = unk1;
        delete file;
        return FALSE;
    }

    DkXmd::CChunkIterator file_chunk = xmd.m_chunk_iterator;

    char buf[256];
    DkXmd::CChunkIterator dest1, dest2;
    CResourceFactory* resource_factory;

    if (a1) {
        if (file_chunk.GetChunk("MissionName", dest1)) {
            SetName(dest1.GetStringValue());
        }
        if (file_chunk.GetChunk("StartLockStatus", dest1)) {
            if (dest1.GetS32Value() == 0) {
                m_unk30 = 0;
            } else {
                m_unk30 = 1;
            }
        }
        if (file_chunk.GetChunk("StartCompletionStatus", dest1)) {
            if (dest1.GetS32Value() == 0) {
                m_unk2C = 0;
            } else {
                m_unk2C = 1;
            }
        }
        if (file_chunk.GetChunk("StartRoom", dest1)) {
            m_unkC = dest1.GetS32Value();
            m_rooms[m_unkC] |= (1 << 0);
        }
        if (file_chunk.GetChunk("StartMode", dest1)) {
            m_unk10 = dest1.GetS32Value();
        }

        if (file_chunk.GetChunk("StartPosition", dest1)) {
            if (dest1.GetChunk("X", dest2)) {
                m_start_position_x = dest2.GetFloatValue();
            }
            if (dest1.GetChunk("Y", dest2)) {
                m_start_position_y = dest2.GetFloatValue();
            }
            if (dest1.GetChunk("Z", dest2)) {
                m_start_position_z = dest2.GetFloatValue();
            }
        }
        if (file_chunk.GetChunk("StartRotation", dest1)) {
            if (dest1.GetChunk("X", dest2)) {
                m_start_rotation_x = dest2.GetFloatValue();
            }
            if (dest1.GetChunk("Y", dest2)) {
                m_start_rotation_y = dest2.GetFloatValue();
            }
            if (dest1.GetChunk("Z", dest2)) {
                m_start_rotation_z = dest2.GetFloatValue();
            }
        }

        if (file_chunk.GetChunk("StartFMV", dest1)) {
            SetStartMissionFMV(dest1.GetStringValue());
        }
        if (file_chunk.GetChunk("StartRTC", dest1)) {
            SetStartRTC(dest1.GetStringValue());
        }
        if (file_chunk.GetChunk("EndFMV", dest1)) {
            SetEndMissionFMV(dest1.GetStringValue());
        }
        if (file_chunk.GetChunk("MissionRoomNumbers", dest1)) {
            m_num_rooms = dest1.GetS32Value();
        }
        if (file_chunk.GetChunk("MissionFightWarpModel", dest1)) {
            SetMissionFightWarpModel(dest1.GetStringValue());
        }
        if (file_chunk.GetChunk("MissionTotalCookies", dest1)) {
            m_num_total_cookies = dest1.GetS32Value();
        }

        if (file_chunk.GetChunk("MissionNoNPCRoomList", dest1)) {
            dest1.GetFirstChildChunk(dest1);
            do {
                char* name = dest1.GetName();
                if (name != NULL && strcmp(name, "Room") == 0) {
                    m_rooms[dest1.GetS32Value()] |= (1 << 1);
                } else {
                    break;
                }
            } while (dest1.GetNextSiblingChunk(dest1));
        }
    } else {
        if (file_chunk.GetFirstChildChunk(dest1)) {
            do {
                strcpy(buf, dest1.GetName());

                if (strcmp(buf, "TexDict") == 0) {
                    resource_factory = m_game->m_resource_factory;
                    resource_factory->LoadResource(6, dest1.GetStringValue());
                }
            } while (dest1.GetNextSiblingChunk(dest1));
        }

        if (file_chunk.GetFirstChildChunk(dest1)) {
            do {
                strcpy(buf, dest1.GetName());

                resource_factory = m_game->m_resource_factory;
                if (strcmp(buf, "Mesh") == 0) {
                    resource_factory = m_game->m_resource_factory;
                    resource_factory->LoadResource(1, dest1.GetStringValue());
                } else if (strcmp(buf, "ANM") == 0) {
                    resource_factory = m_game->m_resource_factory;
                    resource_factory->LoadResource(3, dest1.GetStringValue());
                } else if (strcmp(buf, "DMA") == 0) {
                    resource_factory = m_game->m_resource_factory;
                    resource_factory->LoadResource(4, dest1.GetStringValue());
                } else if (strcmp(buf, "TAN") == 0) {
                    resource_factory = m_game->m_resource_factory;
                    resource_factory->LoadResource(5, dest1.GetStringValue());
                } else if (strcmp(buf, "ParticleEmitterFile") == 0) {
                    if (dest1.GetStringValue() != NULL) {
                        resource_factory = m_game->m_resource_factory;
                        resource_factory->LoadResource(9, dest1.GetStringValue());
                    }
                } else if (strcmp(buf, "SndDict") == 0) {
                    resource_factory = m_game->m_resource_factory;
                    resource_factory->LoadResource(10, dest1.GetStringValue());
                } else if (strcmp(buf, "StreamDict") == 0) {
                    resource_factory = m_game->m_resource_factory;
                    resource_factory->LoadResource(12, dest1.GetStringValue());
                } else if (strcmp(buf, "GUI") == 0) {
                    m_game->m_gui_manager->LoadGui(&dest1, 1);
                } else if (strcmp(buf, "Minimap") == 0) {
                    m_game->m_minimap->Load(&dest1);
                }
            } while (dest1.GetNextSiblingChunk(dest1));
        }

        if (file_chunk.GetChunk("MissionNoNPCRoomList", dest1)) {
            dest1.GetFirstChildChunk(dest1);
            do {
                strcpy(buf, dest1.GetName());
                if (strcmp(buf, "Room") == 0) {
                    m_rooms[dest1.GetS32Value()] |= (1 << 1);
                } else {
                    break;
                }
            } while (dest1.GetNextSiblingChunk(dest1));
        }
    }

    m_game->m_resource_factory->m_unkC = unk1;
    delete file;
    return TRUE;
}

void CMission::UnloadConfigFile() {
    m_game->m_gui_manager->UnLoadLevel(1);
    m_game->m_resource_factory->UnloadResources(1);
    m_game->m_minimap->Unload();
}

u32 CMission::GetSaveSize() {
    // FIXME: Calculate this
    return 0x298;
}

void CMission::Initialize() {
    m_game = NULL;
    m_mission_no = -1;

    SetName("");
    SetEndMissionFMV("");
    SetStartMissionFMV("");
    SetStartRTC("");
    SetMissionFightWarpModel("");

    m_unk10 = 0;
    m_unkC = 1;

    m_num_rooms = 1;

    m_start_position_x = 0.0f;
    m_start_position_y = 0.0f;
    m_start_position_z = 0.0f;
    m_start_rotation_x = 0.0f;
    m_start_rotation_y = 0.0f;
    m_start_rotation_z = 0.0f;
    m_unk2C = 0;
    m_unk30 = 0;

    strcpy(m_unk48C[0], "NULL");
    strcpy(m_unk48C[1], "NULL");
    strcpy(m_unk48C[2], "NULL");

    m_num_total_cookies = 0;
    m_num_current_cookies = 0;

    m_MaxNbCookies = 0;

    m_unk34 = 0;

    m_unk4C4 = 0;
    m_unk4C8 = 0;
    m_unk4CC = 0;
    m_unk4D0 = 0;

    for (int i = 0; i < 16; i++) {
        m_rooms[i] = 0;
    }

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 8; j++) {
            m_unk7C[i].unk0[j] = -1;
        }
    }
}

int CMission::AddItem(s8* str_) {
    int ret;

    char* str = (char*)str_;

    if (strcmp(str, m_unk48C[0]) == 0) {
        ret = 0;
    } else if (strcmp(str, m_unk48C[1]) == 0) {
        ret = 1;
    } else if (strcmp(str, m_unk48C[2]) == 0) {
        ret = 2;
    } else if (strcmp("NULL", m_unk48C[0]) == 0) {
        strcpy(m_unk48C[0], str);
        ret = 0;
    } else if (strcmp("NULL", m_unk48C[1]) == 0) {
        strcpy(m_unk48C[1], str);
        ret = 1;
    } else if (strcmp("NULL", m_unk48C[2]) == 0) {
        strcpy(m_unk48C[2], str);
        ret = 2;
    } else {
        ret = -1;
    }

    return ret;
}

void CMission::RemoveItem(s8* str_) {
    char* str = (char*)str_;

    if (strcmp(str, m_unk48C[0]) == 0) {
        strcpy(m_unk48C[0], "NULL");
    } else if (strcmp(str, m_unk48C[1]) == 0) {
        strcpy(m_unk48C[1], "NULL");
    } else if (strcmp(str, m_unk48C[2]) == 0) {
        strcpy(m_unk48C[2], "NULL");
    }
}

void CMission::RemoveItem(int item_no) {
    strcpy(m_unk48C[item_no], "NULL");
}

char* CMission::GetItem(int item_no) {
    if (strcmp("NULL", m_unk48C[item_no]) == 0) {
        return NULL;
    }
    return m_unk48C[item_no];
}

u32 CMission::GetNbTotalCookies() {
    return m_num_total_cookies;
}

u32 CMission::GetCurrentNbCookies() {
    return m_num_current_cookies;
}

u32 CMission::GetMaxCollectedNbCookies() {
    return m_MaxNbCookies;
}

BOOL CMission::IsRoomCompleted(u32 room_no) {
    return (m_rooms[room_no] & 2) != 0;
}

BOOL CMission::IsRoomOpened(u32 room_no) {
    return (m_rooms[room_no] & 1) != 0;
}

BOOL CMission::IsSecondPassCompleted() {
    BOOL ret = TRUE;

    for (int i = 1; i <= m_num_rooms; i++) {
        ret = IsRoomCompleted(i) ? TRUE : FALSE;
        if (ret == FALSE) {
            break;
        }
    }

    return ret;
}
