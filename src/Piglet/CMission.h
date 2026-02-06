#ifndef PIGLET_CMISSION_H
#define PIGLET_CMISSION_H

#include <string>
#include "engine/wrap/DKW_V3d.h"

class CGame;

struct CMission_UnkSubstruct {
    int unk0[8];
    int unk20[8];
};

class CMission {
public:
    CGame* m_game;
    std::string m_mission_name;
    int m_mission_no;
    U32 m_unkC;
    U32 m_unk10;
    CDKW_V3d m_start_position;
    CDKW_V3d m_start_rotation;
    U32 m_unk2C;
    int m_unk30;
    int m_unk34;
    int m_num_rooms;
    U32 m_rooms[16];
    CMission_UnkSubstruct m_unk7C[16];
    std::string m_start_mission_fmv;
    std::string m_end_mission_fmv;
    std::string m_mission_fight_warp_model;
    std::string m_start_rtc;
    S8 m_unk48C[3][16];
    U32 m_num_total_cookies;
    U32 m_num_current_cookies;
    U32 m_unk4C4[4];

    static U32 m_MaxNbCookies;

public:
    CMission() {}
    ~CMission() {}

    void SetName(std::string name) {
        m_mission_name.assign(name, 0);
    }

    void SetEndMissionFMV(std::string name) {
        m_end_mission_fmv.assign(name, 0);
    }

    void SetStartMissionFMV(std::string name) {
        m_start_mission_fmv.assign(name, 0);
    }

    std::string GetStartRTC() {
        return m_start_rtc;
    }
    void SetStartRTC(std::string name) {
        m_start_rtc.assign(name, 0);
    }

    std::string GetMissionFightWarpModel() {
        return m_mission_fight_warp_model;
    }
    void SetMissionFightWarpModel(std::string name) {
        m_mission_fight_warp_model.assign(name, 0);
    }

    BOOL LoadConfigFile(U32);
    void UnloadConfigFile();
    U32 GetSaveSize();
    void Save(void*);
    void Restore(void*);
    void Initialize();
    int AddItem(S8* str);
    void RemoveItem(S8* str);
    void RemoveItem(int item_no);
    char* GetItem(int item_no);
    U32 GetNbTotalCookies();
    U32 GetCurrentNbCookies();
    U32 GetMaxCollectedNbCookies();
    BOOL IsRoomCompleted(U32 room_no);
    BOOL IsRoomOpened(U32 room_no);
    BOOL IsSecondPassCompleted();

    U32 GetUnkC() { return m_unkC; }
    CDKW_V3d GetStartPosition() { return m_start_position; }
    CDKW_V3d GetStartRotation() { return m_start_rotation; }
    BOOL Unk30Check() { return m_unk30; }
    int& GetUnk34() { return m_unk34; }
};
REQUIRE_SIZE(CMission, 0x4D4);

#endif
