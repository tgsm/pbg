#include "CEntityManager.h"

u32 CEntityManager::GetTypeFromString(std::string type_str) {
    if (type_str == "WorldEntity") {
        return 3;
    } else if (type_str == "WarpEntity") {
        return 4;
    } else if (type_str == "MeshEntity") {
        return 14;
    } else if (type_str == "LightEntity") {
        return 10;
    } else if (type_str == "RTCPlayerEntity") {
        return 12;
    } else if (type_str == "FXEntity") {
        return 13;
    } else if (type_str == "ZoneEntity") {
        return 6;
    } else if (type_str == "SearchableZoneEntity") {
        return 7;
    } else if (type_str == "ScaryFaceBoxEntity") {
        return 8;
    } else if (type_str == "PigletEntity") {
        return 18;
    } else if (type_str == "WinnieEntity") {
        return 19;
    } else if (type_str == "TiggerEntity") {
        return 20;
    } else if (type_str == "NPCEntity") {
        return 21;
    } else if (type_str == "BasicEffalumEntity") {
        return 24;
    } else if (type_str == "BasicWoozleEntity") {
        return 25;
    } else if (type_str == "BasicTreeEntity") {
        return 26;
    } else if (type_str == "BeeEffalumEntity") {
        return 27;
    } else if (type_str == "HideAndSeekWoozleEntity") {
        return 28;
    } else if (type_str == "MirrorWoozleEntity") {
        return 29;
    } else if (type_str == "RoadSweeperEffalumEntity") {
        return 30;
    } else if (type_str == "TubaEffalumEntity") {
        return 31;
    } else if (type_str == "JackpotEffalumEntity") {
        return 32;
    } else if (type_str == "TennisWoozleEntity") {
        return 33;
    } else if (type_str == "DoorEntity") {
        return 34;
    } else if (type_str == "GralosaurusEntity") {
        return 35;
    } else if (type_str == "PathFinder") {
        return 45;
    } else if (type_str == "PushingBoxEntity") {
        return 15;
    } else if (type_str == "TimerEntity") {
        return 11;
    } else if (type_str == "CookieEntity") {
        return 16;
    } else if (type_str == "EscapingObjectEntity") {
        return 39;
    } else if (type_str == "NPCArrowEntity") {
        return 42;
    } else if (type_str == "KeySequenceEntity") {
        return 43;
    } else if (type_str == "3DSoundEntity") {
        return 46;
    } else {
        return 0;
    }
}

u32 CEntityManager::CreateEntity(std::string str1, std::string type, std::string str3) {
    u32 type_id = GetTypeFromString(type);
    return CreateEntity(str1, type_id, str3);
}

u32 CEntityManager::GetHero() {
    u32 entity = GetEntity("Piglet");
    if (entity == 0) {
        entity = GetEntity("Tigger");
        if (entity == 0) {
            entity = GetEntity("Winnie");
        }
    }
    return entity;
}
