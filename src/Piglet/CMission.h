#ifndef PIGLET_CMISSION_H
#define PIGLET_CMISSION_H

#include <string>
#include "types.h"

class CGame;

struct CMission_UnkSubstruct {
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
    int unk40;
    int unk44;
    int unk48;
    int unk4C;
    int unk50;
    int unk54;
    int unk58;
    int unk5C;
    int unk60;
    int unk64;
    int unk68;
    int unk6C;
    int unk70;
    int unk74;
    int unk78;
    int unk7C;
    int unk80;
    int unk84;
    int unk88;
    int unk8C;
    int unk90;
    int unk94;
    int unk98;
    int unk9C;
    int unkA0;
    int unkA4;
    int unkA8;
    int unkAC;
    int unkB0;
    int unkB4;
    int unkB8;
    int unkBC;
    int unkC0;
    int unkC4;
    int unkC8;
    int unkCC;
    int unkD0;
    int unkD4;
    int unkD8;
    int unkDC;
    u8 unkE0[0x20];
};

class CMission {
public:
    CGame* m_game;
    std::string m_mission_name;
    int m_mission_no;
    u32 m_unkC;
    u32 m_unk10;
    f32 m_start_position_x;
    f32 m_start_position_y;
    f32 m_start_position_z;
    f32 m_start_rotation_x;
    f32 m_start_rotation_y;
    f32 m_start_rotation_z;
    u32 m_unk2C;
    u32 m_unk30;
    int m_unk34;
    int m_num_rooms;
    u32 m_rooms[16];
    CMission_UnkSubstruct m_unk7C[16];
    std::string m_start_mission_fmv;
    std::string m_end_mission_fmv;
    std::string m_mission_fight_warp_model;
    std::string m_start_rtc;
    char m_unk48C[3][16];
    u32 m_num_total_cookies;
    u32 m_num_current_cookies;
    u32 m_unk4C4;
    u32 m_unk4C8;
    u32 m_unk4CC;
    u32 m_unk4D0;

    static u32 m_MaxNbCookies;

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

    BOOL LoadConfigFile(u32);
    void UnloadConfigFile();
    u32 GetSaveSize();
    void Save(void*);
    void Restore(void*);
    void Initialize();
    int AddItem(s8* str);
    void RemoveItem(s8* str);
    void RemoveItem(int item_no);
    char* GetItem(int item_no);
    u32 GetNbTotalCookies();
    u32 GetCurrentNbCookies();
    u32 GetMaxCollectedNbCookies();
    BOOL IsRoomCompleted(u32 room_no);
    BOOL IsRoomOpened(u32 room_no);
    BOOL IsSecondPassCompleted();
};
REQUIRE_SIZE(CMission, 0x4D4);

#endif
